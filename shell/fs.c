#include "fs.h"
#include <stddef.h>

int fs_init(fs_t* fs) {
    //前提条件
    if (fs == NULL) {
        return -1;
    }

    // 単方向リストの初期化
    fs_node_t* nodes = fs->nodes;
    fs->free = &nodes[0];
    fs->used = 0;
    for (size_t i = 0; i < FILE_NODE_MAX - 1; i++) {
        nodes[i].next = &nodes[i + 1];
    }
    nodes[FILE_NODE_MAX - 1].next = NULL;

    // rootの作成
    fs->root = fs_directory_create_default(fs);

    return 0;
}

fs_node_t* fs_allocate(fs_t* fs) {
    //前提条件
    if (fs == NULL) return NULL;
    if (fs->free == NULL) return NULL;

    // stackとしてpop O(1)
    fs_node_t* top = fs->free;
    fs->free = top->next;
    //カウント
    fs->used++;
    return top;
}

int fs_release(fs_t* fs, fs_node_t* node) {
    //前提条件
    if (fs == NULL) return -1;
    if (node == NULL) return -2;
    fs_node_t* fs_node = (fs_node_t*)node;
    // stackとしてpush O(1)
    fs_node->next = fs->free;
    fs->free = fs_node;
    //カウント
    fs->used--;
    return 0;
}

fs_directory_t* fs_create_directory(fs_t* fs, file_access_t access) {
    if (fs == NULL) return NULL;
    // file_nodeを初期化する
    file_node_t* node = (file_node_t*)fs_allocate_node(fs);
    if (node == NULL) return NULL;
    node->flags = FileTypeDirectory | access;
    node->impl = NULL;
    node->parent = NULL;
    return node;
}

int fs_directory_insert(fs_t* fs,
                        fs_directory_t* dir,
                        fs_file_t* file,
                        const char* name) {
    //前提条件
    if (fs == NULL || dir == NULL || file == NULL || name == NULL) return -1;
    //もし挿入したファイルがディレクトリであれば親ディレクトリを登録する
    if ((file->flags & FileTypeMask) == FileTypeDirectory) {
        //親を複数持つことはできない
        if (file->parent != NULL) {
            return -2;
        }
        file->parent = dir;
    }

    // linkを確保
    file_link_t* link = fs_allocate_link(fs);
    if (link == NULL) return -3;

    // linkをpushする
    link->name = name;
    link->node = file;
    link->next = (file_link_t*)dir->impl;
    dir->impl = link;
}

directory_iter_t directory_iter(fs_directory_t* dir) {
    if (dir == NULL) return NULL;
    return (directory_iter_t)dir->impl;
}

fs_file_t* fs_create_excute(fs_t* fs,
                            excute_t excute,
                            void* context,
                            file_access_t access) {
    if (fs == NULL || excute == NULL) {
        return NULL;
    }
    // allocate file
    file_node_t* file = fs_allocate_node(fs);
    if (file == NULL) {
        return NULL;
    }
    // initialize file
    file->flags = FileTypeExcute | access;
    file->impl = excute;
    file->context = context;
    return file;
}