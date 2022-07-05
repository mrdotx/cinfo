/*
 * path:   /home/klassiker/.local/share/repos/cinfo/util.h
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2022-07-05T11:23:54+0200
 */

void split_string(char* input, char** output, char* delimiter);
const char *remove_char(char *string, const char *remove);
const int update_header_len(const char *line, int header_len);
const int update_line_len(const char *line, int line_len);
const char *set_spacer(const char *character, int length);
const double get_execution_time(void *print());
const char *remove_file(const char *file);
