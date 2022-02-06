#ifndef GITHUB_TEST_H
#define GITHUB_TEST_H


struct test_result successful_result();
struct test_result error_result(char* text);

void run_all();
#endif //GITHUB_TEST_H
