#include "file.h"
#include <stdio.h>  //重くなるので使わずに実装したい
#include <stdlib.h>
#include <string.h>
#include "ctype.h"
#define lengthof(x) sizeof(x) / sizeof(x[0])

const char* file_error_message(file_error_t error) {
    static const char* messages[] = {
        "ok",         "null pointer",  "without permition",
        "type error", "convert error", "over size"};
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

int file_node_excute(file_node_t* fp, int argc, char** argv) {
    if (!fp) {
        return FileErrorNull;
    }
    if ((fp->flags & FileAccessExcute) != FileAccessExcute) {
        return FileErorrPermition;
    }

    if ((fp->flags & FileAccessDevice) == FileAccessDevice) {
        // device fileとして扱う
        const file_device_t* device = (file_device_t*)fp->impl;
        if (!device->write) {
            return FileErrorNull;
        }
        return device->excute(fp->context, argc, argv);

    } else {
        const file_type_t type = fp->flags & FileTypeMask;
        if (type == FileTypeExcute) {
            return ((excute_t)fp->impl)(fp->context, argc, argv);
        }
    }
}

int file_node_write(file_node_t* fp, const char* str) {
    if (!fp) {
        return FileErrorNull;
    }
    if ((fp->flags & FileAccessWrite) != FileAccessWrite) {
        return FileErorrPermition;
    }
    const file_type_t type = fp->flags & FileTypeMask;

    if ((fp->flags & FileAccessDevice) == FileAccessDevice) {
        // device fileとして扱う
        const file_device_t* device = (file_device_t*)fp->impl;
        if (!device->write) {
            return FileErrorNull;
        }
        return device->write(fp->context, str);

    } else {
        switch (type) {
            case FileTypeBool:
                *(bool*)fp->impl = str_to_bool(str);
                return FileErrorNone;
            case FileTypeInt:
                *(int*)fp->impl = atoi(str);
                return FileErrorNone;
            case FileTypeFloat:
                //実装されていない場合はatoiでごまかすと良い
                *(float*)fp->impl = atof(str);
                return FileErrorNone;
            case FileTypeText:
                strncpy(fp->impl, str, fp->size);
                return FileErrorNone;
            default:
                return FileErrorType;
        }
    }
}

int file_node_read(file_node_t* fp, char* str, size_t size) {
    if (!fp) {
        return FileErrorNull;
    }
    if ((fp->flags & FileAccessRead) != FileAccessRead) {
        return FileErorrPermition;
    }
    const file_type_t type = fp->flags & FileTypeMask;

    if ((fp->flags & FileAccessDevice) == FileAccessDevice) {
        // device fileとして扱う
        const file_device_t* device = (file_device_t*)fp->impl;
        if (!device->write) {
            return FileErrorNull;
        }
        return device->read(fp->context, str, size);

    } else {
        return FileErrorType;
        // switch (type) {
        // case FileTypeBool:
        //     if (snprintf(str, size, "%d", *((int_t*)fp->impl)) < 0) {
        //         return FileErrorOverSize;
        //     }
        //     return FileErrorNone;
        // case FileTypeInt:
        //     if (snprintf(str, size, "%d", *((int_t*)fp->impl)) < 0) {
        //         return FileErrorOverSize;
        //     }
        //     return FileErrorNone;
        // case FileTypeFloat:
        //     if (snprintf(str, size, "%f", *((float*)fp->impl)) < 0) {
        //         return FileErrorOverSize;
        //     }
        //     return FileErrorNone;
        // case FileTypeText:
        //     strncpy(str, fp->impl, (size_t)fp->context);
        //     return FileErrorNone;
        // default:
        //     return FileErrorType;
        //}
    }
}
