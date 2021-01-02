/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-02T21:34:39+0100
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"

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
    int pacman;

    FILE *file = popen(PKGCMD, "r");
    fscanf(file, "%d", &pacman);
    fclose(file);

    sprintf(pkgs, "%d (pacman)", pacman);

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
    int ramavailable,
        ramtotal;

    FILE *file;
    if ((file = fopen("/proc/meminfo", "r"))) {
        file = popen("grep 'MemAvailable:' /proc/meminfo \
                | sed 's/MemAvailable://'", "r");
        fscanf(file, "%d", &ramavailable);

        file = popen("grep 'MemTotal:' /proc/meminfo \
                | sed 's/MemTotal://'", "r");
        fscanf(file, "%d", &ramtotal);
        fclose(file);

        ramavailable = ((ramtotal - ramavailable) / 1024);
        ramtotal = (ramtotal / 1024);

        sprintf(ram, "%dMiB / %dMiB", ramavailable, ramtotal);

        if (linelen < strlen(ram)) {
            linelen = strlen(ram);
        }
    }
}

void printAscii() {
    headerlen = strlen(user) + strlen(host) + strlen(zeit);
    if (linelen < headerlen - 7)
    {
        linelen = headerlen - 9;
    }

    printf("%s@%s", user, host);
    printf("%s%s\n", getSpacer(HEADERSPACER, linelen - headerlen + 10) ,zeit);
    printf("%s", getSpacer(ASCIILINE, 8));
    printf("%s", ASCIILINEDIVIDER);
    printf("%s\n", getSpacer(ASCIILINE, linelen + 2));

    printf("%s%s%s\n", DISTROTEXT, ASCIIDIVIDER, distro);
    printf("%s%s%s\n", MODELTEXT, ASCIIDIVIDER, model);
    printf("%s%s%s\n", KERNELTEXT, ASCIIDIVIDER, kernel);
    printf("%s%s%s\n", UPTIMETEXT, ASCIIDIVIDER, uptime);
    printf("%s%s%s\n", PKGSTEXT, ASCIIDIVIDER, pkgs);
    printf("%s%s%s\n", SHELLTEXT, ASCIIDIVIDER, shell);
    printf("%s%s%s\n", CPUTEXT, ASCIIDIVIDER, cpu);
    printf("%s%s%s\n", RAMTEXT, ASCIIDIVIDER, ram);

    printf("%s", getSpacer(ASCIILINE, 8));
    printf("%s", ASCIILINEDIVIDER);
    printf("%s\n", getSpacer(ASCIILINE, linelen + 2));
}

void printColor() {
    headerlen = strlen(user) + strlen(host) + strlen(zeit);
    if (linelen < headerlen - 12)
    {
        linelen = headerlen - 14;
    }

    printf("%s%s%s@%s%s%s", BWHITE, user, RESET, BWHITE, host, RESET);
    printf("%s%s\n", getSpacer(HEADERSPACER, linelen - headerlen + 15), zeit);
    printf("%s", getSpacer(COLORLINE, 13));
    printf("%s", COLORLINEDIVIDERTOP);
    printf("%s\n", getSpacer(COLORLINE, linelen + 2));

    printf(" %s%s%s%s", \
            BLACK, COLORSYMBOL, BBLACK, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, DISTROTEXT, RESET, COLORDIVIDER, distro);
    printf(" %s%s%s%s", \
            RED, COLORSYMBOL, BRED, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, MODELTEXT, RESET, COLORDIVIDER, model);
    printf(" %s%s%s%s", \
            GREEN, COLORSYMBOL, BGREEN, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, KERNELTEXT, RESET, COLORDIVIDER, kernel);
    printf(" %s%s%s%s", \
            YELLOW, COLORSYMBOL, BYELLOW, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, UPTIMETEXT, RESET, COLORDIVIDER, uptime);
    printf(" %s%s%s%s", \
            BLUE, COLORSYMBOL, BBLUE, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, PKGSTEXT, RESET, COLORDIVIDER, pkgs);
    printf(" %s%s%s%s", \
            MAGENTA, COLORSYMBOL, BMAGENTA, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, SHELLTEXT, RESET, COLORDIVIDER, shell);
    printf(" %s%s%s%s", \
            CYAN, COLORSYMBOL, BCYAN, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, CPUTEXT, RESET, COLORDIVIDER, cpu);
    printf(" %s%s%s%s", \
            WHITE, COLORSYMBOL, BWHITE, COLORSYMBOL);
    printf("%s%s%s%s%s\n", \
            BWHITE, RAMTEXT, RESET, COLORDIVIDER, ram);

    printf("%s", getSpacer(COLORLINE, 13));
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

    if (argc > 1) {
        if (strcmp(argv[1], "-a") == 0) {
            printAscii();
        } else if (strcmp(argv[1],"-h") == 0) {
            printf("%s\n\n", HELP);
        }
    } else {
        printColor();
    }
    return 0;
}
