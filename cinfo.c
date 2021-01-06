/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-06T17:13:29+0100
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "config.h"

int linelen,
    headerlen,
    startcolor = 30;

char user[50],
     host[50],
     zeit[20],
     distro[50],
     model[65],
     kernel[50],
     uptime[10],
     pkgs[15],
     shell[25],
     cpu[50],
     ram[25];

const char* getSpacer(const char *character, int length) {
    static char spacer[65];
    int i = 1;

    strcpy(spacer, "");

    while (i <= length) {
        strcat(spacer, character);
        i++;
    }

    return(spacer);
}

void *getUser() {
    sprintf(user, "%s", getenv("USER"));

    headerlen += strlen(user);

    return NULL;
}

void *getHost() {
    FILE *file;
    if ((file = fopen("/proc/sys/kernel/hostname", "r"))) {
        fscanf(file, "%s", host);
        fclose(file);
    }

    headerlen += strlen(host);

    return NULL;
}

void *getTime() {
    time_t raw;
    struct tm * part;
    time(&raw);
    part = localtime(&raw);

    sprintf(zeit, "%02d.%02d.%d %02d:%02d:%02d", \
            part->tm_mday, \
            part->tm_mon + 1, \
            part->tm_year + 1900, \
            part->tm_hour, \
            part->tm_min, \
            part->tm_sec);

    headerlen += strlen(zeit);

    return NULL;
}

void *getDistro() {
    FILE *file = popen("grep 'PRETTY_NAME=' /etc/os-release \
            | cut -d '\"' -f2", "r");
    fscanf(file, "%[^\n]s", distro);
    pclose(file);

    if (linelen < strlen(distro)) {
        linelen = strlen(distro);
    }

    return NULL;
}

void *getModel() {
    char modelname[50] = "",
         modelversion[15] = "";

    FILE *file;
    if ((file = fopen("/sys/devices/virtual/dmi/id/product_name", "r"))) {
        fscanf(file, "%[^\n]s", modelname);
        fclose(file);

        file = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
        fscanf(file, "%s", modelversion);
        fclose(file);
    } else if ((file = fopen("/sys/firmware/devicetree/base/model", "r"))) {
        fscanf(file, "%[^\n]s", modelname);
        fclose(file);
    } else {
        strcpy(modelname, "file not found");
    }

    sprintf(model, "%s %s", modelname, modelversion);

    if (linelen < strlen(model)) {
        linelen = strlen(model);
    }

    return NULL;
}

void *getKernel() {
    FILE *file;
    if ((file = fopen("/proc/sys/kernel/osrelease", "r"))) {
        fscanf(file, "%[^\n]s", kernel);
        fclose(file);

        if (linelen < strlen(kernel)) {
            linelen = strlen(kernel);
        }
    }

    return NULL;
}

void *getUptime() {
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

        if (day == 0) {
            sprintf(uptime, "%dh %dm", hour, min);
        } else if (day == 0 && hour == 0) {
            sprintf(uptime, "%dm", min);
        } else if (day == 0 && hour == 0 && min == 0) {
            sprintf(uptime, "0m");
        } else {
            sprintf(uptime, "%dd %dh %dm", day, hour, min);
        }

        if (linelen < strlen(uptime)) {
            linelen = strlen(uptime);
        }
    }

    return NULL;
}

void *getPackages() {
    int pkgcount;

    FILE *file = popen(PKG_CMD, "r");
    fscanf(file, "%d", &pkgcount);
    pclose(file);

    sprintf(pkgs, "%d%s", pkgcount, PKG_DESC);

    if (linelen < strlen(pkgs)) {
        linelen = strlen(pkgs);
    }

    return NULL;
}

void *getShell() {
    sprintf(shell, "%s", getenv("SHELL"));

    if (linelen < strlen(shell)) {
        linelen = strlen(shell);
    }

    return NULL;
}

void *getCPU() {
    char cpumodel[45];
    float cputemp;

    FILE *file;
    file = popen("grep -m 1 'model name	:' /proc/cpuinfo \
            | sed 's/model name	: //'", "r");
    fscanf(file, "%[^\n]s", cpumodel);
    pclose(file);

    if ((file = fopen("/sys/class/thermal/thermal_zone0/temp", "r"))) {
        fscanf(file, "%f", &cputemp);
        fclose(file);
    }

    cputemp /= 1000;

    sprintf(cpu, "%s [%.0fC]", cpumodel, cputemp);

    if (linelen < strlen(cpu)) {
        linelen = strlen(cpu);
    }

    return NULL;
}

void *getRAM() {
    int memtotal,
        memavailable;

    float mempercent;

    FILE *file;
    file = popen("grep 'MemTotal:' /proc/meminfo \
            | sed 's/MemTotal://'", "r");
    fscanf(file, "%d", &memtotal);

    file = popen("grep 'MemAvailable:' /proc/meminfo \
            | sed 's/MemAvailable://'", "r");
    fscanf(file, "%d", &memavailable);
    pclose(file);

    memavailable = (memtotal - memavailable) / 1024;
    memtotal /= 1024;
    mempercent = (float)memavailable / memtotal * 100;

    sprintf(ram, "%dMiB / %dMiB [%.0f%%]", memavailable, memtotal, mempercent);

    if (linelen < strlen(ram)) {
        linelen = strlen(ram);
    }

    return NULL;
}

void printUsage() {
    printf("usage: cinfo [-a]\n");
}

void printLine(const int lineleftlen, const char *line, const char *linedivider) {
    printf("%s", getSpacer(line, lineleftlen));
    printf("%s", linedivider);
    printf("%s\n", getSpacer(line, linelen + 2));
}

void printInfo(const char *label, const char *result) {
    if (startcolor >= 30 && startcolor <= 37) {
        printf(" \033[0;%dm%s", startcolor, COLOR_SYMBOL);
        printf("\033[1;%dm%s", startcolor, COLOR_SYMBOL);
        startcolor++;
    } else {
        printf("     ");
    }
    printf("%s%s%s%s%s%s%s\n", COLOR_PRIMARY, label, COLOR_TABLE, COLOR_DIVIDER, COLOR_SECONDARY, result, COLOR_TABLE);
}

void printAscii() {
    if (linelen < headerlen - ASCII_LINELEFTLEN) {
        linelen = headerlen - ASCII_LINELEFTLEN - 1;
    }

    printf("%s@%s", user, host);
    printf("%s", getSpacer(" ", linelen - headerlen + ASCII_LINELEFTLEN + 2));
    printf("%s\n",zeit);

    printLine(ASCII_LINELEFTLEN, ASCII_LINE, ASCII_LINEDIVIDER);

    printf("%s%s%s\n", DISTRO_TXT, ASCII_DIVIDER, distro);
    printf("%s%s%s\n", MODEL_TXT, ASCII_DIVIDER, model);
    printf("%s%s%s\n", KERNEL_TXT, ASCII_DIVIDER, kernel);
    printf("%s%s%s\n", UPTIME_TXT, ASCII_DIVIDER, uptime);
    printf("%s%s%s\n", PKGS_TXT, ASCII_DIVIDER, pkgs);
    printf("%s%s%s\n", SHELL_TXT, ASCII_DIVIDER, shell);
    printf("%s%s%s\n", CPU_TXT, ASCII_DIVIDER, cpu);
    printf("%s%s%s\n", RAM_TXT, ASCII_DIVIDER, ram);

    printLine(ASCII_LINELEFTLEN, ASCII_LINE, ASCII_LINEDIVIDER);
}

void printColor() {
    if (linelen < headerlen - COLOR_LINELEFTLEN) {
        linelen = headerlen - COLOR_LINELEFTLEN - 1;
    }

    printf("%s%s@%s%s", COLOR_PRIMARY, user, host, COLOR_SECONDARY);
    printf("%s", getSpacer(" ", linelen - headerlen + COLOR_LINELEFTLEN + 2));
    printf("%s%s\n", zeit, COLOR_TABLE);

    printLine(COLOR_LINELEFTLEN, COLOR_LINE, COLOR_LINEDIVIDERTOP);

    printInfo(DISTRO_TXT, distro);
    printInfo(MODEL_TXT, model);
    printInfo(KERNEL_TXT, kernel);
    printInfo(UPTIME_TXT, uptime);
    printInfo(PKGS_TXT, pkgs);
    printInfo(SHELL_TXT, shell);
    printInfo(CPU_TXT, cpu);
    printInfo(RAM_TXT, ram);

    printLine(COLOR_LINELEFTLEN, COLOR_LINE, COLOR_LINEDIVIDERBOTTOM);
}

int main(int argc, char *argv[]) {
    const int THREADS_NUM = 11;
    pthread_t threads[THREADS_NUM];
    int i;

    pthread_create(&threads[0], NULL, getUser, NULL);
    pthread_create(&threads[1], NULL, getHost, NULL);
    pthread_create(&threads[2], NULL, getTime, NULL);
    pthread_create(&threads[3], NULL, getDistro, NULL);
    pthread_create(&threads[4], NULL, getModel, NULL);
    pthread_create(&threads[5], NULL, getKernel, NULL);
    pthread_create(&threads[6], NULL, getUptime, NULL);
    pthread_create(&threads[7], NULL, getPackages, NULL);
    pthread_create(&threads[8], NULL, getShell, NULL);
    pthread_create(&threads[9], NULL, getCPU, NULL);
    pthread_create(&threads[10], NULL, getRAM, NULL);

    for (i = 0; i < THREADS_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    if (argc == 1) {
        printColor();
    } else if (strcmp(argv[1], "-a") == 0) {
        printAscii();
    } else {
        printUsage();
    }

    pthread_exit(NULL);

    return 0;
}
