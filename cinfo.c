/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2020-12-29T20:29:40+0100
 */

#include <stdio.h>
#include <string.h>

#define help "cinfo [-h] -- a minimal system information tool\n\
              for arch-based distributions\n\
  Usage:\n\
    cinfo [-a]\n\n\
  Settings:\n\
    without given settings, colored version is used\n\
    [-a] = ascii without colors\n\n\
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

int day,
    hour,
    min,
    sec,
    pacman,
    ramavailable,
    ramtotal;

char user[50],
     host[50],
     os[25],
     model[50],
     modelversion[25],
     kernel[50],
     shell[10],
     cpu[50];

char lineleft[] = "─────────────",
     linedivider[] = "┬",
     lineright[] = "──────────────────────────────────────────────────────────────────",
     divider[] = "│",
     blocks[] = "██";

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
    FILE *available = popen("cat /proc/meminfo \
            | grep 'MemAvailable:' \
            | sed 's/MemAvailable://'", "r");
    FILE *total = popen("cat /proc/meminfo \
            | grep 'MemTotal:' \
            | sed 's/MemTotal://'", "r");

    fscanf(available, "%d", &ramavailable);
    fscanf(total, "%d", &ramtotal);
    fclose(available);
    fclose(total);

    ramavailable = ((ramtotal-ramavailable)/1024);
    ramtotal = (ramtotal/1024);
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

int main(int argc, char *argv[]) {

    getSysinfo();

    if(argc > 1)
    {
        if(strcmp(argv[1],"-a") == 0)
        {
            strcpy(lineleft, "--------");
            strcpy(linedivider, "+");
            strcpy(lineright, "-----------------------------------------------------------------------");
            strcpy(divider, "|");

            printf("\n%s@%s\n", user, host);
            printf("%s%s%s\n", lineleft, linedivider, lineright);

            printf("     os %s %s\n", divider, os);
            printf("  model %s %s %s\n", divider, model, modelversion);
            printf(" kernel %s %s\n", divider, kernel);
            printf(" uptime %s %dd %dh %dm\n", divider, day, hour, min);
            printf("   pkgs %s %d(pacman)\n", divider, pacman);
            printf("  shell %s %s\n", divider, shell);
            printf("    cpu %s %s\n", divider, cpu);
            printf("    ram %s %dM / %dM\n\n", divider, ramavailable, ramtotal);
        } else if (strcmp(argv[1],"-h") == 0) {
            printf("\n%s\n\n", help);
        }
    } else {
        printf("\n%s%s%s@%s%s%s\n", bold, user, reset, bold, host, reset);
        printf("%s%s%s\n", lineleft, linedivider, lineright);

        printf(" %s%s%s%s%s", black, blocks, bold, blocks, reset);
        printf("%s     os %s%s %s\n", bold, divider, reset, os);
        printf(" %s%s%s%s%s", red, blocks, bold, blocks, reset);
        printf("%s  model %s%s %s %s\n", bold, divider, reset, model, modelversion);
        printf(" %s%s%s%s%s", green, blocks, bold, blocks, reset);
        printf("%s kernel %s%s %s\n", bold, divider, reset, kernel);
        printf(" %s%s%s%s%s", yellow, blocks, bold, blocks, reset);
        printf("%s uptime %s%s %dd %dh %dm\n", bold, divider, reset, day, hour, min);
        printf(" %s%s%s%s%s", blue, blocks, bold, blocks, reset);
        printf("%s   pkgs %s%s %d(pacman)\n", bold, divider, reset, pacman);
        printf(" %s%s%s%s%s", magenta, blocks, bold, blocks, reset);
        printf("%s  shell %s%s %s\n", bold, divider, reset, shell);
        printf(" %s%s%s%s%s", cyan, blocks, bold, blocks, reset);
        printf("%s    cpu %s%s %s\n", bold, divider, reset, cpu);
        printf(" %s%s%s%s%s", white, blocks, bold, blocks, reset);
        printf("%s    ram %s%s %dM / %dM\n\n", bold, divider, reset, ramavailable, ramtotal);
    }
    return 0;
}
