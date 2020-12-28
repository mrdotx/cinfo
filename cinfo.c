/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2020-12-28T13:08:35+0100
 */

#include <stdio.h>

int day,
    hour,
    min,
    sec,
    pacman,
    ramused,
    ramtotal;

char user[50],
     host[50],
     os[25],
     model[50],
     modelversion[25],
     kernel[50],
     shell[10],
     cpu[50];

void detectUser() {
    FILE *userName = popen("printf '%s\n' $USER", "r");
    FILE *userHost = popen("cat /proc/sys/kernel/hostname", "r");

    fscanf(userName, "%s", user);
    fscanf(userHost, "%s", host);
    fclose(userName);
    fclose(userHost);
}

void detectDistro() {
    FILE *distroName = popen("cat /etc/*-release \
            | grep 'PRETTY_NAME=' \
            | cut -d '\"' -f2", "r");

    fscanf(distroName, "%[^\n]s", os);
    fclose(distroName);
}

void detectKernel() {
    FILE *pathKernel = popen("uname -r", "r");

    fscanf(pathKernel, "%[^\n]s", kernel);
    fclose(pathKernel);
}

void detectModel() {
    FILE *productName = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    FILE *productVersion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");

    fscanf(productName, "%[^\n]s", model);
    fscanf(productVersion, "%s", modelversion);
    fclose(productName);
    fclose(productVersion);
}

void detectShell() {
    FILE *shellpath = popen("basename $SHELL", "r");

    fscanf(shellpath, "%s", shell);
    fclose(shellpath);
}

void detectPackages() {
    FILE *packageman = popen("pacman -Q | wc -l", "r");

    fscanf(packageman, "%d", &pacman);
    fclose(packageman);
}

void detectCPU() {
    FILE *cpuinfo = popen("cat /proc/cpuinfo \
            | grep 'model name	:' \
            | sed -r 's/model name	:\\s{1,}//'", "r");

    fscanf(cpuinfo, "%[^\n]s", cpu);
    fclose(cpuinfo);
}

void detectRAM() {
    FILE *total = popen("cat /proc/meminfo \
            | grep 'MemTotal:' \
            | sed 's/MemTotal://'", "r");
    FILE *used = popen("cat /proc/meminfo \
            | grep 'MemFree:' \
            | sed 's/MemFree://'", "r");

    fscanf(total, "%d", &ramtotal);
    fscanf(used, "%d", &ramused);
    fclose(total);
    fclose(used);

    ramtotal = (ramtotal/1024);
    ramused = (ramused/1024);
}

void detectUptime() {
    FILE *pathUptime = fopen("/proc/uptime", "r");

    fscanf(pathUptime, "%d", &sec);
    fclose(pathUptime);

    day = (sec/60/60/24);
    hour = (sec/60/60%24);
    min = (sec/60%60);
}

void getSysinfo() {
    detectUser();
    detectDistro();
    detectModel();
    detectKernel();
    detectUptime();
    detectPackages();
    detectShell();
    detectCPU();
    detectRAM();
}

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

int main() {
    getSysinfo();
    printf("\n");
    printf("%s%s%s@%s%s%s\n", \
            bold, user, reset, bold, host, reset);
    printf("─────────────┬──────────────────────────────────────────────────────────────────\n");
    printf(" %s██%s██%s%s     os %s│ %s\n", \
            black, bold, reset, bold, reset, os);
    printf(" %s██%s██%s%s  model %s│ %s %s\n", \
            red, bold, reset, bold, reset, model, modelversion);
    printf(" %s██%s██%s%s kernel %s│ %s\n", \
            green, bold, reset, bold, reset, kernel);
    printf(" %s██%s██%s%s uptime %s│ %dd %dh %dm\n", \
            yellow, bold, reset, bold, reset, day, hour, min);
    printf(" %s██%s██%s%s   pkgs %s│ %d(pacman)\n", \
            blue, bold, reset, bold, reset, pacman);
    printf(" %s██%s██%s%s  shell %s│ %s\n", \
            magenta, bold, reset, bold, reset, shell);
    printf(" %s██%s██%s%s    cpu %s│ %s\n", \
            cyan, bold, reset, bold, reset, cpu);
    printf(" %s██%s██%s%s    ram %s│ %dM / %dM\n", \
            white, bold, reset, bold, reset, ramused, ramtotal);
    printf("\n");
    return 0;
}
