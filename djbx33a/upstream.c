#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define PRINT_FUNCTION_NAME(func) printf("Function Name: %s\n", #func)

long long getCurrentTimeInMicroseconds() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return (long long)currentTime.tv_sec * 1000000 + currentTime.tv_usec;
}

void calculateFunctionTime(void (*func)(), const char *str , int repeatTime, int len){
    
    long long start_time = getCurrentTimeInMicroseconds();
    for ( ; len>0; len--){
        for(int i=0; i < repeatTime; i++){
            func(str, len);
        }
    }
    long long end_time = getCurrentTimeInMicroseconds();
    long long execution_time = end_time - start_time;
    printf(" esclaped time: %lld us\n", execution_time);
}

void djbx33a_python_ori(const char *str,  int len) {
    unsigned long hash = 5381;
    switch(len) {
        case 7: hash = ((hash << 5) + hash) + *str++; /* fallthrough */
        case 6: hash = ((hash << 5) + hash) + *str++; /* fallthrough */
        case 5: hash = ((hash << 5) + hash) + *str++; /* fallthrough */
        case 4: hash = ((hash << 5) + hash) + *str++; /* fallthrough */
        case 3: hash = ((hash << 5) + hash) + *str++; /* fallthrough */
        case 2: hash = ((hash << 5) + hash) + *str++; /* fallthrough */
        case 1: hash = ((hash << 5) + hash) + *str++; break;
        default:
            break;
    }
}

void djbx33a_opt(const char *str,  int len) {
    unsigned long hash = 5381;
    if (len >= 4){
        hash =
			hash   * 33 * 33 * 33 * 33UL +
			str[0] * 33 * 33 * 33UL +
			str[1] * 33 * 33UL +
			str[2] * 33UL +
			str[3];
        len -= 4;
        str += 4;
    }
    if (len >=2) {
        if (len >2){
            hash = hash * 33 * 33 * 33UL +
                str[0] * 33 * 33UL +
                str[1] * 33UL+
                str[2];
        } else {
            hash = hash * 33 * 33UL +
                str[0] * 33UL + 
                str[1];
        }
    } else if (len !=0 ) {
        hash = hash * 33UL + *str;
    }
}

int main() {
    const char *str ="abcdefg";
    int length = strlen(str);
    PRINT_FUNCTION_NAME(djbx33a_python_ori);
    calculateFunctionTime(djbx33a_python_ori, str, 1000000, length);
    PRINT_FUNCTION_NAME(djbx33a_opt);
    calculateFunctionTime(djbx33a_opt, str, 1000000, length);
}