/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-01T11:33:58+0100
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#define help "cinfo [-h] -- a minimal system information tool\n\
              for arch-based distributions\n\
  Usage:\n\
    cinfo [-a]\n\n\
  Settings:\n\
    without given settings, start colored version\n\
    [-a] = start plain ascii version\n\n\
  Examples:\n\
    cinfo\n\
    cinfo -a"

#define reset "\x1b[0m"
#define bold "\x1b[1m"
#define black "\x1b[30m"
#define red "\x1b[31m"
#define green "\x1b[32m"
#define yellow "\x1b[33m"
#define blue "\x1b[34m"
#define magenta "\x1b[35m"
#define cyan "\x1b[36m"
#define white "\x1b[37m"

int linelen;

char user[50],
     host[50],
     zeit[] = "31.12.2020 23:59:59",
     distro[50],
     model[65],
     kernel[50],
     uptime[] = "999d 23h 59m",
     pkgs[] = "99999 (pacman)",
     shell[] = "dash",
     cpu[50],
     ram[] = "99999M / 99999M",
     color[15];

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
    FILE *userName = popen("printf '%s\n' $USER", "r");
    fscanf(userName, "%s", user);
    fclose(userName);
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
    FILE *distroName = popen("cat /etc/*-release \
            | grep 'PRETTY_NAME=' \
            | cut -d '\"' -f2", "r");
    fscanf(distroName, "%[^\n]s", distro);
    fclose(distroName);

    if (linelen < strlen(distro)) {
        linelen = strlen(distro);
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

        sprintf(uptime, "%dd %dh %dm", day, hour, min);

        if (linelen < strlen(uptime)) {
            linelen = strlen(uptime);
        }
    }
}

void getPackages() {
    int pacman;

    FILE *file = popen("pacman -Q | wc -l", "r");
    fscanf(file, "%d", &pacman);
    fclose(file);

    sprintf(pkgs, "%d (pacman)", pacman);

    if (linelen < strlen(pkgs)) {
        linelen = strlen(pkgs);
    }
}

void getShell() {
    FILE *shellpath = popen("basename $SHELL", "r");
    fscanf(shellpath, "%s", shell);
    fclose(shellpath);

    if (linelen < strlen(shell)) {
        linelen = strlen(shell);
    }
}

void getCPU() {
    FILE *file;
    if ((file = fopen("/proc/cpuinfo", "r"))) {
        file = popen("cat /proc/cpuinfo \
                | grep 'model name	:' \
                | sed -r 's/model name	:\\s{1,}//'", "r");
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
        file = popen("cat /proc/meminfo \
                | grep 'MemAvailable:' \
                | sed 's/MemAvailable://'", "r");
        fscanf(file, "%d", &ramavailable);

        file = popen("cat /proc/meminfo \
                | grep 'MemTotal:' \
                | sed 's/MemTotal://'", "r");
        fscanf(file, "%d", &ramtotal);
        fclose(file);

        ramavailable = ((ramtotal - ramavailable) / 1024);
        ramtotal = (ramtotal / 1024);

        sprintf(ram, "%dM / %dM", ramavailable, ramtotal);

        if (linelen < strlen(ram)) {
            linelen = strlen(ram);
        }
    }
}

void getSysInfo() {
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
}

void getAscii() {
    if (linelen < strlen(user) + strlen(host) + strlen(zeit) - 5)
    {
        linelen = strlen(user) + strlen(host) + strlen(zeit) - 7;
    }

    printf("%s@%s - %s\n", user, host, zeit);
    printf("--------+%s\n", getSpacer("-", linelen + 2));

    printf(" distro | %s\n", distro);
    printf("  model | %s\n", model);
    printf(" kernel | %s\n", kernel);
    printf(" uptime | %s\n", uptime);
    printf("   pkgs | %s\n", pkgs);
    printf("  shell | %s\n", shell);
    printf("    cpu | %s\n", cpu);
    printf("    ram | %s\n", ram);

    printf("--------+%s\n", getSpacer("-", linelen + 2));
}

void getColor() {
    sprintf(color, " ██%s██%s ", bold, reset);

    if (linelen < strlen(user) + strlen(host) + strlen(zeit) - 10)
    {
        linelen = strlen(user) + strlen(host) + strlen(zeit) - 12;
    }

    printf("%s%s%s@%s%s%s - %s\n", bold, user, reset, bold, host, reset, zeit);
    printf("─────────────┬%s\n", getSpacer("─", linelen + 2));

    printf("%s%s%sdistro%s │ %s\n", black, color, bold, reset, distro);
    printf("%s%s%s model%s │ %s\n", red, color, bold, reset, model);
    printf("%s%s%skernel%s │ %s\n", green, color, bold, reset, kernel);
    printf("%s%s%suptime%s │ %s\n", yellow, color, bold, reset, uptime);
    printf("%s%s%s  pkgs%s │ %s\n", blue, color, bold, reset, pkgs);
    printf("%s%s%s shell%s │ %s\n", magenta, color, bold, reset, shell);
    printf("%s%s%s   cpu%s │ %s\n", cyan, color, bold, reset, cpu);
    printf("%s%s%s   ram%s │ %s\n", white, color, bold, reset, ram);

    printf("─────────────┴%s\n", getSpacer("─", linelen + 2));
}

int main(int argc, char *argv[]) {
    getSysInfo();

    if (argc > 1) {
        if (strcmp(argv[1], "-a") == 0) {
            getAscii();
        } else if (strcmp(argv[1],"-h") == 0) {
            printf("%s\n\n", help);
        }
    } else {
        getColor();
    }
    return 0;
}
