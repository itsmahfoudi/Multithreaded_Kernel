#ifndef MULTITHREADED_KERNEL_PPARSER_H
#define MULTITHREADED_KERNEL_PPARSER_H


typedef struct path_part{
    const char* part;
    struct path_part* next;
}path_part;

typedef struct path_root {
    int drive_num;
    path_part* first;
}path_root;

path_root* pathparser_parse(const char* path, const char* current_directory_path);
void pathparser_free(path_root* root);
#endif //MULTITHREADED_KERNEL_PPARSER_H
