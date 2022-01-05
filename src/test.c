#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include "../include/test.h"
#include "../include/mem.h"
#include "../include/mem_internals.h"

#define TEST_COUNT 6


struct test_result successful_simple_allocation_10_bytes_in_100_bytes_heap() {
    heap_init(100);
    if (_malloc(10) == NULL) {
        heap_destroy(100);
        return error_result("failed to allocate");
    }


    heap_destroy(100);
    return successful_result();
};

struct test_result successful_changed_content_after_malloc() {
    heap_init(100);
    struct block_header *heap = HEAP_START;
    uint8_t *mem = (_malloc(10));
    mem[0] = 9;

    if (heap->contents[0] != 9) {
        heap_destroy(100);
        return error_result("failed to changed content");
    }

    heap_destroy(100);
    return successful_result();
}


struct test_result successful_initialization_of_100_bytes_heap() {
    heap_init(100);
    struct block_header *heap = HEAP_START;

    if (heap->is_free == false) {
        heap_destroy(100);
        return error_result("heap is not free after init");
    }
    if (heap->capacity.bytes != capacity_from_size((block_size) {100}).bytes) {
        heap_destroy(100);
        return error_result("wrong capacity");
    }
    if (heap->next != NULL) {
        heap_destroy(100);
        return error_result("there is more than one block after initialization");
    }

    heap_destroy(100);
    return successful_result();
};


struct test_result successful_simple_allocation_100_bytes_in_100_bytes_heap() {
    heap_init(100);
    char *arr = (_malloc(100));

    if (arr == NULL) {
        heap_destroy(100);
        return error_result("failed to allocate");
    }

    heap_destroy(100);
    return successful_result();
};

struct test_result successful_allocate_more_mem_than_heap_has() {
    return error_result("not implemented");
}

struct test_result successful_allocation_of_zero_bytes() {
    heap_init(100);
    void *arr = (_malloc(0));
    struct block_header* heap = HEAP_START;
    if (arr == NULL) {
        heap_destroy(100);
        return error_result("failed to allocate");
    }

    heap_destroy(100);
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
                .name = "Successful scenario. Allocate 10 bytes in 100-bytes heap",
                .value = &successful_simple_allocation_10_bytes_in_100_bytes_heap
        },
        {
                .name = "Successful scenario. Allocate 100 bytes in 100-bytes heap",
                .value = &successful_simple_allocation_100_bytes_in_100_bytes_heap
        },
        {
                .name = "Successful scenario. Allocate more than heap has",
                .value = &successful_allocate_more_mem_than_heap_has
        },
        {
                .name = "Successful scenario. Allocate zero bytes",
                .value = &successful_allocation_of_zero_bytes
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
