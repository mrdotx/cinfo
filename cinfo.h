/*
 * path:   /home/klassiker/.local/share/repos/cinfo/cinfo.h
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2022-07-15T11:48:58+0200
 */

void *get_user();
void *get_host();
void *get_datetime();
void *get_distro();
void *get_kernel();
void *get_pkgs();
void *get_model();
void *get_cpu();
void *get_mem();
void *get_uptime();
void *get_shell();
void get_infos(void *print());
void print_header(const int left_len,
                  const char *color_primary,
                  const char *color_secondary,
                  const char *color_table);
void print_line(const int left_len,
                const char *line,
                const char *divider);
void print_info(const char *label,
                const char *info);
void *print_ascii();
void *print_color();
void get_execution_times();
void delete_cache();
void print_usage();
