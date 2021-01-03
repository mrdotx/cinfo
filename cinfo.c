/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-03T21:30:15+0100
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"

#define RESET "\033[0m"

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"

#define BBLACK "\033[1;30m"
#define BRED "\033[1;31m"
#define BGREEN "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BBLUE "\033[1;34m"
#define BMAGENTA "\033[1;35m"
#define BCYAN "\033[1;36m"
#define BWHITE "\033[1;37m"

int linelen,
    headerlen;

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
     ram[20];

const char* getSpacer(char *character, int length) {
    static char spacer[] = "";
    int i = 1;

    strcpy(spacer, "");

    while (i <= length) {
        strcat(spacer, character);
        i++;
    }

    return(spacer);
}

void getUser() {
    sprintf(user, "%s", getenv("USER"));
}

void getHost() {
    FILE *file;
    if ((file = fopen("/proc/sys/kernel/hostname", "r"))) {
        fscanf(file, "%s", host);
        fclose(file);
    }
}

void getTime() {
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
}

void getDistro() {
    FILE *file;
    if ((file = fopen("/etc/os-release", "r"))) {
        file = popen("grep 'PRETTY_NAME=' /etc/os-release \
                | cut -d '\"' -f2", "r");
        fscanf(file, "%[^\n]s", distro);
        fclose(file);

        if (linelen < strlen(distro)) {
            linelen = strlen(distro);
        }
    }
}

void getModel() {
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
        strcpy(modelname, "not found");
    }

    sprintf(model, "%s %s", modelname, modelversion);

    if (linelen < strlen(model)) {
        linelen = strlen(model);
    }
}

void getKernel() {
    FILE *file;
    if ((file = fopen("/proc/sys/kernel/osrelease", "r"))) {
        fscanf(file, "%[^\n]s", kernel);
        fclose(file);

        if (linelen < strlen(kernel)) {
            linelen = strlen(kernel);
        }
    }
}

void getUptime() {
    int sec,
        day,
        hour,
        min;

    FILE *file;
    if ((file = fopen("/proc/uptime", "r"))) {
        fscanf(file, "%d", &sec);
        fclose(file);

        day = (sec / 60 / 60 / 24);
        hour = (sec / 60 / 60 % 24);
        min = (sec / 60 % 60);

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
}

void getPackages() {
    int pkgcnt;

    FILE *file = popen(PKGCMD, "r");
    fscanf(file, "%d", &pkgcnt);
    fclose(file);

    sprintf(pkgs, "%d%s", pkgcnt, PKGDESC);

    if (linelen < strlen(pkgs)) {
        linelen = strlen(pkgs);
    }
}

void getShell() {
    sprintf(shell, "%s", getenv("SHELL"));

    if (linelen < strlen(shell)) {
        linelen = strlen(shell);
    }
}

void getCPU() {
    FILE *file;
    if ((file = fopen("/proc/cpuinfo", "r"))) {
        file = popen("grep -m 1 'model name	:' /proc/cpuinfo \
                | sed 's/model name	: //'", "r");
        fscanf(file, "%[^\n]s", cpu);
        fclose(file);

        if (linelen < strlen(cpu)) {
            linelen = strlen(cpu);
        }
    }
}

void getRAM() {
    int memtotal,
        memavailable;

    FILE *file;
    if ((file = fopen("/proc/meminfo", "r"))) {
        file = popen("grep 'MemTotal:' /proc/meminfo \
                | sed 's/MemTotal://'", "r");
        fscanf(file, "%d", &memtotal);

        file = popen("grep 'MemAvailable:' /proc/meminfo \
                | sed 's/MemAvailable://'", "r");
        fscanf(file, "%d", &memavailable);
        fclose(file);

        memavailable = ((memtotal - memavailable) / 1024);
        memtotal = (memtotal / 1024);

        sprintf(ram, "%dMiB / %dMiB", memavailable, memtotal);

        if (linelen < strlen(ram)) {
            linelen = strlen(ram);
        }
    }
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

    printf(" %s%s%s%s", \
            BLACK, COLORSYMBOL, BBLACK, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, DISTROTXT, RESET, COLORDIVIDER, distro);
    printf(" %s%s%s%s", \
            RED, COLORSYMBOL, BRED, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, MODELTXT, RESET, COLORDIVIDER, model);
    printf(" %s%s%s%s", \
            GREEN, COLORSYMBOL, BGREEN, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, KERNELTXT, RESET, COLORDIVIDER, kernel);
    printf(" %s%s%s%s", \
            YELLOW, COLORSYMBOL, BYELLOW, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, UPTIMETXT, RESET, COLORDIVIDER, uptime);
    printf(" %s%s%s%s", \
            BLUE, COLORSYMBOL, BBLUE, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, PKGSTXT, RESET, COLORDIVIDER, pkgs);
    printf(" %s%s%s%s", \
            MAGENTA, COLORSYMBOL, BMAGENTA, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, SHELLTXT, RESET, COLORDIVIDER, shell);
    printf(" %s%s%s%s", \
            CYAN, COLORSYMBOL, BCYAN, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, CPUTXT, RESET, COLORDIVIDER, cpu);
    printf(" %s%s%s%s", \
            WHITE, COLORSYMBOL, BWHITE, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, RAMTXT, RESET, COLORDIVIDER, ram);

    printf("%s", getSpacer(COLORLINE, COLORLINELEFTLEN));
    printf("%s", COLORLINEDIVIDERBOTTOM);
    printf("%s\n", getSpacer(COLORLINE, linelen + 2));
}

int main(int argc, char *argv[]) {
    getUser();
    getHost();
    getTime();
    getDistro();
    getModel();
    getKernel();
    getUptime();
    getPackages();
    getShell();
    getCPU();
    getRAM();

    if (argc == 1) {
        printColor();
    } else if (strcmp(argv[1], "-a") == 0) {
        printAscii();
    } else {
        printUsage();
    }
    return 0;
}
