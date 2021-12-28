#include <stdbool.h>
#include <time.h>
#include "./../include/utils.h"


const char* error_log_pattern(){
    static char s[30];
    time_t t = time(NULL);
    size_t count = strftime(s , sizeof(s) - 1, "%a %b %d %T %Z %Y", localtime(&t));
    sprintf(s + count, " error ---");
    return s;
}


const char* info_log_pattern(){
    static char s[30];
    time_t t = time(NULL);
    size_t count = strftime(s , sizeof(s) - 1, "%a %b %d %T %Z %Y", localtime(&t));
    sprintf(s + count, "  info ---");
    return s;
}



