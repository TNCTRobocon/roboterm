#ifndef __HEADER_GUARD__FS__
#define __HEADER_GUARD__FS__
#include "config.h"
#include "file.h"

/* filesystem_node
 * filesystem_nodeで管理できる要素を登録する。
 * 静的なメモリ配置を可能にする工夫
 */

union fs_node {
    union fs_node* next;  //単方向リスト構築用
    file_node_t node;
    file_device_t device;
    file_link_t link;
};
typedef union fs_node fs_node_t;

struct fs {
    fs_node_t* free;  //利用できるファイルノードの単方向リスト
    size_t used;      //利用しているファイルノード数
    file_node_t* root;  // rootとしてマウントされるファイルノード
    fs_node_t nodes[FILE_NODE_MAX];
};

typedef struct fs fs_t;

int fs_init(fs_t* fs);

// filesystemからnodeをallocateする関数
fs_node_t* fs_allocate(fs_t* fs);
static inline file_node_t* fs_allocate_node(fs_t* fs) {
    return (file_node_t*)fs_allocate(fs);
}
static inline file_device_t* fs_allocate_device(fs_t* fs) {
    return (file_device_t*)fs_allocate(fs);
}
static inline file_link_t* fs_allocate_link(fs_t* fs) {
    return (file_link_t*)fs_allocate(fs);
}

// filesystemにnodeを開放する関数
int fs_release(fs_t* fs, fs_node_t* node);
static inline int fs_release_node(fs_t* fs, file_node_t* node) {
    return fs_release(fs, (fs_node_t*)node);
}
static inline int fs_release_device(fs_t* fs, file_device_t* device) {
    return fs_release(fs, (fs_node_t*)device);
}
static inline int fs_release_link(fs_t* fs, file_link_t* link) {
    return fs_release(fs, (fs_node_t*)link);
}

//ファイルノード使用量
static inline size_t fs_used(const fs_t* fs) {
    return fs->used;
}
static inline size_t fs_total() {
    return FILE_NODE_MAX;
}
static inline size_t fs_free(const fs_t* fs) {
    return FILE_NODE_MAX - fs->used;
}

// ディレクトリ
typedef file_node_t fs_directory_t;
typedef file_node_t fs_file_t;
fs_directory_t* fs_create_directory(fs_t* fs, file_access_t access);
static inline fs_directory_t* fs_directory_create_default(fs_t* fs) {
    return fs_create_directory(fs, FileAccessFull);
}

int fs_directory_insert(fs_t* fs,
                        fs_directory_t* dir,
                        fs_file_t* file,
                        const char* name);
/* directory_iter_t
 * ファイルの列挙に用いる
 */
typedef file_link_t* directory_iter_t;
directory_iter_t directory_iter(fs_directory_t* dir);

// 実行ファイル
fs_file_t* fs_create_excute(fs_t* fs,
                            excute_t excute,
                            void* context,
                            file_access_t access);
static inline fs_file_t* fs_create_excute_default(fs_t* fs, excute_t excute) {
    return fs_create_excute(fs, excute, NULL, FileAccessExcute);
}

#endif