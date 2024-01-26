#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#define PRINT_FUNCTION_NAME(func) printf("Function Name: %s\n", #func)

void djbx33a_opt_php(char const *str,  int len) {
    unsigned long hash = 5381;
    for (; len >= 7; len -= 7, str += 7) {
        hash =
			hash   * 33 * 33 * 33 * 33UL +
			str[0] * 33 * 33 * 33UL +
			str[1] * 33 * 33UL +
			str[2] * 33UL +
			str[3];
        hash =
			hash   * 33 * 33 * 33+
			str[4] * 33 * 33+
			str[5] * 33+
			str[6];
    }
    // printf("%lu from OPT PHP Hash\n", hash);
}

void djbx33a_opt_yyh(char const *str,  int len) {
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
        
    // printf("%lu from yyh Hash\n", hash);
}

void djbx33a_opt_yyh2(char const *str,  int len) {
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
    switch(len) {
        case 3: hash = hash * 33 * 33 * 33UL +
                str[0] * 33 * 33UL +
                str[1] * 33UL+
                str[2];
        case 2: hash = hash * 33 * 33UL +
                str[0] * 33UL + 
                str[1];
        case 1: hash = hash * 33UL + *str;
        default: break;
    }
    // printf("%lu from yyh2 Hash\n", hash);
}

void djbx33a_opt_python(char const *str,  int len) {
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
    // printf("%lu from OPT PYTHON Hash\n", hash);
}

void __attribute__((optimize("unroll-loops"))) djbx33a_ori2(char const *str,  int len) { 
    unsigned long hash = 5381;
    for (int i = 0; i < len; i++) { 
        hash = hash *33 + (unsigned long) str[i];
    }
}

void djbx33a_ori(char const *str,  int len) { 
    unsigned long hash = 5381;
    for (int i = 0; i < len; i++) { 
        hash = hash *33 + (unsigned long) str[i];
    }
}

long long getCurrentTimeInMicroseconds() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return (long long)currentTime.tv_sec * 1000000 + currentTime.tv_usec;
}

void calculateFunctionTime(void (*func)(), int repeatTime, int len){
    const char* str ="abcdefg";
    long long start_time = getCurrentTimeInMicroseconds();
    for(int i=0; i < repeatTime; i++){
        func(str, len);
    }
    long long end_time = getCurrentTimeInMicroseconds();
    long long execution_time = end_time - start_time;
    printf(" esclaped time: %lld us\n", execution_time);
}

int main() {
    // PRINT_FUNCTION_NAME(djbx33a_opt_php);
    // calculateFunctionTime(djbx33a_opt_php, 1000000, 7);
    PRINT_FUNCTION_NAME(djbx33a_opt_python);
    calculateFunctionTime(djbx33a_opt_python, 1000000, 7);
    PRINT_FUNCTION_NAME(djbx33a_opt_yyh);
    calculateFunctionTime(djbx33a_opt_yyh, 1000000, 7);
    PRINT_FUNCTION_NAME(djbx33a_opt_yyh2);
    calculateFunctionTime(djbx33a_opt_yyh2, 1000000, 7);
    PRINT_FUNCTION_NAME(djbx33a_ori);
    calculateFunctionTime(djbx33a_ori, 1000000, 7);
    PRINT_FUNCTION_NAME(djbx33a_ori2);
    calculateFunctionTime(djbx33a_ori2, 1000000, 7);
}