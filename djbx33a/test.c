#include <immintrin.h>
#include <emmintrin.h>
#include <stdint.h>
#include <stdio.h>

void djbx33a_avx512(const char *str, size_t len){

    __m512i vectorizedConst = _mm512_set_epi64(
        33*33*33*33*33*33*33UL,
        33*33*33*33*33*33UL,
        33*33*33*33*33UL,
        33*33*33*33UL,
        33*33*33UL,
        33*33UL,
        33UL,
        1UL
    );

    unsigned long hash=5381;
    for (; len >= 8; len -= 8, str += 8) {
        // 向量化字符串
        __m512i vectorizedStr = _mm512_set_epi64(
            (uint64_t)str[0],
            (uint64_t)str[1],
            (uint64_t)str[2],
            (uint64_t)str[3],
            (uint64_t)str[4],
            (uint64_t)str[5],
            (uint64_t)str[6],
            (uint64_t)str[7]
        );
        __m512i result1 = _mm512_mullo_epi64(vectorizedStr, vectorizedConst);
        // Accumulate the elements of the result vector
        unsigned long sum = _mm512_reduce_add_epi64(result1);
        hash = sum + hash*33*33*33*33*33*33*33*33UL;
    }

    // printf("%lu from djbx33a_avx512 Hash\n", hash);
}

void djbx33a_avx256(const char *str, size_t len){

    __m256i vectorizedConst = _mm256_set_epi64x(
        33*33*33UL,
        33*33UL,
        33UL,
        1UL
    );

    unsigned long hash=5381;
    for (; len >= 4; len -= 4, str += 4) {
        // 向量化字符串
        __m256i vectorizedStr = _mm256_set_epi64x(
            (uint64_t)str[0],
            (uint64_t)str[1],
            (uint64_t)str[2],
            (uint64_t)str[3]
        );

        __m256i result1 = _mm256_mul_epu32(vectorizedStr, vectorizedConst);
        // Accumulate the elements of the result vector
        unsigned long sum = 0;
        sum += _mm256_extract_epi64(result1, 0);
        sum += _mm256_extract_epi64(result1, 1);
        sum += _mm256_extract_epi64(result1, 2);
        sum += _mm256_extract_epi64(result1, 3);
        
        hash = sum + hash*33*33*33*33UL;
    }

    printf("%lu from djbx33a_avx256 Hash\n", hash);
}

void djbx33a_avx256_2(const char *str, size_t len){

    __m256i vectorizedConst = _mm256_set_epi64x(
        33*33*33UL,
        33*33UL,
        33UL,
        1UL
    );
    __m256i mul_result1;
	__m256i mul_result2;

    unsigned long hash=5381;
    for (; len >= 8; len -= 8, str += 8) {
        // 向量化字符串
        __m256i vectorizedStr1 = _mm256_set_epi64x(
            (uint64_t)str[0],
            (uint64_t)str[1],
            (uint64_t)str[2],
            (uint64_t)str[3]
        );
		__m256i vectorizedStr2 = _mm256_set_epi64x(
            (uint64_t)str[4],
            (uint64_t)str[5],
            (uint64_t)str[6],
            (uint64_t)str[7]
        );

        mul_result1 = _mm256_mul_epu32(vectorizedStr1, vectorizedConst);
		mul_result2 = _mm256_mul_epu32(vectorizedStr2, vectorizedConst);
        // Accumulate the elements of the result vector
        hash = hash*33*33*33*33UL;
        hash += _mm256_extract_epi64(mul_result1, 0) + _mm256_extract_epi64(mul_result1, 1) + _mm256_extract_epi64(mul_result1, 2) + _mm256_extract_epi64(mul_result1, 3);
        hash = hash*33*33*33*33UL;
		hash += _mm256_extract_epi64(mul_result2, 0) + _mm256_extract_epi64(mul_result2, 1) + _mm256_extract_epi64(mul_result2, 2) + _mm256_extract_epi64(mul_result2, 3);
    }

    // printf("%lu from djbx33a_avx256_2 Hash\n", hash);
}

void djbx33a_sse(const char *str, size_t len) {
    __m128i vectorizedConst = _mm_set_epi32(
        (unsigned int)33 * 33 * 33, 
        (unsigned int)33 * 33,
        (unsigned int)33,
        (unsigned int)1);

    unsigned long hash = 5381;
    unsigned int elements[4];

    for (; len >= 4; len -= 4, str += 4) {
        // 向量化字符串
        __m128i vectorizedStr = _mm_set_epi32(
            (unsigned int)str[0],
            (unsigned int)str[1],
            (unsigned int)str[2],
            (unsigned int)str[3]
        );

        __m128i result1 = _mm_mullo_epi32(vectorizedStr, vectorizedConst);
        // Accumulate the elements of the result vector
        _mm_store_si128((__m128i*)elements, result1);
        unsigned long sum = elements[0] + elements[1] + elements[2] + elements[3];
        hash = sum + hash * 33 * 33 * 33 * 33;
    }

    // printf("%lu from djbx33a_sse Hash\n", hash);
}

void djbx33a_opt_php(char  const *str,  int len) {
    unsigned long hash = 5381;
    for (; len >= 8; len -= 8, str += 8) {
        hash =
			hash   * 33 * 33 * 33 * 33UL +
			str[0] * 33 * 33 * 33UL +
			str[1] * 33 * 33UL +
			str[2] * 33UL +
			str[3];
        hash =
			hash   * 33 * 33 * 33 * 33UL +
			str[4] * 33 * 33 * 33UL +
			str[5] * 33 * 33UL +
			str[6] * 33UL +
			str[7];
    }
    // printf("%lu from OPT PHP Hash\n", hash);
}

void djbx33a_opt_php2(const char *str, size_t len) {
    unsigned long hash = 5381;
    
    while (len >= 8) {
        hash = (hash * 33 + str[0]) * 33 + str[1];
        hash = (hash * 33 + str[2]) * 33 + str[3];
        hash = (hash * 33 + str[4]) * 33 + str[5];
        hash = (hash * 33 + str[6]) * 33 + str[7];
        
        str += 8;
        len -= 8;
    }
    
    // printf("%lu from OPT PHP Hash\n", hash);
}



void djbx33a_ori(char  const *str,  int len) { 
    unsigned long hash = 5381;
    for (int i = 0; i < len; i++) { 
        hash = hash *2 + (unsigned long) str[i];
    }
    printf("%lu from ori Hash\n", hash);
}

void djbx33a_sse_hu(const char *str, size_t len) {
    unsigned long hash = 5381UL;
    __m128i tiger = _mm_set_epi16(0, 0, 0, 0, 1, 2, 2 * 2, 2 * 2 * 2);
    __m128i tmp, string;
    const __m128i zero = _mm_setzero_si128();
 
    for (; len >= 8; len -= 8, str += 8) {
        string = _mm_loadu_si64((__m128i*)str);
       
        string = _mm_unpacklo_epi8(string, zero);
        tmp = _mm_madd_epi16(string, tiger);
        tmp = _mm_add_epi32(tmp, _mm_srli_si128(tmp, 4));
        hash = hash * 2 * 2 * 2 * 2UL + _mm_cvtsi128_si32(tmp);

        string = _mm_srli_si128(string, 4);
        __m128i tmp = _mm_madd_epi16(string, tiger);
        tmp= _mm_add_epi32(tmp, _mm_srli_si128(tmp, 4));
        hash = hash * 2 * 2 * 2 * 2UL + _mm_cvtsi128_si32(tmp);
    }

    printf("%lu from djbx33a_sse_hu Hash\n", hash);
}

void djbx33a_sse_hu2(const char *str, size_t len) {
    unsigned long hash = 5381UL;
    /* it should be 1, 33, 33*33, 33*33*33. However, 33^3 (0x8c61) is treated
       as signed 16bit integer. So for the two low bytes, we scale down by 33
      and bring it back later */
    __m128i djbx33 = _mm_set_epi16(0, 0, 0, 0, 1, 33, 33, 33 * 33);

    /* low:  str[0] * Z_L(33 * 33 * 33) + str[1] * Z_L(33 * 33) */
    __m128i low, product, string;

    const __m128i zero = _mm_setzero_si128();

    for (; len >= 8; len -= 8, str += 8) {
        string = _mm_loadu_si64((__m128i*)str);

        string = _mm_unpacklo_epi8 (string, zero);

        low = _mm_madd_epi16(string, djbx33);
        product = _mm_srli_si128(low, 4);
        product = _mm_add_epi32(low, product);
        low = _mm_slli_epi32(low, 5);
        product = _mm_add_epi32(product, low);
        hash = hash * 33 * 33 * 33 * 33UL + _mm_cvtsi128_si32(product);

        string = _mm_srli_si128(string, 8);

        low = _mm_madd_epi16(string, djbx33);
        product = _mm_srli_si128(low, 4);
        product = _mm_add_epi32(low, product);
        low = _mm_slli_epi32(low, 5);
        product = _mm_add_epi32(product, low);
        hash = hash * 33 * 33 * 33 * 33UL + _mm_cvtsi128_si32(product);
    }
    // printf("%lu from djbx33a_sse_hu2 Hash\n", hash);
}



int main() {
    const char* str = "abcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefgh";
    // djbx33a_ori(str, 16);
    for(int i; i<500000; i++){
        djbx33a_opt_php(str, 256);
    }
    
    // djbx33a_sse(str, 16);
    // djbx33a_sse_hu(str, 16);
    // djbx33a_sse_hu2(str, 16);
    // djbx33a_avx256(str,16);
    // djbx33a_avx256_2(str,16);
    // djbx33a_avx512(str, 16);
}
