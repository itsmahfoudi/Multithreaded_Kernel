#include "pparser.h"
#include "kernel.h"
#include "string/string.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "status.h"

static int pathparser_path_valid_format(const char* filename) {
    int len = strnlen(filename, FASKA_OS_MAX_PATH);
    return (len > 3 && is_digit(filename[0]) && (memcmp((void*)&filename[1], ":/", 2) == 0));
}

static int pathparser_get_drive_by_path(const char** path) {
    if (!pathparser_path_valid_format(*path)) {
        return -EINVPATH;
    }
    int drive_num = to_numeric_digit((*path)[0]);
    // Skip drive number and colon ([0-9]:/...)
    *path += 3;
    return drive_num;
}

static path_root* pathparser_create_root(int drive_num) {
    path_root* root = (path_root*) kzalloc(sizeof(path_root));
    root->drive_num = drive_num;
    root->first = NULL;
    return root;
}

static const char* pathparser_get_next_part(const char** path) {
    char* result_path_part = (char*) kzalloc(FASKA_OS_MAX_PATH);
    int i = 0;
    while (**path != '/' && **path != '\0') {
        result_path_part[i++] = **path;
        (*path)++;
    }
    // Skip slash
    if (**path == '/') {
        (*path)++;
    }
    if (i == 0) {
        kfree(result_path_part);
        return NULL;
    }
    return result_path_part;
}

static path_part* pathparser_parse_path_part(path_part* last_part, const char** path) {
    const char* part = pathparser_get_next_part(path);
    if (part == NULL) {
        return NULL;
    }
    path_part* new_part = (path_part*) kzalloc(sizeof(path_part));
    new_part->part = part;
    new_part->next = NULL;
    if (last_part != NULL) {
        last_part->next = new_part;
    }
    return new_part;
}

void pathparser_free(path_root* root) {
    path_part* current = root->first;
    while (current != NULL) {
        path_part* next = current->next;
        kfree((void*) current->part);
        kfree(current);
        current = next;
    }
    kfree(root);
}

path_root* pathparser_parse(const char* path, const char* current_directory_path) {
    int result = 0;
    const char* current_path = path;
    path_root* pathRoot = NULL;
    if (strlen(path) > FASKA_OS_MAX_PATH) {
        goto out;
    }
    result = pathparser_get_drive_by_path(&current_path);
    if (result < 0) {
        goto out;
    }
    pathRoot = pathparser_create_root(result);
    if (pathRoot == NULL) {
        goto out;
    }
    path_part* first_part = pathparser_parse_path_part(NULL, &current_path);
    if (first_part == NULL) {
        goto out;
    }
    pathRoot->first = first_part;
    path_part *part = pathparser_parse_path_part(first_part, &current_path);
    while (part) {
        part = pathparser_parse_path_part(part, &current_path);
    }

    out:
    return pathRoot;
}