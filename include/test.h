#ifndef GITHUB_TEST_H
#define GITHUB_TEST_H

 enum status{
    OK,
    ERROR
};

struct test_result{
    enum status status;
    char* description;
};


struct test_result successful_result();
struct test_result error_result(char* text);

typedef struct test_result (test) ();

struct test_info{
    char* name;
    test* value;
};


static struct test_result successful_simple_allocation_10_bytes_in_100_bytes_heap();
void run_all();


#endif //GITHUB_TEST_H
