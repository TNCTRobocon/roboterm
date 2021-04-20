#pragma once
#ifndef __HEADER__GUARD__SHELL__
#define __HEADER__GUARD__SHELL__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "pool.h"
/* file_error
 * ファイルの読み書き時のエラーについて
 */
enum file_error {
    FileErrorNone = 0, /* OK */
    FileErrorNull = 1,
    FileErorrPermition = 2,
    FileErrorType = 3,
    FileErrorConvert = 4,
    FileErrorOverSize = 5,
    FileErrorNotSupported = 6
};

typedef enum file_error file_error_t;

#pragma region file_header

typedef enum file_type {
    FileTypeDirectory,
    FileTypeExcute,
    FileTypePipe,
} file_type_t;

typedef struct file_header {
    unsigned access : 3;  //アクセス権
    unsigned type : 5;    //ファイルの種類
    const char* name;     //ファイル名
} file_header_t;

int file_header_compare(const file_header_t*, const file_header_t*);
int file_header_compare_name(const char*, const file_header_t*);

#pragma endregion

#pragma region file_directory

typedef struct file_directory {
    file_header_t header;
    size_t reserved, used;
    file_header_t* files[0];
} file_directory_t;

file_directory_t* file_directory_create(pool_t*,
                                        const char* name,
                                        size_t reserved,
                                        uint8_t access);

file_directory_t* file_directory_add_files(file_directory_t*,
                                           file_header_t** files,
                                           size_t size);
file_directory_t* file_directory_add(file_directory_t*, file_header_t* files);
static inline file_header_t** file_directory_list(file_directory_t* dp) {
    return dp ? dp->files : NULL;
}
file_header_t* file_directory_find(file_directory_t*, const char* name);

#pragma endregion

#pragma file_excute

typedef int (*excute_t)(void* context, int argc, char** argv);
typedef struct file_excute {
    file_header_t header;
    excute_t excute;
    void* context;
} file_excute_t;

file_excute_t* file_excute_create(pool_t*,
                                  const char* name,
                                  excute_t excute,
                                  void* context);
int file_excute_excute(file_excute_t*, int argc, char** argv);

#pragma endregion

#endif