/*
 * path:   /home/klassiker/.local/share/repos/cinfo/util.c
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2022-07-06T14:32:51+0200
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "util.h"

void split_string(char* input, char** output, char* delimiter) {
    char *temp;
    temp = strtok(input, delimiter);

    for (int i = 0; temp != NULL; i++) {
        output[i] = temp;
        temp = strtok(NULL, " ");
    }
}

const char *remove_char(char *string, const char *remove) {
    int i = 0, j;

    while (i < strlen(string)) {
        if (string[i] == *remove) {
            for (j = i; j < strlen(string); j++)
                string[j] = string[j + 1];
        }
        else i++;
    }

    return string;
}

const int update_header_len(const char *line, int header_len) {
    if (line == NULL) {
        header_len = 0;
    } else {
        header_len += strlen(line);
    }

    return header_len;
}

const int update_line_len(const char *line, int line_len) {
    if (line == NULL) {
        line_len = 0;
    } else if (line_len < strlen(line)) {
        line_len = strlen(line);
    }

    return line_len;
}

const char *set_spacer(const char *character, int length) {
    int i;

    static char spacer[65];

    spacer[0] = '\0';

    for (i = 0; i < length; i++) {
        strcat(spacer, character);
    }

    return spacer;
}

const double get_execution_time(void *print()) {
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

    print();

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spend = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    return time_spend;
}

const char *remove_file(const char *file) {
    static char string[2];

    if (0 == remove(file)) {
        sprintf(string, "x");
    } else {
        sprintf(string, "?");
    }

    return string;
}
