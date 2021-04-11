//
// Created by yb on 4/7/21.
//
#include "alloc.h"

namespace ybSTL {
    char *alloc::start_free = nullptr;
    char *alloc::end_free = nullptr;
    size_t alloc::heap_size = 0;

    freeList *alloc::free_List[EFreeListsNumber] = {
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };

    inline void *alloc::allocate(size_t n) {
        if (n > ESmallObjectBytes)
            return malloc(n);
        size_t index = M_freelist_index(n);
        freeList *list = free_List[index];
        if (list) {
            free_List[index] = list->next;
            return list;
        } else
            return M_refill(M_round_up(n));
    }

    inline void alloc::deallocate(void *p, size_t n) {
        if (n > ESmallObjectBytes) {
            free(p);
            return;
        }
        freeList *q = static_cast<freeList *>(p);
        freeList **my_free_list = free_List + M_freelist_index(n);
        q->next = *my_free_list;
        *my_free_list = q;
    }

    inline void* alloc::reallocate(void *p, size_t old_n, size_t new_n) {
        deallocate(p,old_n);
        p=allocate(new_n);
        return p;
    }
}
