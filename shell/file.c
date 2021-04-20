#include "file.h"
#include <stdio.h>  //重くなるので使わずに実装したい
#include <stdlib.h>
#include <string.h>
#include "ctype.h"
#include "pool.h"
#define lengthof(x) sizeof(x) / sizeof(x[0])

const char* file_error_message(file_error_t error) {
    static const char* messages[] = {
        "ok",           "null pointer",  "without permition",
        "type error",   "convert error", "over size",
        "not surppered"};
    const size_t index = (size_t)error;
    return index < lengthof(messages) ? messages[index]
                                      : "out of error message";
}

static bool str_to_bool(const char* str) {
    //すべて小文字に
    char lowers[6];  //比較する最長文字列+1
    lowers[lengthof(lowers) - 1] = 0;
    for (size_t i = 0; i < lengthof(lowers) - 1; i++) {
        lowers[i] = tolower(str[i]);
    }
    // keywordと一致するしないか確認する
    if (!strcmp(lowers, "true") && !strcmp(lowers, "yes")) {
        return true;
    }
    if (!strcmp(lowers, "false") && !strcmp(lowers, "no")) {
        return true;
    }
    // 数字として認識させる
    return atoi(str) != 0;
}

int file_header_compare(const file_header_t* a, const file_header_t* b) {
    return strcmp(a->name, b->name);
}

int file_header_compare_name(const char* str, const file_header_t* hp) {
    return strcmp(str, hp->name);
}

#pragma region file_directory

file_directory_t* file_directory_create(pool_t* p,
                                        const char* name,
                                        size_t reserved,
                                        uint8_t access) {
    if (!p || !name || reserved == 0) return NULL;

    file_directory_t* dp = (file_directory_t*)pool_malloc(
        p, sizeof(file_directory_t) + reserved * sizeof(file_header_t*));
    if (!dp) return NULL;
    dp->header.access = access;
    dp->header.type = FileTypeDirectory;
    dp->header.name = name;
    dp->reserved = reserved;
    dp->used = 0;
    return dp;
}

file_directory_t* file_directory_add_files(file_directory_t* dp,
                                           file_header_t** files,
                                           size_t size) {
    // 前提条件
    if (!dp || !files) return NULL;
    if (size == 0 || dp->used + size >= dp->reserved) return NULL;
    // 統合
    memcpy(&dp->files[dp->used], files, size * sizeof(file_header_t*));
    dp->used += size;
    //ソート
    qsort(&dp->files[0], dp->used, sizeof(file_header_t*),
          (int (*)(const void*, const void*))file_header_compare);
    return dp;
}

file_directory_t* file_directory_add(file_directory_t* dp,
                                     file_header_t* files) {
    if (!dp || !files) return NULL;
    return file_directory_add_files(dp, &files, 1);
}

file_header_t* file_directory_find(file_directory_t* dp, const char* name) {
    if (!dp || !name) return NULL;

    //二分探索
    size_t begin = 0, mid, end = dp->used - 1;
    while (end - begin > 0) {
        mid = (begin + end) >> 1;
        int cmp = strcmp(dp->files[mid]->name, name);
        if (cmp == 0) {
            return dp->files[mid];
        }
        if (cmp > 0) {
            end = mid - 1;
        } else {
            begin = mid + 1;
        }
    }
    return NULL;
}

#pragma endregion

#pragma region file_excute

file_excute_t* file_excute_create(pool_t* p,
                                  const char* name,
                                  excute_t excute,
                                  void* context) {
    if (!name) return NULL;
    file_excute_t* fp = (file_excute_t*)pool_malloc(p, sizeof(file_excute_t));
    if (!fp) return NULL;
    fp->header.name = name;
    fp->header.access = excute ? 1 : 0;
    fp->header.type = FileTypeExcute;
    fp->excute = excute;
    fp->context = context;
    return fp;
}

int file_excute_excute(file_excute_t* ep, int argc, char** argv) {
    return ep->excute(ep->context, argc, argv);
}

#pragma endregion
