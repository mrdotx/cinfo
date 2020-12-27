/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2020-12-27T21:38:33+0100
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
    FILE *pathKernel = popen("uname -rsm", "r");

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
    FILE *cpuinfo = popen("lscpu \
            | grep 'Modellname:' \
            | sed -r 's/Modellname:\\s{1,}//'", "r");

    fscanf(cpuinfo, "%[^\n]s", cpu);
    fclose(cpuinfo);
}

void detectRAM() {
    FILE *used = popen("vmstat -s -S M \
            | grep ' benutzter Speicher'", "r");
    FILE *total = popen("vmstat -s -S M \
            | grep ' Gesamtspeicher'", "r");

    fscanf(used, "%d", &ramused);
    fscanf(total, "%d", &ramtotal);
    fclose(used);
    fclose(total);
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
    printf("\n%s     %s%s@%s%s%s\n", \
            bold, user, reset, bold, host, reset);
    printf("%s██%s██%s OS     %s\n", \
            black, bold, reset, os);
    printf("%s██%s██%s MODEL  %s %s\n", \
            red, bold, reset, model, modelversion);
    printf("%s██%s██%s KERNEL %s\n", \
            green, bold, reset, kernel);
    printf("%s██%s██%s UPTIME %dd %dh %dm\n", \
            yellow, bold, reset, day, hour, min);
    printf("%s██%s██%s PKGS   %d(pacman)\n", \
            blue, bold, reset, pacman);
    printf("%s██%s██%s SHELL  %s\n", \
            magenta, bold, reset, shell);
    printf("%s██%s██%s CPU    %s\n", \
            cyan, bold, reset, cpu);
    printf("%s██%s██%s RAM    %dM / %dM\n\n", \
            white, bold, reset, ramused, ramtotal);
    return 0;
}
