#include <stdio.h>
#include <stdbool.h>

bool printable_buf(const void * buf, int len){
    // check if elements of buffer are bytes <32-126>
    for(int i = 0; i < len; i++){
        if(*((char *)buf + i) < 32 || *((char *)buf + i) > 126){
            printf("buf[%d] = %d\n", i, *((char *)buf + i));
            return false;
        }
    }
    return true;
}

bool printable_str(const char * buf){
    // string of not zero's, ended with zero (this 0 isn't a part of the string)
    for(int i = 0; buf[i] != 0; i++){
        if(buf[i] < 32 || buf[i] > 126){
            printf("buf[%d] = %d\n", i, buf[i]);
            return false;
        }
    }
    return true;
}

int main(void){
    // test
    char buf[] = "Hello, World!";

    if(printable_buf(buf, sizeof(buf)/sizeof(char) - 1))
        printf("buf is printable\n");
    else
        printf("buf is not printable\n");

    printf("for string: ");
    if(printable_str(buf))
        printf("buf is printable\n");
    else
        printf("buf is not printable\n");
}
