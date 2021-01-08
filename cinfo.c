/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-08T20:39:33+0100
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "config.h"

int line_len,
    header_len;

char user[50],
     host[50],
     datetime[20],
     distro[50],
     model[65],
     kernel[50],
     uptime[13],
     pkgs[25],
     shell[25],
     cpu[50],
     ram[30];

const char* get_spacer(const char *character, int length) {
    static char spacer[65];
    int i = 1;

    strcpy(spacer, "");

    while (i <= length) {
        strcat(spacer, character);
        i++;
    }

    return(spacer);
}

void *get_user() {
    sprintf(user, "%s", getenv("USER"));

    header_len += strlen(user);

    return NULL;
}

void *get_host() {
    FILE *file;
    if ((file = fopen("/proc/sys/kernel/hostname", "r"))) {
        fscanf(file, "%s", host);
        fclose(file);
    }

    header_len += strlen(host);

    return NULL;
}

void *get_datetime() {
    time_t raw;
    struct tm * part;
    time(&raw);
    part = localtime(&raw);

    sprintf(datetime, "%02d.%02d.%d %02d:%02d:%02d", \
            part->tm_mday, \
            part->tm_mon + 1, \
            part->tm_year + 1900, \
            part->tm_hour, \
            part->tm_min, \
            part->tm_sec);

    header_len += strlen(datetime);

    return NULL;
}

void *get_distro() {
    FILE *file = popen("grep -m 1 '^PRETTY_NAME=' /etc/os-release \
            | cut -d '\"' -f2", "r");
    fscanf(file, "%[^\n]s", distro);
    pclose(file);

    if (line_len < strlen(distro)) {
        line_len = strlen(distro);
    }

    return NULL;
}

void *get_model() {
    char name[50] = "",
         version[15] = "";

    FILE *file;
    if ((file = fopen("/sys/devices/virtual/dmi/id/product_name", "r"))) {
        fscanf(file, "%[^\n]s", name);
        fclose(file);

        file = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
        fscanf(file, "%s", version);
        fclose(file);
    } else if ((file = fopen("/sys/firmware/devicetree/base/model", "r"))) {
        fscanf(file, "%[^\n]s", name);
        fclose(file);
    } else {
        strcpy(name, "not found");
    }

    sprintf(model, "%s %s", name, version);

    if (line_len < strlen(model)) {
        line_len = strlen(model);
    }

    return NULL;
}

void *get_kernel() {
    FILE *file;
    if ((file = fopen("/proc/sys/kernel/osrelease", "r"))) {
        fscanf(file, "%[^\n]s", kernel);
        fclose(file);

        if (line_len < strlen(kernel)) {
            line_len = strlen(kernel);
        }
    }

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

        if (0 == day) {
            sprintf(uptime, "%dh %dm", hour, min);
        } else if (0 == day && 0 == hour) {
            sprintf(uptime, "%dm", min);
        } else if (0 == day && 0 == hour && 0 == min) {
            sprintf(uptime, "0m");
        } else {
            sprintf(uptime, "%dd %dh %dm", day, hour, min);
        }
        if (line_len < strlen(uptime)) {
            line_len = strlen(uptime);
        }
    }

    return NULL;
}

void *get_pkgs() {
    int pkgs_count;

    FILE *file = popen(PKGS_CMD, "r");
    fscanf(file, "%d", &pkgs_count);
    pclose(file);

    sprintf(pkgs, "%d%s", pkgs_count, PKGS_DESC);

    if (line_len < strlen(pkgs)) {
        line_len = strlen(pkgs);
    }

    return NULL;
}

void *get_shell() {
    sprintf(shell, "%s", getenv("SHELL"));

    if (line_len < strlen(shell)) {
        line_len = strlen(shell);
    }

    return NULL;
}

void *get_cpu() {
    char model[45];
    float temp;

    FILE *file;
    file = popen("grep -m 1 '^model name.*:' /proc/cpuinfo \
            | sed 's/^model name.*: //'", "r");
    fscanf(file, "%[^\n]s", model);
    pclose(file);

    if ((file = fopen("/sys/class/thermal/thermal_zone0/temp", "r"))) {
        fscanf(file, "%f", &temp);
        fclose(file);
    }

    temp /= 1000;

    sprintf(cpu, "%s [%.0fC]", model, temp);

    if (line_len < strlen(cpu)) {
        line_len = strlen(cpu);
    }

    return NULL;
}

void *get_ram() {
    int total,
        available;

    float percent;

    FILE *file;
    file = popen("grep -m 1 '^MemTotal:' /proc/meminfo \
            | sed 's/^MemTotal://'", "r");
    fscanf(file, "%d", &total);

    file = popen("grep -m 1 '^MemAvailable:' /proc/meminfo \
            | sed 's/^MemAvailable://'", "r");
    fscanf(file, "%d", &available);
    pclose(file);

    available = (total - available) / 1024;
    total /= 1024;
    percent = (float) available / total * 100;

    sprintf(ram, "%dMiB / %dMiB [%.0f%%]", available, total, percent);

    if (line_len < strlen(ram)) {
        line_len = strlen(ram);
    }

    return NULL;
}

void get_infos(void *print()) {
    const void *routines[] = {
        get_user,
        get_host,
        get_datetime,
        get_distro,
        get_model,
        get_kernel,
        get_uptime,
        get_pkgs,
        get_shell,
        get_cpu,
        get_ram
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
    if (line_len < header_len - left_len) {
        line_len = header_len - left_len - 1;
    }

    printf("%s%s@%s%s", color_primary, user, host, color_secondary);
    printf("%s", get_spacer(" ", line_len - header_len + left_len + 2));
    printf("%s%s\n", datetime, color_table);
}

void print_line(const int left_len,
                const char *line,
                const char *divider) {
    printf("%s", get_spacer(line, left_len));
    printf("%s", divider);
    printf("%s\n", get_spacer(line, line_len + 2));
}

void print_info(const char *label,
                const char *info) {
    static int color_code = 30;

    if (37 >= color_code) {
        printf(" \033[0;%dm%s", color_code, COLOR_SYMBOL);
        printf("\033[1;%dm%s", color_code, COLOR_SYMBOL);
        color_code++;
    }

    printf("%s%s%s", COLOR_PRIMARY, label, COLOR_TABLE);
    printf("%s", COLOR_DIVIDER);
    printf("%s%s%s\n", COLOR_SECONDARY, info, COLOR_TABLE);
}

void *print_ascii() {
    print_header(ASCII_LEFT_LEN, "", "", "");

    print_line(ASCII_LEFT_LEN, ASCII_LINE, ASCII_DIVIDER_TOP);

    printf("%s%s%s\n", LABEL_DISTRO, ASCII_DIVIDER, distro);
    printf("%s%s%s\n", LABEL_MODEL, ASCII_DIVIDER, model);
    printf("%s%s%s\n", LABEL_KERNEL, ASCII_DIVIDER, kernel);
    printf("%s%s%s\n", LABEL_UPTIME, ASCII_DIVIDER, uptime);
    printf("%s%s%s\n", LABEL_PKGS, ASCII_DIVIDER, pkgs);
    printf("%s%s%s\n", LABEL_SHELL, ASCII_DIVIDER, shell);
    printf("%s%s%s\n", LABEL_CPU, ASCII_DIVIDER, cpu);
    printf("%s%s%s\n", LABEL_RAM, ASCII_DIVIDER, ram);

    print_line(ASCII_LEFT_LEN, ASCII_LINE, ASCII_DIVIDER_BOTTOM);

    return NULL;
}

void *print_color() {
    print_header(COLOR_LEFT_LEN, COLOR_PRIMARY, COLOR_SECONDARY, COLOR_TABLE);

    print_line(COLOR_LEFT_LEN, COLOR_LINE, COLOR_DIVIDER_TOP);

    print_info(LABEL_DISTRO, distro);
    print_info(LABEL_MODEL, model);
    print_info(LABEL_KERNEL, kernel);
    print_info(LABEL_UPTIME, uptime);
    print_info(LABEL_PKGS, pkgs);
    print_info(LABEL_SHELL, shell);
    print_info(LABEL_CPU, cpu);
    print_info(LABEL_RAM, ram);

    print_line(COLOR_LEFT_LEN, COLOR_LINE, COLOR_DIVIDER_BOTTOM);

    return NULL;
}

void print_usage() {
    printf("usage: cinfo [-a]\n");
}

int main(int argc, char *argv[]) {
    if (1 == argc) {
        get_infos(print_color);
    } else if (0 == strcmp(argv[1], "-a")) {
        get_infos(print_ascii);
    } else {
        print_usage();
    }

    return 0;
}
