/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2020-12-30T18:33:01+0100
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
    [-a] = plain ascii without colors\n\n\
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

char user[50],
     host[50],
     zeit[20],
     os[50],
     model[80],
     kernel[50],
     uptime[50],
     packages[15],
     shell[10],
     cpu[50],
     ram[10];

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

void detectTime()
{
    time_t raw;
    struct tm * part;
    time(&raw);
    part = localtime(&raw);

    sprintf(zeit, "%02d.%02d.%d %02d:%02d:%02d", \
            part->tm_mday, \
            part->tm_mon+1, \
            part->tm_year+1900, \
            part->tm_hour, \
            part->tm_min, \
            part->tm_sec);
}

void detectDistro()
{
    FILE *distroName = popen("cat /etc/*-release \
            | grep 'PRETTY_NAME=' \
            | cut -d '\"' -f2", "r");

    fscanf(distroName, "%[^\n]s", os);
    fclose(distroName);
}

void detectModel()
{
    char modelname[50] = "",
         modelversion[25] = "";

    if (fexists("/sys/devices/virtual/dmi/id/product_name") == 1)
    {
        FILE *productName = fopen("/sys/devices/virtual/dmi/id/product_name", "r");

        fscanf(productName, "%[^\n]s", modelname);
        fclose(productName);
    } else {
        strcpy(modelname, "not found");
    }

    if (fexists("/sys/devices/virtual/dmi/id/product_version") == 1)
    {
        FILE *productVersion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");

        fscanf(productVersion, "%s", modelversion);
        fclose(productVersion);
    }

    sprintf(model, "%s %s", modelname, modelversion);
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

void detectUptime()
{
    int sec,
        day,
        hour,
        min;

    if (fexists("/proc/uptime") == 1)
    {
        FILE *pathUptime = fopen("/proc/uptime", "r");

        fscanf(pathUptime, "%d", &sec);
        fclose(pathUptime);

        day = (sec/60/60/24);
        hour = (sec/60/60%24);
        min = (sec/60%60);

        sprintf(uptime, "%dd %dh %dm", day, hour, min);
    }
}

void detectPackages()
{
    int pacman;

    FILE *packageman = popen("pacman -Q | wc -l", "r");

    fscanf(packageman, "%d", &pacman);
    fclose(packageman);

    sprintf(packages, "%d (pacman)", pacman);
}

void detectShell()
{
    FILE *shellpath = popen("basename $SHELL", "r");

    fscanf(shellpath, "%s", shell);
    fclose(shellpath);
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
    int ramavailable,
        ramtotal;

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

        sprintf(ram, "%dM / %dM", ramavailable, ramtotal);
    }
}

int main(int argc, char *argv[])
{
    char lineleft[] = "─────────────",
         linedivider[] = "┬",
         lineright[] = "──────────────────────────────────────────────────────────────────",
         divider[] = "│",
         blocks[] = "██";

    detectUser();
    detectTime();
    detectDistro();
    detectModel();
    detectKernel();
    detectUptime();
    detectPackages();
    detectShell();
    detectCPU();
    detectRAM();

    if (argc > 1)
    {
        if (strcmp(argv[1], "-a") == 0)
        {
            strcpy(lineleft, "--------");
            strcpy(linedivider, "+");
            strcpy(lineright, "-----------------------------------------------------------------------");
            strcpy(divider, "|");

            printf("%s@%s - %s\n", user, host, zeit);
            printf("%s%s%s\n", lineleft, linedivider, lineright);

            printf("     os %s %s\n", divider, os);
            printf("  model %s %s\n", divider, model);
            printf(" kernel %s %s\n", divider, kernel);
            printf(" uptime %s %s\n", divider, uptime);
            printf("   pkgs %s %s\n", divider, packages);
            printf("  shell %s %s\n", divider, shell);
            printf("    cpu %s %s\n", divider, cpu);
            printf("    ram %s %s\n\n", divider, ram);
        } else if (strcmp(argv[1],"-h") == 0) {
            printf("%s\n\n", help);
        }
    } else {
        printf("%s%s%s@%s%s%s - %s\n", bold, user, reset, bold, host, reset, zeit);
        printf("%s%s%s\n", lineleft, linedivider, lineright);

        printf(" %s%s%s%s%s", black, blocks, bold, blocks, reset);
        printf("%s     os %s%s %s\n", bold, divider, reset, os);
        printf(" %s%s%s%s%s", red, blocks, bold, blocks, reset);
        printf("%s  model %s%s %s\n", bold, divider, reset, model);
        printf(" %s%s%s%s%s", green, blocks, bold, blocks, reset);
        printf("%s kernel %s%s %s\n", bold, divider, reset, kernel);
        printf(" %s%s%s%s%s", yellow, blocks, bold, blocks, reset);
        printf("%s uptime %s%s %s\n", bold, divider, reset, uptime);
        printf(" %s%s%s%s%s", blue, blocks, bold, blocks, reset);
        printf("%s   pkgs %s%s %s\n", bold, divider, reset, packages);
        printf(" %s%s%s%s%s", magenta, blocks, bold, blocks, reset);
        printf("%s  shell %s%s %s\n", bold, divider, reset, shell);
        printf(" %s%s%s%s%s", cyan, blocks, bold, blocks, reset);
        printf("%s    cpu %s%s %s\n", bold, divider, reset, cpu);
        printf(" %s%s%s%s%s", white, blocks, bold, blocks, reset);
        printf("%s    ram %s%s %s\n\n", bold, divider, reset, ram);
    }
    return 0;
}
