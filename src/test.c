#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include "../include/test.h"
#include "../include/mem.h"
#include "../include/mem_internals.h"

#define TEST_COUNT 14


struct test_result successful_simple_allocation_25_bytes_in_100_bytes_heap() {
    size_t heap_size = 100;
    heap_init(heap_size);
    struct block_header *heap = HEAP_START;
    if (_malloc(25) == NULL) {
        heap_destroy(heap_size);
        return error_result("failed to allocate");
    }

    if (heap->capacity.bytes != 25 || heap->is_free != false) {
        heap_destroy(heap_size);
        return error_result("invalid first block");
    }

    if (heap->next->capacity.bytes != 41 || heap->next->is_free != true) {
        heap_destroy(heap_size);
        return error_result("invalid second block");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_changed_content_after_malloc() {
    size_t heap_size = 100;
    heap_init(heap_size);
    struct block_header *heap = HEAP_START;
    uint8_t *mem = (_malloc(25));
    mem[0] = 9;

    if (heap->contents[0] != 9) {
        heap_destroy(heap_size);
        return error_result("failed to changed content");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_changed_content_after_second_malloc() {
    size_t heap_size = 200;
    heap_init(heap_size);
    struct block_header *heap = HEAP_START;
    _malloc(25);
    uint8_t *mem = (_malloc(26));
    mem[0] = 9;

    if (heap->next->contents[0] != 9) {
        heap_destroy(heap_size);
        return error_result("failed to changed content");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_allocate_as_much_as_cheap_has() {
    size_t heap_size = 100;
    heap_init(heap_size);
    if (_malloc(42) == NULL) {
        heap_destroy(heap_size);
        return error_result("failed to allocate");
    };

    heap_destroy(heap_size);
    return successful_result();
}


struct test_result successful_initialization_of_100_bytes_heap() {
    size_t heap_size = 100;
    heap_init(heap_size);
    struct block_header *heap = HEAP_START;

    if (heap->is_free == false) {
        heap_destroy(heap_size);
        return error_result("heap is not free after init");
    }
    if (heap->capacity.bytes != capacity_from_size((block_size) {100}).bytes) {
        heap_destroy(heap_size);
        return error_result("wrong capacity");
    }
    if (heap->next != NULL) {
        heap_destroy(heap_size);
        return error_result("there is more than one block after initialization");
    }

    heap_destroy(heap_size);
    return successful_result();
}


struct test_result successful_simple_allocation_25_and_26_bytes_in_200_bytes_heap() {
    size_t heap_size = 200;
    heap_init(heap_size);
    struct block_header *heap = HEAP_START;

    _malloc(25);
    if (_malloc(26) == NULL) {
        heap_destroy(heap_size);
        return error_result("failed to allocate 26 bytes");
    }

    if (heap->capacity.bytes != 25 || heap->is_free != false) {
        heap_destroy(heap_size);
        return error_result("invalid first block");
    }

    if (heap->next->capacity.bytes != 26 || heap->next->is_free != false) {
        heap_destroy(heap_size);
        return error_result("invalid second block");
    }

    if (heap->next->next->capacity.bytes != 98 || heap->next->next->is_free != true) {
        heap_destroy(heap_size);
        return error_result("invalid third block");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_allocate_more_mem_than_heap_has_with_not_sequentially_regions() {
    size_t heap_size = 100;
    struct block_header *heap = HEAP_START;
    heap_init(heap_size);
    if (_malloc(100) == NULL) {
        heap_destroy(heap_size);
        return error_result("failed to allocate");
    };

    if (heap->capacity.bytes != 83) {
        heap_destroy(heap_size);
        return error_result("invalid capacity in first block");
    }

    if (heap->next->capacity.bytes != 100) {
        heap_destroy(heap_size);
        return error_result("invalid capacity in second block");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_changing_content_in_next_region() {
    size_t heap_size = 100;
    struct block_header *heap = HEAP_START;
    heap_init(heap_size);
    uint8_t *mem = (_malloc(100));
    mem[0] = 9;

    if (heap->next->contents[0] != 9) {
        heap_destroy(heap_size);
        return error_result("failed to changed content");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_merging_two_sequential_region() {
    size_t heap_size = REGION_MIN_SIZE;
    struct block_header *heap = HEAP_START;
    heap_init(heap_size);
    _malloc(REGION_MIN_SIZE);

    if (heap->capacity.bytes != REGION_MIN_SIZE ) {
        heap_destroy(heap_size);
        return error_result("wrong capacity in first block");
    }

    if (heap->next->capacity.bytes != 8175 ) {
        heap_destroy(heap_size);
        return error_result("wrong capacity in second block");
    }

    if (heap->is_free) {
        heap_destroy(heap_size);
        return error_result("merged block is free");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_block_free() {
    size_t heap_size = 100;
    struct block_header *heap = HEAP_START;
    heap_init(heap_size);
    void *mem = _malloc(10);
    _free(mem);

    if (heap->is_free == false) {
        heap_destroy(heap_size);
        return error_result("block is not free");
    }

    if (heap->capacity.bytes != 83) {
        heap_destroy(heap_size);
        return error_result("wrong capacity after free");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_free_three_sequential_blocks() {
    size_t heap_size = 200;
    struct block_header *heap = HEAP_START;
    heap_init(heap_size);
    void *first = _malloc(24);
    void *second = _malloc(25);
    void *third = _malloc(26);

    _free(third);
    _free(second);
    _free(first);

    if (heap->is_free == false) {
        heap_destroy(heap_size);
        return error_result("block is not free");
    }

    if (heap->capacity.bytes != 183) {
        heap_destroy(heap_size);
        return error_result("wrong capacity after free");
    }

    heap_destroy(heap_size);
    return successful_result();
}


struct test_result successful_free_block_with_not_sequential_next_region() {
    size_t heap_size = 200;
    struct block_header *heap = HEAP_START;
    heap_init(heap_size);
    void *first = _malloc(25);
    void *second = _malloc(1000);

    _free(second);
    _free(first);

    if (heap->is_free == false) {
        heap_destroy(heap_size);
        return error_result("first block is not free");
    }

    if (heap->next->is_free == false) {
        heap_destroy(heap_size);
        return error_result("second block is not free");
    }

    if (heap->capacity.bytes != 183) {
        heap_destroy(heap_size);
        return error_result("wrong capacity after free in first block");
    }

    if (heap->next->capacity.bytes != 1000) {
        heap_destroy(heap_size);
        return error_result("wrong capacity after free in next block");
    }

    heap_destroy(heap_size);
    return successful_result();
}


struct test_result successful_allocation_of_zero_bytes() {
    size_t heap_size = 200;
    heap_init(heap_size);

    if (_malloc(0) == NULL) {
        heap_destroy(heap_size);
        return error_result("failed to allocate");
    }

    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_allocate_less_than_min_block_size() {
    size_t heap_size = 100;
    heap_init(heap_size);
    _malloc(16);
    struct block_header *heap = HEAP_START;
    if (heap->capacity.bytes == 83) {
        heap_destroy(heap_size);
        return error_result("wrong capacity");
    }
    heap_destroy(heap_size);
    return successful_result();
}

struct test_result successful_result() {
    return (struct test_result) {
            .status = OK,
            .description = ""
    };
}

struct test_result error_result(char *text) {
    return (struct test_result) {
            .status = ERROR,
            .description = text
    };
}


struct test_info tests[TEST_COUNT] = {
        {
                .name = "Successful scenario. Init 100-bytes heap",
                .value = &successful_initialization_of_100_bytes_heap
        },
        {
                .name = "Successful scenario. Changed content of allocated memory",
                .value = &successful_changed_content_after_malloc
        },
        {
                .name = "Successful scenario. Changed content of allocated second block",
                .value = &successful_changed_content_after_second_malloc
        },
        {
                .name = "Successful scenario. Allocate 25 bytes in 100-bytes heap",
                .value = &successful_simple_allocation_25_bytes_in_100_bytes_heap
        },
        {
                .name = "Successful scenario. Allocate 25 and 26 bytes in 200-bytes heap",
                .value = &successful_simple_allocation_25_and_26_bytes_in_200_bytes_heap
        },
        {
                .name = "Successful scenario. Allocate more than heap has. Regions are located not sequentially",
                .value = &successful_allocate_more_mem_than_heap_has_with_not_sequentially_regions
        },
        {
                .name = "Successful scenario. Allocate more than heap has. Regions are located not sequentially",
                .value = &successful_changing_content_in_next_region
        },
        {
                .name = "Successful scenario. Merging two regions, which are located sequentially",
                .value = &successful_merging_two_sequential_region
        },
        {
                .name = "Successful scenario. Allocate as much as heap has",
                .value = &successful_allocate_as_much_as_cheap_has
        },
        {
                .name = "Successful scenario. Allocate zero bytes",
                .value = &successful_allocation_of_zero_bytes
        },
        {
                .name="Successful scenario. Make block free after allocation",
                .value = &successful_block_free
        },
        {
                .name="Successful scenario. Free three sequential blocks",
                .value = &successful_free_three_sequential_blocks
        },

        {
                .name="Successful scenario. Free block with next no sequential region",
                .value = &successful_free_block_with_not_sequential_next_region
        },
        {
                .name="Successful scenario. Allocate less than MIN_BLOCK_SIZE",
                .value = &successful_allocate_less_than_min_block_size
        }
};

void run_all() {
    int statistic[2] = {[OK] = 0, [ERROR] = 0};

    for (size_t i = 0; i < TEST_COUNT; i++) {
        struct test_result res = tests[i].value();
        statistic[res.status]++;
        if (res.status == OK) {
            fprintf(stdout, "Test %zu - \"%s\" is passed\n", i, tests[i].name);
        } else {
            fprintf(stderr, "Test %zu - \"%s\" failed:  %s\n", i, tests[i].name, res.description);
        }
    }
    fprintf(stdout, "------------------------------------------------------------------------------------\n");
    fprintf(stdout, "Total - %d: passed - %d, failed - %d\n", TEST_COUNT, statistic[OK], statistic[ERROR]);
}


int main(){
    run_all();
}