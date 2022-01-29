#define _DEFAULT_SOURCE

#include "../include/mem_internals.h"
#include "../include/mem.h"
#include "../include/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void debug_block(struct block_header *b, const char *fmt, ...);

void debug(const char *fmt, ...);

extern inline block_size
size_from_capacity(block_capacity
cap);

extern inline block_capacity
capacity_from_size(block_size
sz);

static bool block_is_big_enough(size_t query, struct block_header *block) { return block->capacity.bytes >= query; }

static size_t pages_count(size_t mem) { return mem / getpagesize() + ((mem % getpagesize()) > 0); }

static size_t round_pages(size_t mem) { return getpagesize() * pages_count(mem); }

static void block_init(void *restrict addr, block_size block_sz, void *restrict next) {
    *((struct block_header *) addr) = (struct block_header) {
            .next = next,
            .capacity = capacity_from_size(block_sz),
            .is_free = true
    };
}

static size_t region_actual_size(size_t query) { return size_max(round_pages(query), REGION_MIN_SIZE); }

extern inline bool

region_is_invalid(const struct region *r);


static void *map_pages(void const *addr, size_t length, int additional_flags) {
    return mmap((void *) addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | additional_flags, -1, 0);
}

/*  аллоцировkать регион памяти и инициализировать его блоком */
static struct region alloc_region(void const *addr, size_t query) {
    size_t region_size = region_actual_size(query + offsetof(struct block_header, contents));
    void *reg = map_pages(addr, region_size, MAP_FIXED);
    bool is_extends = true;
    if (reg == MAP_FAILED) {
        is_extends = false;
        reg = map_pages(addr, region_size, 0);
        if (reg == MAP_FAILED) return REGION_INVALID;
    }

    block_init(reg, (block_size) {region_size}, NULL);

    return (struct region) {
            .size = region_size,
            .addr = reg,
            .extends = is_extends
    };
}

static void *block_after(struct block_header const *block);

void *heap_init(size_t initial) {
    const struct region region = alloc_region(HEAP_START, initial);
    if (region_is_invalid(&region)) return NULL;
    return region.addr;
}

void heap_destroy() {
    struct block_header* heap = HEAP_START;


    if (munmap(HEAP_START, size_from_capacity(heap->capacity).bytes) != 0) {
        fprintf(stderr, "Failed to destroy heap\n");
    }
}


#define BLOCK_MIN_CAPACITY 24

/*  --- Разделение блоков (если найденный свободный блок слишком большой )--- */

static bool block_splittable(struct block_header *restrict block, size_t query) {
    return block->is_free && query + offsetof(struct block_header, contents) + BLOCK_MIN_CAPACITY <= block->capacity.bytes;
}

static void split(struct block_header *block, size_t query) {
    block_capacity first_block_cap = (block_capacity) {size_max(query, BLOCK_MIN_CAPACITY)};
    block_size next_block_size = (block_size){
            size_from_capacity(block->capacity).bytes - size_from_capacity(first_block_cap).bytes};
    struct block_header* third_block = block->next;

    block->capacity = first_block_cap;
    struct block_header* next_block = block_after(block);

    block->next = next_block;

    block_init(block->next, next_block_size, third_block);
}

static bool split_if_too_big(struct block_header *block, size_t query) {
    if (block_splittable(block, query)) {
        split(block, query);
        return true;
    } else {
        return false;
    }
}

/*  --- Слияние соседних свободных блоков --- */

static void *block_after(struct block_header const *block) {
    return (void *) (block->contents + block->capacity.bytes);
}

static bool blocks_continuous(
        struct block_header const *fst,
        struct block_header const *snd) {
    return (void *) snd == block_after(fst);
}

static bool mergeable(struct block_header const *restrict fst, struct block_header const *restrict snd) {
    return fst->is_free && snd->is_free && blocks_continuous(fst, snd);
}

static bool try_merge_with_next(struct block_header *block) {
    struct block_header *next_block = block->next;
    if (next_block != NULL && mergeable(block, next_block)) {
        block->capacity.bytes += size_from_capacity(next_block->capacity).bytes;
        block->next = next_block->next;
        return true;
    } else {
        return false;
    }
}


/*  --- ... ecли размера кучи хватает --- */

struct block_search_result {
    enum {
        BSR_FOUND_GOOD_BLOCK, BSR_REACHED_END_NOT_FOUND, BSR_CORRUPTED
    } type;
    struct block_header *block;
};

static struct block_search_result find_good_or_last(struct block_header *restrict block, size_t sz) {
    struct block_header *cur_block = block;

    while (1) {
        while (try_merge_with_next(cur_block));
        if (cur_block->is_free && block_is_big_enough(sz, cur_block)) {
            return (struct block_search_result) {
                    .type = BSR_FOUND_GOOD_BLOCK,
                    .block = cur_block
            };
        }

        if (cur_block->next == NULL) {
            return (struct block_search_result) {
                    .type = BSR_REACHED_END_NOT_FOUND,
                    .block = cur_block
            };
        }
        cur_block = cur_block->next;
    }
}

/*  Попробовать выделить память в куче начиная с блока `block` не пытаясь расширить кучу
 Можно переиспользовать как только кучу расширили. */
static struct block_search_result try_memalloc_existing(size_t query, struct block_header *block) {
    struct block_search_result result = find_good_or_last(block, query);
    switch (result.type) {
        case BSR_REACHED_END_NOT_FOUND: {
            break;
        }
        case BSR_FOUND_GOOD_BLOCK: {
            split_if_too_big(result.block, query);
            result.block->is_free = false;
            break;
        }
        case BSR_CORRUPTED: {
            fprintf(stderr, "memory is corrupted\n");
            exit(EXIT_FAILURE);
        }
        default: {
            fprintf(stderr, "retrieved block with unknown type");
            exit(EXIT_FAILURE);
        }
    }

    return result;
}


static struct block_header *grow_heap(struct block_header *restrict last, size_t query) {
    const struct region region = alloc_region(block_after(last), query);
    if (region_is_invalid(&region)) {
        return NULL;
    } else {
        block_init(region.addr, (block_size) {region.size}, NULL);
        last->next = region.addr;
        if (try_merge_with_next(last)){
            return last;
        } else {
            return region.addr;
        }
    }
}

/*  Реализует основную логику malloc и возвращает заголовок выделенного блока */
static struct block_header *memalloc(size_t query, struct block_header *heap_start) {
    struct block_search_result result = try_memalloc_existing(query, heap_start);
    if (result.type == BSR_REACHED_END_NOT_FOUND){
        grow_heap(result.block, size_from_capacity((block_capacity) {query}).bytes);
        result = try_memalloc_existing(query, heap_start);
    }

    switch (result.type) {
        case BSR_FOUND_GOOD_BLOCK: {
            return result.block;
        }
        case BSR_CORRUPTED: {
            fprintf(stderr, "memory is corrupted\n");
            return NULL;
        }
        default: {
            fprintf(stderr, "retrieved block with unknown type");
            return NULL;
        }
    }

}

void *_malloc(size_t query) {
    struct block_header *const addr = memalloc(query, HEAP_START);
    if (addr) return addr->contents;
    else return NULL;
}

static struct block_header *block_get_header(void *contents) {
    return (struct block_header *) (((uint8_t *) contents) - offsetof(
    struct block_header, contents));
}

void _free(void *mem) {
    if (!mem) return;
    struct block_header *header = block_get_header(mem);
    header->is_free = true;
    while (try_merge_with_next(header));
}
