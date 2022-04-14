/*
 * path:   /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2022-04-14T12:13:31+0200
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

#include "config.h"

int g_header_len,
    g_line_len;

char g_user[50],
     g_host[50],
     g_datetime[] = "01.01.1900 00:00:00",
     g_distro[65],
     g_model[65],
     g_kernel[65],
     g_uptime[] = "000d 00h 00m",
     g_pkgs[] = "000000 [portage]",
     g_shell[65],
     g_cpu[65],
     g_mem[65];

const char *remove_char(char *string, const char *remove) {
    int i = 0, j;

    while (i < strlen(string)) {
        if (string[i] == *remove) {
            for (j = i; j < strlen(string); j++)
                string[j] = string[j + 1];
        }
        else i++;
    }

    return string;
}

const int update_line_len(const char *line) {
    if (line == NULL) {
        g_line_len = 0;
    } else if (g_line_len < strlen(line)) {
        g_line_len = strlen(line);
    }

    return g_line_len;
}

const char *set_spacer(const char *character, int length) {
    int i;

    static char spacer[65];

    spacer[0] = '\0';

    for (i = 0; i < length; i++) {
        strcat(spacer, character);
    }

    return spacer;
}

const double get_execution_time(void *print()) {
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

    print();

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spend = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    return time_spend;
}

const char *remove_file(const char *file) {
    static char string[2];

    if (0 == remove(file)) {
        sprintf(string, "x");
    } else {
        sprintf(string, "?");
    }

    return string;
}

void *get_user() {
    sprintf(g_user, "%s", getenv("USER"));

    g_header_len += strlen(g_user);

    return NULL;
}

void *get_host() {
    FILE *file;

    if ((file = fopen("/proc/sys/kernel/hostname", "r"))) {
        fscanf(file, "%s", g_host);
        fclose(file);

        g_header_len += strlen(g_host);
    }

    return NULL;
}

void *get_datetime() {
    time_t raw;
    struct tm * part;
    time(&raw);
    part = localtime(&raw);

    sprintf(g_datetime, "%02d.%02d.%d %02d:%02d:%02d", \
            part->tm_mday, \
            part->tm_mon + 1, \
            part->tm_year + 1900, \
            part->tm_hour, \
            part->tm_min, \
            part->tm_sec);

    g_header_len += strlen(g_datetime);

    return NULL;
}

void *get_distro() {
    char name[15],
         value[65];

    FILE *file;

    if ((file = fopen(CACHE_DISTRO_PATH, "r"))) {
        fscanf(file, "%[^\n]s", g_distro);
        fclose(file);
    } else if ((file = fopen("/etc/os-release", "r"))) {
        while (fscanf(file, " %14[^=]=%64[^\n]", name, value) == 2) {
            if (0 == strcmp(name, "PRETTY_NAME")) {
                file = fopen(CACHE_DISTRO_PATH, "w");
                remove_char(value, "\"");
                fprintf(file, "%s", value);
                strcpy(g_distro, value);
                break;
            }
        }
        fclose(file);
    }

    update_line_len(g_distro);

    return NULL;
}

void *get_model() {
    char name[130] = "",
         version[65] = "";

    FILE *file;

    if ((file = fopen(CACHE_MODEL_PATH, "r"))) {
        fscanf(file, "%[^\n]s", g_model);
        fclose(file);
    } else if ((file = fopen("/sys/devices/virtual/dmi/id/product_name", "r"))) {
        fscanf(file, "%[^\n]s", name);
        fclose(file);

        file = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
        fscanf(file, "%[^\n]s", version);
        fclose(file);
    } else if ((file = fopen("/sys/firmware/devicetree/base/model", "r"))) {
        fscanf(file, "%[^\n]s", name);
        fclose(file);
    } else {
        strcpy(name, "not found");
    }

    if ((file != fopen(CACHE_MODEL_PATH, "r"))) {
        file = fopen(CACHE_MODEL_PATH, "w");
        sprintf(name, "%.64s %.64s", name, version);
        fprintf(file, "%.64s", name);
        sprintf(g_model, "%.64s", name);
        fclose(file);
    }

    update_line_len(g_model);

    return NULL;
}

void *get_kernel() {
    FILE *file;

    if ((file = fopen("/proc/sys/kernel/osrelease", "r"))) {
        fscanf(file, "%[^\n]s", g_kernel);
        fclose(file);
    }

    update_line_len(g_kernel);

    return NULL;
}

void *get_uptime() {
    int sec,
        day,
        hour,
        min;

    FILE *file;

    if ((file = fopen("/proc/uptime", "r"))) {
        fscanf(file, "%d", &sec);
        fclose(file);

        day = sec / 60 / 60 / 24;
        hour = sec / 60 / 60 % 24;
        min = sec / 60 % 60;

        if (0 == day && 0 == hour && 0 == min) {
            sprintf(g_uptime, "0m");
        } else if (0 == day && 0 == hour) {
            sprintf(g_uptime, "%dm", min);
        } else if (0 == day) {
            sprintf(g_uptime, "%dh %dm", hour, min);
        } else {
            sprintf(g_uptime, "%dd %dh %dm", day, hour, min);
        }
    }

    update_line_len(g_uptime);

    return NULL;
}

void *get_pkgs() {
    int pkgs_count = 0;

    FILE *file;
    DIR *dir;

    struct dirent *entry;

    if ((file = fopen(CACHE_PKGS_PATH, "r"))) {
        fscanf(file, "%d", &pkgs_count);
        fclose(file);
    } else {
        if ((dir = opendir(PKGS_PATH)) == NULL) return 0;

        while ((entry = readdir(dir)) != NULL) {
            if (0 == strcmp(entry->d_name,".") ||
                0 == strcmp(entry->d_name,"..")) continue;
            if (entry->d_type == DT_DIR) {
                pkgs_count++;
            }
        }
        file = fopen(CACHE_PKGS_PATH, "w");
        fprintf(file, "%d", pkgs_count);
        fclose(file);
        closedir(dir);
    }

    sprintf(g_pkgs, "%d%s", pkgs_count, PKGS_DESC);

    update_line_len(g_pkgs);

    return NULL;
}

void *get_shell() {
    char shell[20];

    ssize_t len = readlink(SHELL_PATH, shell, sizeof(shell)-1);

    if (len == -1) {
        sprintf(shell, "LINK ERR");
    }

    sprintf(g_shell, "%s [%s]%s%s", \
            SHELL_PATH, shell, \
            INFO_DIVIDER, getenv("SHELL"));

    if (0 != getenv("TERM")) {
        sprintf(g_shell, "%s [%s]", g_shell, getenv("TERM"));
    }

    update_line_len(g_shell);

    return NULL;
}

void *get_cpu() {
    float temp;

    char name[20],
         value[58];

    FILE *file;

    if ((file = fopen(CACHE_CPU_PATH, "r"))) {
        fscanf(file, "%[^\n]s", g_cpu);
        fclose(file);
    } else if ((file = fopen("/proc/cpuinfo", "r"))) {
        while (fscanf(file, " %19[^:]: %57[^\n]", name, value) == 2) {
            if (0 == strcmp(name, "model name	")) {
                file = fopen(CACHE_CPU_PATH, "w");
                fprintf(file, "%s", value);
                strcpy(g_cpu, value);
                break;
            }
        }
        fclose(file);
    }

    if ((file = fopen(CPU_TEMPERATURE_PATH, "r"))) {
        fscanf(file, "%f", &temp);
        temp /= 1000;
        if (0 != temp) {
            sprintf(g_cpu, "%s [%.1f%s]", g_cpu, temp, CPU_TEMPERATURE);
        }
        fclose(file);
    }

    update_line_len(g_cpu);

    return NULL;
}

void *get_mem() {
    int value,
        mem_total,
        mem_free,
        mem_available,
        mem_buffers,
        mem_cached,
        mem_shared,
        mem_reclaimable,
        swap_total,
        swap_free,
        swap_available;

    float mem_percent,
          swap_percent;

    char name[20];

    FILE *file;

    if ((file = fopen("/proc/meminfo", "r"))) {
        while (fscanf(file, "%15s  %d %*s", name, &value) == 2) {
            if (0 == strcmp(name, "MemTotal:")) {
                mem_total = value;
            } else if (0 == strcmp(name, "MemFree:")) {
                mem_free = value;
            } else if (0 == strcmp(name, "MemAvailable:")) {
                mem_available = value;
            } else if (0 == strcmp(name, "Buffers:")) {
                mem_buffers = value;
            } else if (0 == strcmp(name, "Cached:")) {
                mem_cached = value;
            } else if (0 == strcmp(name, "SwapTotal:")) {
                swap_total = value;
            } else if (0 == strcmp(name, "SwapFree:")) {
                swap_free = value;
            } else if (0 == strcmp(name, "Shmem:")) {
                mem_shared = value;
            } else if (0 == strcmp(name, "SReclaimable:")) {
                mem_reclaimable = value;
                break;
            }
        }
        fclose(file);

        if (2 == MEMORY_METHOD) {
            mem_available = (mem_total - mem_free - mem_buffers - mem_cached \
                    - mem_reclaimable) / 1024;
        } else if (1 == MEMORY_METHOD) {
            mem_available = (mem_total + mem_shared - mem_free - mem_buffers \
                    - mem_cached - mem_reclaimable) / 1024;
        } else {
            mem_available = (mem_total - mem_available) / 1024;
        }

        mem_total /= 1024;
        mem_percent = (float) mem_available / mem_total * 100;

        if (0 < swap_total) {
            swap_available = (swap_total - swap_free) / 1024;
            swap_total /= 1024;
            swap_percent = (float) swap_available / swap_total * 100;
        }

        if (0 == strcmp(MEMORY_UNIT, "MiB") \
        || (0 == strcmp(MEMORY_UNIT, "auto") && 1024 > mem_total)) {
            if (0 == swap_total) {
                sprintf(g_mem, "%dMiB%s%dMiB [%.1f%%]", \
                        mem_available, \
                        MEMORY_DIVIDER, \
                        mem_total, \
                        mem_percent);
            } else {
                sprintf(g_mem, "%dMiB%s%dMiB [%.1f%%]%s%dMiB%s%dMiB [%.1f%%]", \
                        mem_available, \
                        MEMORY_DIVIDER, \
                        mem_total, \
                        mem_percent, \
                        INFO_DIVIDER, \
                        swap_available, \
                        MEMORY_DIVIDER, \
                        swap_total, \
                        swap_percent);
            }
        } else if (0 == strcmp(MEMORY_UNIT, "GiB") \
               || (0 == strcmp(MEMORY_UNIT, "auto") && 1024 <= mem_total)) {
            if (0 == swap_total) {
                sprintf(g_mem, "%.2fGiB%s%.2fGiB [%.1f%%]", \
                        (float) mem_available / 1024, \
                        MEMORY_DIVIDER, \
                        (float) mem_total / 1024, \
                        mem_percent);
            } else {
                sprintf(g_mem, "%.2fGiB%s%.2fGiB [%.1f%%]%s%.2fGiB%s%.2fGiB [%.1f%%]", \
                        (float) mem_available / 1024, \
                        MEMORY_DIVIDER, \
                        (float) mem_total / 1024, \
                        mem_percent, \
                        INFO_DIVIDER, \
                        (float) swap_available / 1024, \
                        MEMORY_DIVIDER, \
                        (float) swap_total / 1024, \
                        swap_percent);
            }
        }
    }

    update_line_len(g_mem);

    return NULL;
}

void get_infos(void *print()) {
    const void *routines[] = {
        get_cpu,
        get_mem,
        get_datetime,
        get_host,
        get_uptime,
        get_distro,
        get_pkgs,
        get_model,
        get_kernel,
        get_shell,
        get_user
    };

    const int THREADS_NUM = (int) sizeof(routines) / sizeof(routines[0]);

    pthread_t threads[THREADS_NUM];
    int i;

    for (i = 0; i < THREADS_NUM; i++) {
        pthread_create(&threads[i], NULL, routines[i], NULL);
    }

    for (i = 0; i < THREADS_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    print();

    pthread_exit(NULL);
}

void print_header(const int left_len,
                  const char *color_primary,
                  const char *color_secondary,
                  const char *color_table) {
    if (g_line_len < g_header_len - left_len) {
        g_line_len = g_header_len - left_len - 1;
    }

    printf("%s%s%s@%s%s%s", \
            color_primary, g_user, color_secondary, \
            color_primary, g_host, color_secondary);
    printf("%s", set_spacer(" ", g_line_len - g_header_len + left_len + 2));
    printf("%s%s\n", g_datetime, color_table);
}

void print_line(const int left_len,
                const char *line,
                const char *divider) {
    printf("%s", set_spacer(line, left_len));
    printf("%s", divider);
    printf("%s\n", set_spacer(line, g_line_len + 2));
}

void print_info(const char *label,
                const char *info) {
    static int color_code = 30;

    if (37 >= color_code) {
        printf(" \x1b[0;%dm%s", color_code, COLOR_SYMBOL);
        printf("\x1b[1;%dm%s", color_code, COLOR_SYMBOL);
        color_code++;
    } else {
        printf("%s", set_spacer(" ", COLOR_LEFT_LEN \
                    - strlen(LABEL_DISTRO) - 1));
    }

    printf("%s%s%s", COLOR_PRIMARY, label, COLOR_TABLE);
    printf("%s", COLOR_DIVIDER);
    printf("%s%s%s\n", COLOR_SECONDARY, info, COLOR_TABLE);
}

void *print_ascii() {
    print_header(ASCII_LEFT_LEN, "", "", "");

    print_line(ASCII_LEFT_LEN, ASCII_LINE, ASCII_DIVIDER_TOP);

    printf("%s%s%s\n", LABEL_DISTRO, ASCII_DIVIDER, g_distro);
    printf("%s%s%s\n", LABEL_MODEL, ASCII_DIVIDER, g_model);
    printf("%s%s%s\n", LABEL_KERNEL, ASCII_DIVIDER, g_kernel);
    printf("%s%s%s\n", LABEL_UPTIME, ASCII_DIVIDER, g_uptime);
    printf("%s%s%s\n", LABEL_PKGS, ASCII_DIVIDER, g_pkgs);
    printf("%s%s%s\n", LABEL_SHELL, ASCII_DIVIDER, g_shell);
    printf("%s%s%s\n", LABEL_CPU, ASCII_DIVIDER, g_cpu);
    printf("%s%s%s\n", LABEL_MEM, ASCII_DIVIDER, g_mem);

    print_line(ASCII_LEFT_LEN, ASCII_LINE, ASCII_DIVIDER_BOTTOM);

    return NULL;
}

void *print_color() {
    print_header(COLOR_LEFT_LEN, COLOR_PRIMARY, COLOR_SECONDARY, COLOR_TABLE);

    print_line(COLOR_LEFT_LEN, COLOR_LINE, COLOR_DIVIDER_TOP);

    print_info(LABEL_DISTRO, g_distro);
    print_info(LABEL_MODEL, g_model);
    print_info(LABEL_KERNEL, g_kernel);
    print_info(LABEL_UPTIME, g_uptime);
    print_info(LABEL_PKGS, g_pkgs);
    print_info(LABEL_SHELL, g_shell);
    print_info(LABEL_CPU, g_cpu);
    print_info(LABEL_MEM, g_mem);

    print_line(COLOR_LEFT_LEN, COLOR_LINE, COLOR_DIVIDER_BOTTOM);

    printf("\x1b[0m");
    return NULL;
}

void get_execution_times() {
    int line_len = 24;

    puts("execution times in seconds");

    print_line(line_len, ASCII_LINE, ASCII_DIVIDER_TOP);

    printf(" get_user    %s%f\n", ASCII_DIVIDER, get_execution_time(get_user));
    printf(" get_host    %s%f\n", ASCII_DIVIDER, get_execution_time(get_host));
    printf(" get_datetime%s%f\n", ASCII_DIVIDER, get_execution_time(get_datetime));

    printf(" get_distro  %s%f\n", ASCII_DIVIDER, get_execution_time(get_distro));
    printf(" get_model   %s%f\n", ASCII_DIVIDER, get_execution_time(get_model));
    printf(" get_kernel  %s%f\n", ASCII_DIVIDER, get_execution_time(get_kernel));
    printf(" get_uptime  %s%f\n", ASCII_DIVIDER, get_execution_time(get_uptime));
    printf(" get_pkgs    %s%f\n", ASCII_DIVIDER, get_execution_time(get_pkgs));
    printf(" get_shell   %s%f\n", ASCII_DIVIDER, get_execution_time(get_shell));
    printf(" get_cpu     %s%f\n", ASCII_DIVIDER, get_execution_time(get_cpu));
    printf(" get_mem     %s%f\n", ASCII_DIVIDER, get_execution_time(get_mem));

    update_line_len(NULL);
    print_line(line_len, ASCII_LINE, ASCII_DIVIDER_TOP);
}

void clear_files() {
    int line_len = 44;

    puts("cached files to clear (x=deleted, ?=not found)");

    print_line(line_len, ASCII_LINE, ASCII_DIVIDER_TOP);

    printf(" [%s] - %s\n", remove_file(CACHE_DISTRO_PATH), CACHE_DISTRO_PATH);
    printf(" [%s] - %s\n", remove_file(CACHE_MODEL_PATH), CACHE_MODEL_PATH);
    printf(" [%s] - %s\n", remove_file(CACHE_CPU_PATH), CACHE_CPU_PATH);
    printf(" [%s] - %s\n", remove_file(CACHE_PKGS_PATH), CACHE_PKGS_PATH);

    print_line(line_len, ASCII_LINE, ASCII_DIVIDER_TOP);
}

void print_usage() {
    puts("usage: cinfo [-a] [-c] [-i]");
}

int main(int argc, char *argv[]) {
    if (1 == argc) {
        get_infos(print_color);
    } else if (0 == strcmp(argv[1], "-a")) {
        get_infos(print_ascii);
    } else if (0 == strcmp(argv[1], "-c")) {
        clear_files();
    } else if (0 == strcmp(argv[1], "-i")) {
        get_execution_times();
        puts("cinfo-"VERSION);
    } else {
        print_usage();
    }

    return 0;
}
