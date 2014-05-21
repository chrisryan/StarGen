#pragma once

char *engineer_notation(long double, int);
void text_describe_system(planet_pointer, bool, long);
void csv_describe_system(FILE *, planet_pointer, bool, long);
void csv_thumbnails(FILE*, char*, char*, char*, char*);
const char *type_string(planet_type);
void create_svg_file (FILE *, planet_pointer, const char *, const char *, const char *, const char *);
FILE *open_csv_file (char *, char *);
FILE *open_html_file(const char *, long, const char *, const char *, const char *, const char *, const char *, FILE *);
void close_html_file(FILE *);
void html_thumbnails(planet_pointer, FILE *, char *, char *, char *, char *, char *, bool, bool, bool, bool, int);
void html_thumbnail_totals(FILE *);
void html_describe_system(planet_pointer, bool, char *, FILE *);
void celestia_describe_system(planet_pointer, char *);
