/*
 * path:   /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2024-05-15T18:23:48+0200
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

#include "cinfo.h"
#include "util.h"
#include "config.h"

int g_header_len,
    g_line_len;

char g_user[50],
     g_host[50],
     g_datetime[65],
     g_distro[65],
     g_kernel[65],
     g_pkgs[65],
     g_model[65],
     g_cpu[65],
     g_mem[65],
     g_uptime[65],
     g_shell[65];

void *get_user() {
    sprintf(g_user, "%49s", getenv("USER"));
    remove_char(g_user, " ");

    g_header_len = update_header_len(g_user, g_header_len);

    return NULL;
}

void *get_host() {
    FILE *file;

    if ((file = fopen("/proc/sys/kernel/hostname", "r"))) {
        if (fscanf(file, "%49s", g_host)) {
            fclose(file);
        }

        g_header_len = update_header_len(g_host, g_header_len);
    }

    return NULL;
}

void *get_datetime() {
    time_t raw;
    struct tm * part;
    time(&raw);
    part = localtime(&raw);

    sprintf(g_datetime, "%02d.%02d.%d %02d:%02d:%02d",
            part->tm_mday,
            part->tm_mon + 1,
            part->tm_year + 1900,
            part->tm_hour,
            part->tm_min,
            part->tm_sec);

    g_header_len = update_header_len(g_datetime, g_header_len);

    return NULL;
}

void *get_distro() {
    char filter[33],
         value[63];

    FILE *file;

    if ((file = fopen(CACHE_DISTRO_PATH, "r"))) {
        if (fscanf(file, "%64[^\n]s", g_distro)) {
            fclose(file);
        }
    } else if ((file = fopen("/etc/os-release", "r"))) {
        while (2 == fscanf(file, " %32[^=]=%62[^\n]", filter, value)) {
            if (0 == strcmp(filter, "PRETTY_NAME")) {
                file = fopen(CACHE_DISTRO_PATH, "w");
                remove_char(value, "\"");
                fprintf(file, "%s", value);
                strcpy(g_distro, value);
                break;
            }
        }
        fclose(file);
    }

    g_line_len = update_line_len(g_distro, g_line_len);

    return NULL;
}

void *get_kernel() {
    FILE *file;

    if ((file = fopen("/proc/sys/kernel/osrelease", "r"))) {
        if (fscanf(file, "%64[^\n]s", g_kernel)) {
            fclose(file);
        }
    }

    g_line_len = update_line_len(g_kernel, g_line_len);

    return NULL;
}

void *get_pkgs() {
    int max_len = sizeof(g_pkgs),
        pkgs_count = 0;

    FILE *file;

    if ((file = fopen(CACHE_PKGS_PATH, "r"))) {
        if (fscanf(file, "%d", &pkgs_count)) {
            fclose(file);
        }
    } else {
        if ((file = popen(PKGS_CMD, "r"))) {
            if (fscanf(file, "%d", &pkgs_count)) {
                fclose(file);
            }

            file = fopen(CACHE_PKGS_PATH, "w");
            fprintf(file, "%d", pkgs_count);
            fclose(file);
        }
    }

    snprintf(g_pkgs, max_len, "%d%s", pkgs_count, PKGS_DESC);

    g_line_len = update_line_len(g_pkgs, g_line_len);

    return NULL;
}

void *get_model() {
    int max_len = sizeof(g_model);

    char name[65] = "",
         version[65] = "",
         model[130] = "";

    FILE *file;

    if ((file = fopen(CACHE_MODEL_PATH, "r"))) {
        if (fscanf(file, "%64[^\n]s", g_model)) {
            fclose(file);
        }
    } else if ((file = fopen("/sys/devices/virtual/dmi/id/product_name", "r"))) {
        if (fscanf(file, "%64[^\n]s", name)) {
            fclose(file);
        }

        file = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
        if (fscanf(file, "%64[^\n]s", version)) {
            fclose(file);
        }
    } else if ((file = fopen("/sys/firmware/devicetree/base/model", "r"))) {
        if (fscanf(file, "%64[^\n]s", name)) {
            fclose(file);
        }
    } else {
        strcpy(name, "not found");
    }

    if ((file != fopen(CACHE_MODEL_PATH, "r"))) {
        file = fopen(CACHE_MODEL_PATH, "w");
        snprintf(model, max_len,"%s %s", name, version);
        fprintf(file, "%s", model);
        snprintf(g_model, max_len, "%s", model);
        fclose(file);
    }

    g_line_len = update_line_len(g_model, g_line_len);

    return NULL;
}

void *get_cpu() {
    int max_len = sizeof(g_cpu),
        i;

    float temp;

    char filter[20],
         value[58],
         temp_path[65],
         temp_name_path[59],
         temp_name[16],
         cpu[65];

    FILE *file;
    DIR *dir;

    struct dirent *entry;

    if ((file = fopen(CACHE_CPU_PATH, "r"))) {
        if (fscanf(file, "%64[^\n]s", cpu)) {
            fclose(file);
        }
    } else if ((file = fopen("/proc/cpuinfo", "r"))) {
        while (2 == fscanf(file, " %19[^:]: %57[^\n]", filter, value)) {
            if (0 == strcmp(filter, "model name	")) {
                file = fopen(CACHE_CPU_PATH, "w");
                fprintf(file, "%s", value);
                strcpy(cpu, value);
                break;
            }
        }
        fclose(file);
    }

    if ((file != fopen(CACHE_CPU_TEMP_PATH, "r"))) {
        if ((dir = opendir(CPU_TEMP_PATH)) == NULL) return 0;

        while ((entry = readdir(dir)) != NULL) {
            if (0 == strcmp(entry->d_name,".") ||
                0 == strcmp(entry->d_name,"..")) continue;
            else {
                sprintf(temp_path,
                        "%s/%.47s", CPU_TEMP_PATH, entry->d_name);
                sprintf(temp_name_path,
                        "%.52s/name", temp_path);

                if ((file = fopen(temp_name_path, "r"))) {
                    if (fscanf(file, "%15s", temp_name)) {
                        fclose(file);
                    }

                    size_t n = sizeof(CPU_TEMP_INPUT) / sizeof(CPU_TEMP_INPUT[0]);

                    for (i = 0; i < n; i++) {
                        if (0 == strcmp(CPU_TEMP_INPUT[i], temp_name)) {
                            file = fopen(CACHE_CPU_TEMP_PATH, "w");
                            fprintf(file,
                                    "%s/%s", temp_path, CPU_TEMP_INPUT_FILE);
                            fclose(file);
                            break;
                        }
                    }

                    if ((file != fopen(CACHE_CPU_TEMP_PATH, "r"))) {
                        file = fopen(CACHE_CPU_TEMP_PATH, "w");
                        fclose(file);
                    }
                }
            }
        }
    }

    if ((file = fopen(CACHE_CPU_TEMP_PATH, "r"))) {
        if (fscanf(file, "%64[^\n]s", temp_path)) {
            fclose(file);
        }

        if ((file = fopen(temp_path, "r"))) {
            if (fscanf(file, "%f", &temp)) {
                temp /= 1000;
                snprintf(g_cpu, max_len, "%.56s [%.1f%s]", cpu, temp, CPU_TEMP);
                fclose(file);
            }
        } else {
            snprintf(g_cpu, max_len, "%s", cpu);
        }
    }

    g_line_len = update_line_len(g_cpu, g_line_len);

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

    char filter[20];

    FILE *file;

    if ((file = fopen("/proc/meminfo", "r"))) {
        while (2 == fscanf(file, "%15s  %d %*s", filter, &value)) {
            if (0 == strcmp(filter, "MemTotal:")) {
                mem_total = value;
            } else if (0 == strcmp(filter, "MemFree:")) {
                mem_free = value;
            } else if (0 == strcmp(filter, "MemAvailable:")) {
                mem_available = value;
            } else if (0 == strcmp(filter, "Buffers:")) {
                mem_buffers = value;
            } else if (0 == strcmp(filter, "Cached:")) {
                mem_cached = value;
            } else if (0 == strcmp(filter, "SwapTotal:")) {
                swap_total = value;
            } else if (0 == strcmp(filter, "SwapFree:")) {
                swap_free = value;
            } else if (0 == strcmp(filter, "Shmem:")) {
                mem_shared = value;
            } else if (0 == strcmp(filter, "SReclaimable:")) {
                mem_reclaimable = value;
                break;
            }
        }
        fclose(file);

        if (2 == MEMORY_METHOD) {
            mem_available = (mem_total - mem_free - mem_buffers - mem_cached
                    - mem_reclaimable) / 1024;
        } else if (1 == MEMORY_METHOD) {
            mem_available = (mem_total + mem_shared - mem_free - mem_buffers
                    - mem_cached - mem_reclaimable) / 1024;
        } else {
            mem_available = (mem_total - mem_available) / 1024;
        }

        mem_total /= 1024;
        mem_percent = (float)mem_available / mem_total * 100;

        if (0 < swap_total) {
            swap_available = (swap_total - swap_free) / 1024;
            swap_total /= 1024;
            swap_percent = (float)swap_available / swap_total * 100;
        }

        if (0 == strcmp(MEMORY_UNIT, "MiB")
        || (0 == strcmp(MEMORY_UNIT, "auto") && 1024 > mem_total)) {
            if (0 < swap_total) {
                sprintf(g_mem, "%d%s%d MiB [%.1f%%]%s%d%s%d MiB [%.1f%%]",
                        mem_available,
                        MEMORY_DIVIDER,
                        mem_total,
                        mem_percent,
                        INFO_DIVIDER,
                        swap_available,
                        MEMORY_DIVIDER,
                        swap_total,
                        swap_percent);
            } else {
                sprintf(g_mem, "%d%s%d MiB [%.1f%%]",
                        mem_available,
                        MEMORY_DIVIDER,
                        mem_total,
                        mem_percent);
            }
        }

        if (0 == strcmp(MEMORY_UNIT, "GiB")
        || (0 == strcmp(MEMORY_UNIT, "auto") && 1024 <= mem_total)) {
            if (0 < swap_total) {
                sprintf(g_mem, "%.2f%s%.2f GiB [%.1f%%]%s%.2f%s%.2f GiB [%.1f%%]",
                        (float)mem_available / 1024,
                        MEMORY_DIVIDER,
                        (float)mem_total / 1024,
                        mem_percent,
                        INFO_DIVIDER,
                        (float)swap_available / 1024,
                        MEMORY_DIVIDER,
                        (float)swap_total / 1024,
                        swap_percent);
            } else {
                sprintf(g_mem, "%.2f%s%.2f GiB [%.1f%%]",
                        (float)mem_available / 1024,
                        MEMORY_DIVIDER,
                        (float)mem_total / 1024,
                        mem_percent);
            }
        }
    }

    g_line_len = update_line_len(g_mem, g_line_len);

    return NULL;
}

void *get_uptime() {
    int day,
        hour,
        min,
        sec;

    char uptime_day[13] = {0},
         uptime_hour[11] = {0},
         uptime_min[11] = {0},
         uptime_sec[12] = {0},
         loadavg[35],
         *loadavg_split[10];

    FILE *file;

    if ((file = fopen("/proc/uptime", "r"))) {
        if (fscanf(file, "%d", &sec)) {
            fclose(file);
        }

        day = sec / 60 / 60 / 24;
        hour = sec / 60 / 60 % 24;
        min = sec / 60 % 60;
        sec = sec % 60;

        if (0 == day && 0 == hour && 0 == min) {
            sprintf(uptime_sec, "%d second%s",
                    sec,
                    1 == sec ? "" : "s");
        } else {
            if (0 < day) {
                sprintf(uptime_day, "%d day%s%s",
                        day,
                        1 == day ? "" : "s",
                        0 < hour || 0 < min ? ", " : "");
            }
            if (0 < hour) {
                sprintf(uptime_hour, "%d hour%s%s",
                        hour,
                        1 == hour ? "" : "s",
                        0 < min ? ", " : "");
            }
            if (0 < min) {
                sprintf(uptime_min, "%d minute%s",
                        min,
                        1 == min ? "" : "s");
            }
        }
    }

    if ((file = fopen("/proc/loadavg", "r"))) {
        if (fscanf(file, "%24[^\n]s", loadavg)) {
            split_string(loadavg, loadavg_split, " ");
            fclose(file);
        }

        sprintf(g_uptime, "%s%s%s%s%s%s, %s, %s",
                uptime_day,
                uptime_hour,
                uptime_min,
                uptime_sec,
                INFO_DIVIDER,
                loadavg_split[0],
                loadavg_split[1],
                loadavg_split[2]);
    }

    g_line_len = update_line_len(g_uptime, g_line_len);

    return NULL;
}

void *get_shell() {
    int max_len = sizeof(g_shell)-1;

    char shell[20];

    ssize_t len = readlink(SHELL_PATH, shell, sizeof(shell)-1);

    if (-1 == len) {
        sprintf(shell, "LINK ERR");
    }

    if (0 != getenv("TERM")) {
        snprintf(g_shell, max_len, "%s [%.10s]%s%s [%s]",
            SHELL_PATH, shell,
            INFO_DIVIDER, getenv("SHELL"),
            getenv("TERM"));
    } else {
        snprintf(g_shell, max_len, "%s [%.10s]%s%s",
            SHELL_PATH, shell,
            INFO_DIVIDER, getenv("SHELL"));
    }

    g_line_len = update_line_len(g_shell, g_line_len);

    return NULL;
}

void get_infos(void *print()) {
    const void *routines[] = {
        get_cpu,
        get_mem,
        get_user,
        get_host,
        get_datetime,
        get_uptime,
        get_distro,
        get_pkgs,
        get_model,
        get_kernel,
        get_shell
    };

    const int THREADS_NUM = (int)sizeof(routines) / sizeof(routines[0]);

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

    printf("%s%s%s@%s%s%s",
            color_primary, g_user, color_secondary,
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
        printf(" \033[0;%dm%s", color_code, COLOR_SYMBOL);
        printf("\033[1;%dm%s", color_code, COLOR_SYMBOL);
        color_code++;
    } else {
        printf("%s", set_spacer(" ", COLOR_LEFT_LEN
                    - strlen(LABEL_DISTRO) - 1));
    }

    printf("%s%s%s", COLOR_SECONDARY, label, COLOR_TABLE);
    printf("%s", COLOR_DIVIDER);
    printf("%s%s%s\n", COLOR_PRIMARY, info, COLOR_TABLE);
}

void *print_ascii() {
    print_header(ASCII_LEFT_LEN, "", "", "");

    print_line(ASCII_LEFT_LEN, ASCII_LINE, ASCII_DIVIDER_TOP);

    printf("%s%s%s\n", LABEL_DISTRO, ASCII_DIVIDER, g_distro);
    printf("%s%s%s\n", LABEL_KERNEL, ASCII_DIVIDER, g_kernel);
    printf("%s%s%s\n", LABEL_PKGS, ASCII_DIVIDER, g_pkgs);
    printf("%s%s%s\n", LABEL_MODEL, ASCII_DIVIDER, g_model);
    printf("%s%s%s\n", LABEL_CPU, ASCII_DIVIDER, g_cpu);
    printf("%s%s%s\n", LABEL_MEM, ASCII_DIVIDER, g_mem);
    printf("%s%s%s\n", LABEL_UPTIME, ASCII_DIVIDER, g_uptime);
    printf("%s%s%s\n", LABEL_SHELL, ASCII_DIVIDER, g_shell);

    print_line(ASCII_LEFT_LEN, ASCII_LINE, ASCII_DIVIDER_BOTTOM);

    return NULL;
}

void *print_color() {
    print_header(COLOR_LEFT_LEN, COLOR_HEADER_PRIMARY,
                 COLOR_HEADER_SECONDARY, COLOR_TABLE);

    print_line(COLOR_LEFT_LEN, COLOR_LINE, COLOR_DIVIDER_TOP);

    print_info(LABEL_DISTRO, g_distro);
    print_info(LABEL_KERNEL, g_kernel);
    print_info(LABEL_PKGS, g_pkgs);
    print_info(LABEL_MODEL, g_model);
    print_info(LABEL_CPU, g_cpu);
    print_info(LABEL_MEM, g_mem);
    print_info(LABEL_UPTIME, g_uptime);
    print_info(LABEL_SHELL, g_shell);

    print_line(COLOR_LEFT_LEN, COLOR_LINE, COLOR_DIVIDER_BOTTOM);

    printf("\033[0m");
    return NULL;
}

void get_execution_times() {
    int line_len = 22;

    puts("execution time in seconds");

    print_line(line_len, ASCII_LINE, ASCII_DIVIDER_TOP);

    printf(" get_user    %s%f\n", ASCII_DIVIDER, get_execution_time(get_user));
    printf(" get_host    %s%f\n", ASCII_DIVIDER, get_execution_time(get_host));
    printf(" get_datetime%s%f\n", ASCII_DIVIDER, get_execution_time(get_datetime));

    printf(" get_distro  %s%f\n", ASCII_DIVIDER, get_execution_time(get_distro));
    printf(" get_kernel  %s%f\n", ASCII_DIVIDER, get_execution_time(get_kernel));
    printf(" get_pkgs    %s%f\n", ASCII_DIVIDER, get_execution_time(get_pkgs));
    printf(" get_model   %s%f\n", ASCII_DIVIDER, get_execution_time(get_model));
    printf(" get_cpu     %s%f\n", ASCII_DIVIDER, get_execution_time(get_cpu));
    printf(" get_mem     %s%f\n", ASCII_DIVIDER, get_execution_time(get_mem));
    printf(" get_uptime  %s%f\n", ASCII_DIVIDER, get_execution_time(get_uptime));
    printf(" get_shell   %s%f\n", ASCII_DIVIDER, get_execution_time(get_shell));

    g_line_len = update_line_len(NULL, 0);
    print_line(line_len, ASCII_LINE, ASCII_DIVIDER_TOP);
}

void delete_cache() {
    int line_len = 41;

    puts("delete cached files (x=deleted, ?=not found)");

    print_line(line_len, ASCII_LINE, ASCII_DIVIDER_TOP);

    printf(" [%s]%s%s\n", remove_file(CACHE_DISTRO_PATH),
            ASCII_DIVIDER, CACHE_DISTRO_PATH);
    printf(" [%s]%s%s\n", remove_file(CACHE_PKGS_PATH),
            ASCII_DIVIDER, CACHE_PKGS_PATH);
    printf(" [%s]%s%s\n", remove_file(CACHE_MODEL_PATH),
            ASCII_DIVIDER, CACHE_MODEL_PATH);
    printf(" [%s]%s%s\n", remove_file(CACHE_CPU_PATH),
            ASCII_DIVIDER, CACHE_CPU_PATH);
    printf(" [%s]%s%s\n", remove_file(CACHE_CPU_TEMP_PATH),
            ASCII_DIVIDER, CACHE_CPU_TEMP_PATH);

    print_line(line_len, ASCII_LINE, ASCII_DIVIDER_TOP);
}

void print_usage() {
    puts("usage: cinfo [-a] [-c] [-d] [-i]");
}

int main(int argc, char *argv[]) {
    if (1 == argc) {
        if (NULL == getenv("NO_COLOR")) {
            get_infos(print_color);
        } else {
            get_infos(print_ascii);
        };
    } else if (0 == strcmp(argv[1], "-a")) {
        get_infos(print_ascii);
    } else if (0 == strcmp(argv[1], "-c")) {
        get_infos(print_color);
    } else if (0 == strcmp(argv[1], "-d")) {
        delete_cache();
    } else if (0 == strcmp(argv[1], "-i")) {
        get_execution_times();
        puts("cinfo-"VERSION);
    } else {
        print_usage();
    }

    return 0;
}
