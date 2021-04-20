#include "pool.h"
#include <string.h>
#include "config.h"
pool_t* pool_init(pool_t* p, void* mem, size_t size) {
    if (!p || !mem | size == 0) return NULL;
    p->used = 0;
    p->size = size;
    p->mem = (char*)mem;

    memset(mem, 0x55555555, sizeof(size));
}

void* pool_malloc(pool_t* p, size_t size) {
    //まあ、ポインタのサイズなら、ワード長より大きいはず。
    const size_t alignment = sizeof(void*);

    if (!p || !size) {
        return NULL;
    }

    // アライメント単位で切り上げ
    if (size & (alignment - 1)) {
        size = (size & ~(alignment - 1)) + alignment;
    }

    //空き容量に余裕があるかだけ確認する
    if (p->size - p->used < size || size == 0) {
        return NULL;
    }
    //すすめる
    size_t pos = p->used;
    p->used += size;
    return &p->mem[pos];
}