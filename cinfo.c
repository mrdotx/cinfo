/*
 * path:       /home/klassiker/.local/share/repos/cinfo/cinfo.c
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2020-12-31T13:29:54+0100
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

const char* detectUser()
{
    static char user[50];

    FILE *userName = popen("printf '%s\n' $USER", "r");
    fscanf(userName, "%s", user);
    fclose(userName);

    return(user);
}

const char* detectHost()
{
    static char host[50];

    if (fexists("/proc/sys/kernel/hostname") == 1)
    {
        FILE *userHost = fopen("/proc/sys/kernel/hostname", "r");
        fscanf(userHost, "%s", host);
        fclose(userHost);
    }
    return(host);
}

const char* detectTime()
{
    static char zeit[20];

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

    return(zeit);
}

const char* detectDistro()
{
    static char os[50];

    FILE *distroName = popen("cat /etc/*-release \
            | grep 'PRETTY_NAME=' \
            | cut -d '\"' -f2", "r");
    fscanf(distroName, "%[^\n]s", os);
    fclose(distroName);

    return(os);
}

const char* detectModel()
{
    char modelname[50] = "",
         modelversion[25] = "";
    static char model[65];

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
    return(model);
}

const char* detectKernel()
{
    static char kernel[50];

    if (fexists("/proc/sys/kernel/osrelease") == 1)
    {
        FILE *pathKernel = fopen("/proc/sys/kernel/osrelease", "r");
        fscanf(pathKernel, "%[^\n]s", kernel);
        fclose(pathKernel);
    }
    return(kernel);
}

const char* detectUptime()
{
    int sec,
        day,
        hour,
        min;
    static char uptime[] = "999d 23d 59m";

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
    return(uptime);
}

const char* detectPackages()
{
    int pacman;
    static char packages[] = "99999 (pacman)";

    FILE *packageman = popen("pacman -Q | wc -l", "r");
    fscanf(packageman, "%d", &pacman);
    fclose(packageman);

    sprintf(packages, "%d (pacman)", pacman);
    return(packages);
}

const char* detectShell()
{
    static char shell[] = "dash";

    FILE *shellpath = popen("basename $SHELL", "r");
    fscanf(shellpath, "%s", shell);
    fclose(shellpath);

    return(shell);
}

const char* detectCPU()
{
    static char cpu[50];

    if (fexists("/proc/cpuinfo") == 1)
    {
        FILE *cpuinfo = popen("cat /proc/cpuinfo \
                | grep 'model name	:' \
                | sed -r 's/model name	:\\s{1,}//'", "r");
        fscanf(cpuinfo, "%[^\n]s", cpu);
        fclose(cpuinfo);
    }
    return(cpu);
}

const char* detectRAM()
{
    int ramavailable,
        ramtotal;
    static char ram[] = "99999M / 99999M";

    if (fexists("/proc/meminfo") == 1)
    {
        FILE *available = popen("cat /proc/meminfo \
                | grep 'MemAvailable:' \
                | sed 's/MemAvailable://'", "r");
        fscanf(available, "%d", &ramavailable);
        fclose(available);

        FILE *total = popen("cat /proc/meminfo \
                | grep 'MemTotal:' \
                | sed 's/MemTotal://'", "r");
        fscanf(total, "%d", &ramtotal);
        fclose(total);

        ramavailable = ((ramtotal-ramavailable)/1024);
        ramtotal = (ramtotal/1024);

        sprintf(ram, "%dM / %dM", ramavailable, ramtotal);
    }
    return(ram);
}

int main(int argc, char *argv[])
{
    char lineleft[] = "─────────────",
         linedivider[] = "┬",
         lineright[] = "──────────────────────────────────────────────────────────────────",
         divider[] = "│",
         blocks[] = "██";

    if (argc > 1)
    {
        if (strcmp(argv[1], "-a") == 0)
        {
            strcpy(lineleft, "--------");
            strcpy(linedivider, "+");
            strcpy(lineright, "-----------------------------------------------------------------------");
            strcpy(divider, "|");

            printf("%s@%s - %s\n", detectUser(), detectHost(), detectTime());
            printf("%s%s%s\n", lineleft, linedivider, lineright);

            printf("     os %s %s\n", divider, detectDistro());
            printf("  model %s %s\n", divider, detectModel());
            printf(" kernel %s %s\n", divider, detectKernel());
            printf(" uptime %s %s\n", divider, detectUptime());
            printf("   pkgs %s %s\n", divider, detectPackages());
            printf("  shell %s %s\n", divider, detectShell());
            printf("    cpu %s %s\n", divider, detectCPU());
            printf("    ram %s %s\n", divider, detectRAM());
            printf("\n");
        } else if (strcmp(argv[1],"-h") == 0) {
            printf("%s\n\n", help);
        }
    } else {
        printf("%s%s%s@%s%s%s - %s\n", bold, detectUser(), reset, bold, detectHost(), reset, detectTime());
        printf("%s%s%s\n", lineleft, linedivider, lineright);

        printf(" %s%s%s%s%s", black, blocks, bold, blocks, reset);
        printf("%s     os %s%s %s\n", bold, divider, reset, detectDistro());
        printf(" %s%s%s%s%s", red, blocks, bold, blocks, reset);
        printf("%s  model %s%s %s\n", bold, divider, reset, detectModel());
        printf(" %s%s%s%s%s", green, blocks, bold, blocks, reset);
        printf("%s kernel %s%s %s\n", bold, divider, reset, detectKernel());
        printf(" %s%s%s%s%s", yellow, blocks, bold, blocks, reset);
        printf("%s uptime %s%s %s\n", bold, divider, reset, detectUptime());
        printf(" %s%s%s%s%s", blue, blocks, bold, blocks, reset);
        printf("%s   pkgs %s%s %s\n", bold, divider, reset, detectPackages());
        printf(" %s%s%s%s%s", magenta, blocks, bold, blocks, reset);
        printf("%s  shell %s%s %s\n", bold, divider, reset, detectShell());
        printf(" %s%s%s%s%s", cyan, blocks, bold, blocks, reset);
        printf("%s    cpu %s%s %s\n", bold, divider, reset, detectCPU());
        printf(" %s%s%s%s%s", white, blocks, bold, blocks, reset);
        printf("%s    ram %s%s %s\n", bold, divider, reset, detectRAM());
        printf("\n");
    }
    return 0;
}
