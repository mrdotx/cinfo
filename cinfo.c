/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2020-12-30T14:17:10+0100
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

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
     os[50],
     model[50],
     modelversion[25],
     kernel[50],
     shell[15],
     cpu[50];

char lineleft[] = "─────────────",
     linedivider[] = "┬",
     lineright[] = "──────────────────────────────────────────────────────────────────",
     divider[] = "│",
     blocks[] = "██";

int fexists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void detectUser()
{
    FILE *userName = popen("printf '%s\n' $USER", "r");

    fscanf(userName, "%s", user);
    fclose(userName);

    if (fexists("/proc/sys/kernel/hostname") == 1)
    {
        FILE *userHost = fopen("/proc/sys/kernel/hostname", "r");

        fscanf(userHost, "%s", host);
        fclose(userHost);
    }
}

void detectDistro()
{
    FILE *distroName = popen("cat /etc/*-release \
            | grep 'PRETTY_NAME=' \
            | cut -d '\"' -f2", "r");

    fscanf(distroName, "%[^\n]s", os);
    fclose(distroName);
}

void detectKernel()
{
    if (fexists("/proc/sys/kernel/osrelease") == 1)
    {
        FILE *pathKernel = fopen("/proc/sys/kernel/osrelease", "r");

        fscanf(pathKernel, "%[^\n]s", kernel);
        fclose(pathKernel);
    }
}

void detectModel()
{
    if (fexists("/sys/devices/virtual/dmi/id/product_name") == 1)
    {
        FILE *productName = fopen("/sys/devices/virtual/dmi/id/product_name", "r");

        fscanf(productName, "%[^\n]s", model);
        fclose(productName);
    } else {
        strcpy(model, "not found");
    }

    if (fexists("/sys/devices/virtual/dmi/id/product_version") == 1)
    {
        FILE *productVersion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");

        fscanf(productVersion, "%s", modelversion);
        fclose(productVersion);
    }
}

void detectShell()
{
    FILE *shellpath = popen("basename $SHELL", "r");

    fscanf(shellpath, "%s", shell);
    fclose(shellpath);
}

void detectPackages()
{
    FILE *packageman = popen("pacman -Q | wc -l", "r");

    fscanf(packageman, "%d", &pacman);
    fclose(packageman);
}

void detectCPU()
{
    if (fexists("/proc/cpuinfo") == 1)
    {
        FILE *cpuinfo = popen("cat /proc/cpuinfo \
                | grep 'model name	:' \
                | sed -r 's/model name	:\\s{1,}//'", "r");

        fscanf(cpuinfo, "%[^\n]s", cpu);
        fclose(cpuinfo);
    }
}

void detectRAM()
{
    if (fexists("/proc/meminfo") == 1)
    {
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
}

void detectUptime()
{
    if (fexists("/proc/uptime") == 1)
    {
        FILE *pathUptime = fopen("/proc/uptime", "r");

        fscanf(pathUptime, "%d", &sec);
        fclose(pathUptime);

        day = (sec/60/60/24);
        hour = (sec/60/60%24);
        min = (sec/60%60);
    }
}

void getSysinfo()
{
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

int main(int argc, char *argv[])
{
    time_t t;
    struct tm * zeit;
    time(&t);
    zeit = localtime(&t);

    getSysinfo();

    if (argc > 1)
    {
        if (strcmp(argv[1], "-a") == 0)
        {
            strcpy(lineleft, "--------");
            strcpy(linedivider, "+");
            strcpy(lineright, "-----------------------------------------------------------------------");
            strcpy(divider, "|");

            printf("%s@%s - %02d.%02d.%04d %02d:%02d:%02d\n", user, host, zeit->tm_mday, zeit->tm_mon+1, zeit->tm_year+1900, zeit->tm_hour, zeit->tm_min, zeit->tm_sec);
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
            printf("%s\n\n", help);
        }
    } else {
        printf("%s%s%s@%s%s%s - %02d.%02d.%04d %02d:%02d:%02d\n", bold, user, reset, bold, host, reset, zeit->tm_mday, zeit->tm_mon+1, zeit->tm_year+1900, zeit->tm_hour, zeit->tm_min, zeit->tm_sec);
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
