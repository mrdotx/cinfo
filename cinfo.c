/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-10T10:18:03+0100
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "config.h"

int g_line_len,
    g_header_len;

char g_user[50],
     g_host[50],
     g_datetime[20],
     g_distro[50],
     g_model[65],
     g_kernel[50],
     g_uptime[13],
     g_pkgs[25],
     g_shell[25],
     g_cpu[50],
     g_ram[30];

const char* get_spacer(const char *character, int length) {
    static char spacer[65];
    int i;

    spacer[0] = '\0';

    for (i = 0; i < length; i++) {
        strcat(spacer, character);
    }

    return spacer;
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
    }

    g_header_len += strlen(g_host);

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
    FILE *file = popen("grep -m 1 '^PRETTY_NAME=' /etc/os-release \
            | cut -d '\"' -f2", "r");
    fscanf(file, "%[^\n]s", g_distro);
    pclose(file);

    if (g_line_len < strlen(g_distro)) {
        g_line_len = strlen(g_distro);
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

    sprintf(g_model, "%s %s", name, version);

    if (g_line_len < strlen(g_model)) {
        g_line_len = strlen(g_model);
    }

    return NULL;
}

void *get_kernel() {
    FILE *file;
    if ((file = fopen("/proc/sys/kernel/osrelease", "r"))) {
        fscanf(file, "%[^\n]s", g_kernel);
        fclose(file);

        if (g_line_len < strlen(g_kernel)) {
            g_line_len = strlen(g_kernel);
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

        if (0 == day && 0 == hour && 0 == min) {
            sprintf(g_uptime, "0m");
        } else if (0 == day && 0 == hour) {
            sprintf(g_uptime, "%dm", min);
        } else if (0 == day) {
            sprintf(g_uptime, "%dh %dm", hour, min);
        } else {
            sprintf(g_uptime, "%dd %dh %dm", day, hour, min);
        }
        if (g_line_len < strlen(g_uptime)) {
            g_line_len = strlen(g_uptime);
        }
    }

    return NULL;
}

void *get_pkgs() {
    int pkgs_count;

    FILE *file = popen(PKGS_CMD, "r");
    fscanf(file, "%d", &pkgs_count);
    pclose(file);

    sprintf(g_pkgs, "%d%s", pkgs_count, PKGS_DESC);

    if (g_line_len < strlen(g_pkgs)) {
        g_line_len = strlen(g_pkgs);
    }

    return NULL;
}

void *get_shell() {
    sprintf(g_shell, "%s", getenv("SHELL"));

    if (g_line_len < strlen(g_shell)) {
        g_line_len = strlen(g_shell);
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

    sprintf(g_cpu, "%s [%.0fC]", model, temp);

    if (g_line_len < strlen(g_cpu)) {
        g_line_len = strlen(g_cpu);
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

    sprintf(g_ram, "%dMiB / %dMiB [%.0f%%]", available, total, percent);

    if (g_line_len < strlen(g_ram)) {
        g_line_len = strlen(g_ram);
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
    if (g_line_len < g_header_len - left_len) {
        g_line_len = g_header_len - left_len - 1;
    }

    printf("%s%s@%s%s", color_primary, g_user, g_host, color_secondary);
    printf("%s", get_spacer(" ", g_line_len - g_header_len + left_len + 2));
    printf("%s%s\n", g_datetime, color_table);
}

void print_line(const int left_len,
                const char *line,
                const char *divider) {
    printf("%s", get_spacer(line, left_len));
    printf("%s", divider);
    printf("%s\n", get_spacer(line, g_line_len + 2));
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

    printf("%s%s%s\n", LABEL_DISTRO, ASCII_DIVIDER, g_distro);
    printf("%s%s%s\n", LABEL_MODEL, ASCII_DIVIDER, g_model);
    printf("%s%s%s\n", LABEL_KERNEL, ASCII_DIVIDER, g_kernel);
    printf("%s%s%s\n", LABEL_UPTIME, ASCII_DIVIDER, g_uptime);
    printf("%s%s%s\n", LABEL_PKGS, ASCII_DIVIDER, g_pkgs);
    printf("%s%s%s\n", LABEL_SHELL, ASCII_DIVIDER, g_shell);
    printf("%s%s%s\n", LABEL_CPU, ASCII_DIVIDER, g_cpu);
    printf("%s%s%s\n", LABEL_RAM, ASCII_DIVIDER, g_ram);

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
    print_info(LABEL_RAM, g_ram);

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
