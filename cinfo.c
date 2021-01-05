/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-05T12:39:19+0100
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "config.h"

int linelen,
    headerlen;

char user[50],
     host[50],
     zeit[20],
     distro[50],
     model[75],
     kernel[50],
     uptime[10],
     pkgs[15],
     shell[25],
     cpu[50],
     ram[20];

const char *RESET = "\033[0m",
           *BLACK = "\033[0;30m",
           *RED = "\033[0;31m",
           *GREEN = "\033[0;32m",
           *YELLOW = "\033[0;33m",
           *BLUE = "\033[0;34m",
           *MAGENTA = "\033[0;35m",
           *CYAN = "\033[0;36m",
           *WHITE = "\033[0;37m",
           *BBLACK = "\033[1;30m",
           *BRED = "\033[1;31m",
           *BGREEN = "\033[1;32m",
           *BYELLOW = "\033[1;33m",
           *BBLUE = "\033[1;34m",
           *BMAGENTA = "\033[1;35m",
           *BCYAN = "\033[1;36m",
           *BWHITE = "\033[1;37m";

const char* getSpacer(const char *character,
                      int length) {
    static char spacer[] = "";
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

    return NULL;
}

void *getHost() {
    FILE *file;
    if ((file = fopen("/proc/sys/kernel/hostname", "r"))) {
        fscanf(file, "%s", host);
        fclose(file);
    }

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
         modelversion[25] = "";

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

    FILE *file = popen(PKGCMD, "r");
    fscanf(file, "%d", &pkgcount);
    pclose(file);

    sprintf(pkgs, "%d%s", pkgcount, PKGDESC);

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

void printAscii() {
    headerlen = strlen(user) + strlen(host) + strlen(zeit);
    if (linelen < headerlen - ASCIILINELEFTLEN - 1)
    {
        linelen = headerlen - ASCIILINELEFTLEN - 2;
    }

    printf("%s", user);
    printf("%s", HEADERBINDER);
    printf("%s", host);
    printf("%s", getSpacer(HEADERSPACER, linelen - headerlen + ASCIILINELEFTLEN + 2));
    printf("%s\n",zeit);
    printf("%s", getSpacer(ASCIILINE, ASCIILINELEFTLEN));
    printf("%s", ASCIILINEDIVIDER);
    printf("%s\n", getSpacer(ASCIILINE, linelen + 2));

    printf("%s%s%s\n", DISTROTXT, ASCIIDIVIDER, distro);
    printf("%s%s%s\n", MODELTXT, ASCIIDIVIDER, model);
    printf("%s%s%s\n", KERNELTXT, ASCIIDIVIDER, kernel);
    printf("%s%s%s\n", UPTIMETXT, ASCIIDIVIDER, uptime);
    printf("%s%s%s\n", PKGSTXT, ASCIIDIVIDER, pkgs);
    printf("%s%s%s\n", SHELLTXT, ASCIIDIVIDER, shell);
    printf("%s%s%s\n", CPUTXT, ASCIIDIVIDER, cpu);
    printf("%s%s%s\n", RAMTXT, ASCIIDIVIDER, ram);

    printf("%s", getSpacer(ASCIILINE, ASCIILINELEFTLEN));
    printf("%s", ASCIILINEDIVIDER);
    printf("%s\n", getSpacer(ASCIILINE, linelen + 2));
}

void printColor() {
    headerlen = strlen(user) + strlen(host) + strlen(zeit);
    if (linelen < headerlen - COLORLINELEFTLEN - 1)
    {
        linelen = headerlen - COLORLINELEFTLEN - 2;
    }

    printf("%s%s%s", BWHITE, user, RESET);
    printf("%s", HEADERBINDER);
    printf("%s%s%s", BWHITE, host, RESET);
    printf("%s", getSpacer(HEADERSPACER, linelen - headerlen + COLORLINELEFTLEN + 2));
    printf("%s\n", zeit);
    printf("%s", getSpacer(COLORLINE, COLORLINELEFTLEN));
    printf("%s", COLORLINEDIVIDERTOP);
    printf("%s\n", getSpacer(COLORLINE, linelen + 2));

    printf(" %s%s%s%s", BLACK, COLORSYMBOL, BBLACK, COLORSYMBOL);
    printf("%s%s%s%s%s\n", BWHITE, DISTROTXT, RESET, COLORDIVIDER, distro);
    printf(" %s%s%s%s", RED, COLORSYMBOL, BRED, COLORSYMBOL);
    printf("%s%s%s%s%s\n", BWHITE, MODELTXT, RESET, COLORDIVIDER, model);
    printf(" %s%s%s%s", GREEN, COLORSYMBOL, BGREEN, COLORSYMBOL);
    printf("%s%s%s%s%s\n", BWHITE, KERNELTXT, RESET, COLORDIVIDER, kernel);
    printf(" %s%s%s%s", YELLOW, COLORSYMBOL, BYELLOW, COLORSYMBOL);
    printf("%s%s%s%s%s\n", BWHITE, UPTIMETXT, RESET, COLORDIVIDER, uptime);
    printf(" %s%s%s%s", BLUE, COLORSYMBOL, BBLUE, COLORSYMBOL);
    printf("%s%s%s%s%s\n", BWHITE, PKGSTXT, RESET, COLORDIVIDER, pkgs);
    printf(" %s%s%s%s", MAGENTA, COLORSYMBOL, BMAGENTA, COLORSYMBOL);
    printf("%s%s%s%s%s\n", BWHITE, SHELLTXT, RESET, COLORDIVIDER, shell);
    printf(" %s%s%s%s", CYAN, COLORSYMBOL, BCYAN, COLORSYMBOL);
    printf("%s%s%s%s%s\n", BWHITE, CPUTXT, RESET, COLORDIVIDER, cpu);
    printf(" %s%s%s%s", WHITE, COLORSYMBOL, BWHITE, COLORSYMBOL);
    printf("%s%s%s%s%s\n", BWHITE, RAMTXT, RESET, COLORDIVIDER, ram);

    printf("%s", getSpacer(COLORLINE, COLORLINELEFTLEN));
    printf("%s", COLORLINEDIVIDERBOTTOM);
    printf("%s\n", getSpacer(COLORLINE, linelen + 2));
}

int main(int argc, char *argv[]) {
    pthread_t threads[11];

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

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);
    pthread_join(threads[4], NULL);
    pthread_join(threads[5], NULL);
    pthread_join(threads[6], NULL);
    pthread_join(threads[7], NULL);
    pthread_join(threads[8], NULL);
    pthread_join(threads[9], NULL);
    pthread_join(threads[10], NULL);

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
