#ifndef __HEADER__GUARD__SHELL__
#define __HEADER__GUARD__SHELL__
#include <stdbool.h>
#include <stddef.h>

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
/* file_access
 * 疑似ファイルのアクセス権限です
 * 元ネタはLinuxのパーミッションですが、ユーザーやグループの概念が存在しないため、単一のアクセス権限しか持ちません
 */

enum file_access {
    // 基本的なフラグ
    FileAccessNone = 0x00,
    FileAccessExcute = 0x01,
    FileAccessRead = 0x02,
    FileAccessWrite = 0x04,
    FileAccessWriteAndRead = 0x06,
    FileAccessFull = 0x07,
    FileAccessMask = 0x0F,

};
typedef enum file_access file_access_t;

/* file_special
 * ファイルの実装方法について
 */

enum file_special {
    FileSpecialVariable = 0x10,  // implをそのまま利用する
    FileSpecialPointer = 0x20,   // implをポインタとして利用する
    FileSpecialDevice = 0x30,  // デバイスファイルとして利用する
    FileSpecialMask = 0xF0,
};

/* file_type
 * 疑似ファイルもといポインターをどのように扱うか決める列挙型です
 */

enum file_type {
    FileTypeExcute = 0x0100,
    FileTypeDirectory = 0x0200,
    FileTypeLink = 0x0300,  // softlinkに近い
    FileTypeBool = 0x0400,
    FileTypeInt = 0x0500,
    FileTypeFloat = 0x0600,
    FileTypeText = 0x0700,
    /* Reserved */
    FileTypeMask = 0xff00,
};

typedef enum file_type file_type_t;

typedef enum file_special file_special_t;
/* file_flag_t
 * file_typeとfile_accessをまとめたもの
 */
typedef unsigned int file_flag_t;

/* file_node
 * ファイルの属性値及び実体を保持する
 * 元ネタはinodeである。
 */
struct file_node {
    file_flag_t flags;
    void* impl;  //実体
    union {
        // contextはデバイスファイルもしくは実行可能なファイルの場合のみ、ユーザが利用できる。
        void* context;
        // sizeは文字列型のときに利用する
        size_t size;
        // parentはディレクトリの場合に利用する
        struct file_node* parent;
    };
};

typedef struct file_node file_node_t;

const char* file_error_message(file_error_t);
// テキストを介して入出力を行う
typedef int (*excute_t)(void* context, int argc, char** argv);
int file_node_excute(file_node_t*, int argc, char** argv);
int file_node_write(file_node_t*, const char* str);
int file_node_read(file_node_t*, char*, size_t);

/* file_device
 * デバイスファイルとして扱う場合の実体
 * open/closeはずっとopenされていると考えられるため、省略されている。
 * また、probeはコンセプトのため、省略されている。
 */
struct file_device {
    int (*excute)(void* context, int argc, char** argv);
    int (*write)(void* context, const char* str);
    int (*read)(void* context, char* str, size_t size);
};
typedef struct file_device file_device_t;

/* file_link
 * ファイル名とファイルノードの単方向リストを構築する
 */
struct file_link {
    struct file_link* next;  //単方向リストを構築する
    const char* name;
    file_node_t* node;
};

typedef struct file_link file_link_t;

#endif