//
// Created by yb on 4/6/21.
//

#ifndef LEARNCPP_ALLOC_H
#define LEARNCPP_ALLOC_H

#include <cstdio>
#include <memory>

namespace ybSTL {
    union freeList {
        union freeList *next;
        char data[1];
    };
    //不同内存范围的上调大小
    enum {
        EAlign128 = 8
    };
    // 小对象的内存大小
    enum {
        ESmallObjectBytes = 128
    };

    // free lists 个数
    enum {
        EFreeListsNumber = 16
    };

    class alloc {
    private:
        static char *start_free;
        static char *end_free;
        static size_t heap_size;

        static freeList *free_List[EFreeListsNumber];
    public:
        static void *allocate(size_t n);

        static void deallocate(void *p, size_t n);

        static void *reallocate(void *p, size_t old_n, size_t new_n);

    private:

        static size_t M_round_up(size_t bytes);

        static size_t M_freelist_index(size_t bytes);

        static void *M_refill(size_t n);

        static char *M_chunk_alloc(size_t size, size_t &nobj);
    };


    inline size_t alloc::M_round_up(size_t bytes) {
        return (bytes + EAlign128 - 1) & ~(EAlign128 - 1);
    }

    inline size_t alloc::M_freelist_index(size_t bytes) {
        return (((bytes) + EAlign128 - 1) / EAlign128 - 1);
    }

    void *alloc::M_refill(size_t n) {
        size_t nobjs = 20;
        char *chunk = M_chunk_alloc(n, nobjs);

        freeList **my_free_list = 0;
        freeList *result = 0;
        freeList *current_free = 0, *next_free = 0;
        if (nobjs == 1) {
            return chunk;
        } else {
            my_free_list = free_List + M_freelist_index(n);
            result = (freeList *) chunk;
            next_free = (freeList *) (chunk + n);
            for (int i = 1;; ++i) {
                current_free = next_free;
                *my_free_list = next_free = (freeList *) (chunk + n);
                if (nobjs - 1 == i) {
                    current_free->next = 0;
                    break;
                } else
                    current_free->next = next_free;
            }
            return result;
        }
    }

    char *alloc::M_chunk_alloc(size_t size, size_t &nobj) {
        char *result;
        size_t need_bytes = size * nobj;
        size_t pool_bytes = end_free - start_free;
        if (pool_bytes > need_bytes) {
            result = start_free;
            start_free += need_bytes;
            return result;
        } else if (pool_bytes >= size) {
            nobj = pool_bytes / size;
            need_bytes = size * nobj;
            result = start_free;
            start_free += need_bytes;
            return result;
        } else {
            if (pool_bytes > 0) {
                freeList *my_free_list = free_List[M_freelist_index(pool_bytes)];
                ((freeList *) start_free)->next = my_free_list;
                my_free_list = (freeList *) start_free;
            }
            size_t bytes_to_get = (need_bytes << 1) + M_round_up(heap_size >> 4);//内存池空间不够向malloc申请的字节数
            start_free = (char *) std::malloc(bytes_to_get);
            if (!start_free) { // heap 空间也不够
                freeList *my_free_list, *p;
                // 试着查找有无未用区块，且区块足够大的 free list
                for (size_t i = size; i <= ESmallObjectBytes; i += EAlign128) {
                    my_free_list = free_List[M_freelist_index(i)];
                    p = my_free_list;
                    if (p) {
                        my_free_list = p->next;
                        start_free = (char *) p;
                        end_free = start_free + i;
                        return M_chunk_alloc(size, nobj);
                    }
                }
                std::printf("out of memory");
                end_free = nullptr;
                throw std::bad_alloc();
            }
            end_free = start_free + bytes_to_get;
            heap_size += bytes_to_get;
            return M_chunk_alloc(size, nobj);
        }
    }
}

#endif //LEARNCPP_ALLOC_H
