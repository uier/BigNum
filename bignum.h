#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BASE 10000
#define max(x,y) (x >= y ? x : y)
#define min(x,y) (x <= y ? x : y)

typedef struct _BigNum_ {
    // bound: 32767, here we store 4 digits (max:9999) in an int32_t in order to handle decimal number.
    int32_t size, *data;
    bool nega;
} BigNum;

// initial big number to n-bit integer, return -1 if fail to initialize
int32_t bigNumInit( BigNum *pNum, int32_t n );

// free a big number
void bigNumFree( BigNum *pObj );

// 0 equal, -1 (obj1 less than obj2), 1 (obj1 greater than obj2)
int32_t bigNumCmp( const BigNum *pObj1, const BigNum *pObj2 );

// print big number decimal
void bigNumPrintDec( const BigNum *pObj );

// print big number binary
void bigNumPrintBin( const BigNum *pObj );

// print big number hex
void bigNumPrintHex( const BigNum *pObj );

// set big number by decimal string, return -1 if fail or invalid
int32_t bigNumSetDec( BigNum *pObj, const char *decimal, const char* file, const int32_t line, const char* func );

// set big number by binary string, return -1 if fail or invalid
int32_t bigNumSetBin( BigNum *pObj, const char *binary, const char* file, const int32_t line, const char* func );

// set big number by hex string, return -1 if fail or invalid
int32_t bigNumSetHex( BigNum *pObj, const char *hex, const char* file, const int32_t line, const char* func );

// obj1 add obj2, return -1 if fail
int32_t bigNumAdd( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 );

// obj1 substract obj2, return -1 if fail
int32_t bigNumSub( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 );

// obj1 multiply obj2, return -1 if fail
int32_t bigNumMul( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 );

// obj1 divide obj2, return -1 if fail
int32_t bigNumDiv( BigNum *pQuotient, BigNum *pRemainder, const BigNum *pObj1, const BigNum *pObj2 );

// obj1 to the power obj2, return -1 if fail
int32_t bigNumPow( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 );

// factorial of obj1, return -1 if fail
int32_t bigNumFactorial( BigNum *pAns, const BigNum *pObj );

// k-permutations of n, return -1 if fail
int32_t bigNumPermutation( BigNum *pAns, const BigNum *pN, const BigNum *pK );

// k-combinations of n, return -1 if fail
int32_t bigNumCombination( BigNum *pAns, const BigNum *pN, const BigNum *pK );

// greatest common divisor of obj1 and obj2, return -1 if fail
int32_t bigNumGCD( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 );

// least common multiple of obj1 and obj2, return -1 if fail
int32_t bigNumLCM( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 );

// check obj is prime, 1 true, 0 false
int32_t isPrime( const BigNum *pObj );

// square root of obj
int32_t bigNumSQRT( BigNum *pAns, const BigNum *pObj );

// n-th Fibonacci number
int32_t NthFibonacci( BigNum *pAns, const BigNum *pN );

// the log of x based b, log_b(x), taking floor( log_b(x) )
int32_t bigNumLog( BigNum *pAns, const BigNum *pB, const BigNum *pX );

// chech num is palindrome, 1 true, 0 false
int32_t isPalindrome( const BigNum *pNum );

// print big number in number syetem based on n, support base 2 ~ 16
int32_t bigNumPrint( const BigNum *pObj, int32_t n );

int32_t __overflow(const char* file, const int32_t line, const char* func) {
    printf("%s:%d:%s\n", file, line, func);
    puts("overflow! please initialize BigNum with a larger bit number.");
    return -1;
}

int32_t __getIdx( const BigNum *pObj ) {
    int32_t idx = pObj->size - 1;
    while ( idx >= 0 && pObj->data[idx] == 0 )  idx--;
    return idx == -1 ? 0 : idx;
}

int32_t __bigNumInitSize( BigNum *pNum, int32_t size ) {
    pNum->size = size;
    pNum->data = calloc(pNum->size, sizeof(int32_t));
    if ( pNum->data == NULL )   return -1;
    pNum->nega = false;
    return 0;
}

int32_t __bigNumReInitSize( BigNum *pNum, int32_t size ) {
    pNum->size = size;
    pNum->data = realloc(pNum->data, sizeof(int32_t)*(pNum->size));
    if ( pNum->data == NULL )   return -1;
    for ( int32_t idx=0; idx<size; ++idx ) pNum->data[idx] = 0;
    pNum->nega = false;
    return 0;
}

int32_t __bigNumCpy( BigNum *pObj1, const BigNum *pObj2 ) {
    int32_t len = __getIdx(pObj2), size = max(pObj1->size, pObj2->size);
    if ( __bigNumReInitSize(pObj1, size) == -1 )    printf("ReInitSize Fail.\n");
    for ( int32_t idx=0; idx<=len; ++idx )
        pObj1->data[idx] = pObj2->data[idx];
    pObj1->nega = pObj2->nega;
    return 0;
}

int32_t __carry( const BigNum *pNum ) {
    int32_t carry = 0;
    for ( int32_t idx=0; idx<pNum->size; ++idx ) {
        pNum->data[idx] += carry;
        if ( pNum->data[idx] >= 0 ) {
            carry = pNum->data[idx] / BASE;
            pNum->data[idx] %= BASE;
        } else {
            carry = -1;
            pNum->data[idx] += BASE;
        }
    }
    if ( carry > 0 )    return -1;
    return 0;
}

void __swap( const BigNum *pObj1, const BigNum *pObj2 ) {
    const BigNum *pTemp = pObj1;
    pObj1 = pObj2;
    pObj2 = pTemp;
}

void __divideTen( const BigNum *pNum ) {
    int32_t idx = __getIdx(pNum);
    for ( ; idx>0; --idx ) {
        pNum->data[idx-1] += (pNum->data[idx]%10)*BASE;
        pNum->data[idx] /= 10;
    }
    pNum->data[0] /= 10;
}

void __increment( BigNum *pNum ) {
    pNum->data[0]++;
    if ( __carry(pNum) == -1 )  printf("increment overflow!\n");
}

void __decrement( BigNum *pNum ) {
    pNum->data[0]--;
    if ( __carry(pNum) == -1 )  printf("decrement overflow!\n");
}

int32_t bigNumInit( BigNum *pNum, int32_t n ) {
    pNum->size = max(n/32,1)*4;
    pNum->data = calloc(pNum->size, sizeof(int32_t));
    if ( pNum->data == NULL )   return -1;
    pNum->nega = false;
    return 0;
}

void bigNumFree( BigNum *pObj ) {
    free(pObj->data);
    pObj->data = NULL;
}

int32_t bigNumCmp( const BigNum *pObj1, const BigNum *pObj2 ) {
    int32_t len1 = __getIdx(pObj1), len2 = __getIdx(pObj2);
    if ( len1 < len2 || (pObj1->nega && !pObj2->nega) ) return -1;
    if ( len1 > len2 || (!pObj1->nega && pObj2->nega) ) return 1;
    for ( int32_t idx=len1; idx>=0; --idx ) {
        if ( pObj1->data[idx] < pObj2->data[idx] )  return (pObj1->nega ? 1 : -1);
        if ( pObj1->data[idx] > pObj2->data[idx] )  return (pObj1->nega ? -1 : 1);
    }
    return 0;
}

void bigNumPrintDec( const BigNum *pObj ) {
    int32_t idx = __getIdx(pObj);
    if ( pObj->nega )   printf("-");
    printf("%d", pObj->data[idx--]);
    for ( ; idx>=0; --idx ) printf("%04d", pObj->data[idx]);
    printf("\n");
}

void bigNumPrintBin( const BigNum *pObj ) {
    BigNum PowerOf2, Bin, Rem, Cpy, Obj;
    int32_t len = __getIdx(pObj);
    __bigNumInitSize(&PowerOf2, pObj->size);
    __bigNumInitSize(&Bin, 1);
    __bigNumInitSize(&Rem, 1);
    __bigNumInitSize(&Cpy, pObj->size);
    __bigNumInitSize(&Obj, pObj->size);
    bigNumSetDec(&PowerOf2, "1", __FILE__, __LINE__, __func__);
    bigNumSetDec(&Bin, "2", __FILE__, __LINE__, __func__);
    __bigNumCpy(&Obj, pObj);
    Obj.nega = false;
    int32_t exp = 0;
    while ( bigNumCmp(&PowerOf2, &Obj) <= 0 ) {
        exp++;
        __bigNumCpy(&Cpy, &PowerOf2);
        bigNumMul(&PowerOf2, &Cpy, &Bin);
    }
    if ( exp > 0 )  __bigNumCpy(&PowerOf2, &Cpy);
    exp = max(exp-1, 0);
    int32_t *bit = calloc(exp+1, sizeof(int32_t));
    for ( int32_t idx=exp; idx>=0; --idx ) {
        if ( bigNumCmp(&PowerOf2, &Obj) <= 0 ) {
            bit[exp-idx] = 1;
            __bigNumCpy(&Cpy, &Obj);
            bigNumSub(&Obj, &Cpy, &PowerOf2);
        }
        __bigNumCpy(&Cpy, &PowerOf2);
        bigNumDiv(&PowerOf2, &Rem, &Cpy, &Bin);
    }
    if ( pObj->nega )   printf("-");
    for ( int idx=0; idx<=exp; ++idx )  printf("%d", bit[idx]);
    printf("\n");
    bigNumFree(&PowerOf2);
    bigNumFree(&Bin);
    bigNumFree(&Rem);
    bigNumFree(&Cpy);
    bigNumFree(&Obj);
}

void bigNumPrintHex( const BigNum *pObj ) {
    BigNum PowerOf16, Hex, Rem, Cpy, Obj;
    int32_t len = __getIdx(pObj);
    __bigNumInitSize(&PowerOf16, pObj->size);
    __bigNumInitSize(&Hex, 1);
    __bigNumInitSize(&Rem, 1);
    __bigNumInitSize(&Cpy, pObj->size);
    __bigNumInitSize(&Obj, pObj->size);
    bigNumSetDec(&PowerOf16, "1", __FILE__, __LINE__, __func__);
    bigNumSetDec(&Hex, "16", __FILE__, __LINE__, __func__);
    __bigNumCpy(&Obj, pObj);
    Obj.nega = false;
    int32_t exp = 0;
    while ( bigNumCmp(&PowerOf16, &Obj) <= 0 ) {
        exp++;
        __bigNumCpy(&Cpy, &PowerOf16);
        bigNumMul(&PowerOf16, &Cpy, &Hex);
    }
    if ( exp > 0 )  __bigNumCpy(&PowerOf16, &Cpy);
    exp = max(exp-1, 0);
    int32_t *bit = calloc(exp+1, sizeof(int32_t));
    for ( int32_t idx=exp; idx>=0; --idx ) {
        while ( bigNumCmp(&PowerOf16, &Obj) <= 0 ) {
            bit[exp-idx]++;
            __bigNumCpy(&Cpy, &Obj);
            bigNumSub(&Obj, &Cpy, &PowerOf16);
        }
        __bigNumCpy(&Cpy, &PowerOf16);
        bigNumDiv(&PowerOf16, &Rem, &Cpy, &Hex);
    }
    if ( pObj->nega )   printf("-");
    for ( int32_t idx=0; idx<=exp; ++idx ) {
        if ( bit[idx] >= 10 )   printf("%c", ('A'+bit[idx]-10));
        else    printf("%d", bit[idx]);
    }
    printf("\n");
    bigNumFree(&PowerOf16);
    bigNumFree(&Hex);
    bigNumFree(&Rem);
    bigNumFree(&Cpy);
    bigNumFree(&Obj);
}

int32_t bigNumSetDec( BigNum *pObj, const char *decimal, const char* file, const int32_t line, const char* func ) {
    for ( int32_t idx=0; idx<pObj->size; ++idx )    pObj->data[idx] = 0;
    int32_t len = strlen(decimal);
    if ( len <= 0 ) return -1;
    else if ( decimal[0] == '-' ) {
        pObj->nega = true;
        decimal++;
        len--;
    } else pObj->nega = false;
    for ( int32_t idx=0; idx<len; ++idx ) {
        if ( (len-1-idx)/4 >= pObj->size )  return __overflow(file, line, func);
        pObj->data[(len-1-idx)/4] = pObj->data[(len-1-idx)/4]*10 + (decimal[idx]-'0');
    }
    return 0;
}

int32_t bigNumSetBin( BigNum *pObj, const char *binary, const char* file, const int32_t line, const char* func ) {
    bigNumSetDec(pObj, "0", __FILE__, __LINE__, __func__);
    int32_t len = strlen(binary);
    if ( len <= 0 ) return -1;
    else if ( binary[0] == '-' ) {
        pObj->nega = true;
        binary++;
        len--;
    } else pObj->nega = false;
    for ( int32_t idx=0; idx<len; ++idx ) {
        for ( int32_t idx2=0; idx2<pObj->size; ++idx2 ) pObj->data[idx2] *= 2;
        pObj->data[0] += binary[idx] - '0';
        if ( __carry(pObj) == -1 )  return __overflow(file, line, func);
    }
}

int32_t bigNumSetHex( BigNum *pObj, const char *hex, const char* file, const int32_t line, const char* func ) {
    bigNumSetDec(pObj, "0", __FILE__, __LINE__, __func__);
    int32_t len = strlen(hex);
    if ( len <= 0 ) return -1;
    else if ( hex[0] == '-' ) {
        pObj->nega = true;
        hex++;
        len--;
    } else pObj->nega = false;
    for ( int32_t idx=0; idx<len; ++idx ) {
        for ( int32_t idx2=0; idx2<pObj->size; ++idx2 ) pObj->data[idx2] *= 16;
        if ( hex[idx] <= '9' )  pObj->data[0] += hex[idx] - '0';
        else if ( hex[idx] <= 'Z' ) pObj->data[0] += 10 + hex[idx] - 'A';
        else    pObj->data[0] += 10 + hex[idx] - 'a';
        if ( __carry(pObj) == -1 )  return __overflow(file, line, func);
    }
}

int32_t bigNumAdd( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 ) {
    int32_t len = max(__getIdx(pObj1), __getIdx(pObj2));
    if ( __bigNumInitSize(pAns, len+1) == -1 )  return -1;
    bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
    BigNum Cpy1, Cpy2;
    __bigNumInitSize(&Cpy1, 1);
    __bigNumInitSize(&Cpy2, 1);
    __bigNumCpy(&Cpy1, pObj1);
    __bigNumCpy(&Cpy2, pObj2);
    int32_t res = bigNumCmp(pObj1, pObj2);
    if ( res == -1 ) {
        if ( pObj1->nega && pObj2->nega ) {
            Cpy1.nega = false;
            Cpy2.nega = false;
            bigNumAdd(pAns, &Cpy2, &Cpy1);
            pAns->nega = true;
        } else if ( !pObj1->nega && !pObj2->nega ) {
            for ( int32_t idx=0; idx<=len; ++idx )
                pAns->data[idx] = pObj1->data[idx] + pObj2->data[idx];
        } else {
            Cpy1.nega = false;
            bigNumSub(pAns, &Cpy2, &Cpy1);
        }
    } else if ( res == 1 ) {
        if ( pObj1->nega && pObj2->nega ) {
            Cpy1.nega = false;
            Cpy2.nega = false;
            bigNumAdd(pAns, &Cpy1, &Cpy2);
            pAns->nega = true;
        } else if ( !pObj1->nega && !pObj2->nega ) {
            for ( int32_t idx=0; idx<=len; ++idx )
                pAns->data[idx] = pObj1->data[idx] + pObj2->data[idx];
        } else {
            Cpy2.nega = false;
            bigNumSub(pAns, &Cpy1, &Cpy2);
        }
    } else {
        for ( int32_t idx=0; idx<=len; ++idx )
            pAns->data[idx] = pObj1->data[idx] + pObj2->data[idx];
    }
    bigNumFree(&Cpy1);
    bigNumFree(&Cpy2);
    return __carry(pAns);
}

int32_t bigNumSub( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 ) {
    int32_t len = max(__getIdx(pObj1), __getIdx(pObj2));
    if ( __bigNumInitSize(pAns, len+1) == -1 )  return -1;
    bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
    int32_t res = bigNumCmp(pObj1, pObj2);
    BigNum Cpy1, Cpy2;
    __bigNumInitSize(&Cpy1, 1);
    __bigNumInitSize(&Cpy2, 1);
    __bigNumCpy(&Cpy1, pObj1);
    __bigNumCpy(&Cpy2, pObj2);
    if ( res == -1 ) {
        if ( pObj1->nega && pObj2->nega ) { // - - -
            Cpy1.nega = false;
            Cpy2.nega = false;
            bigNumSub(pAns, &Cpy2, &Cpy1);
        } else if ( !pObj1->nega && !pObj2->nega ) { // + - +
            bigNumSub(pAns, &Cpy2, &Cpy1);
            pAns->nega = true;
        } else { // - - +
            Cpy1.nega = false;
            bigNumAdd(pAns, &Cpy1, &Cpy2);
            pAns->nega = true;
        }
    } else if ( res == 1 ) {
        if ( pObj1->nega && pObj2->nega ) { // - - -
            Cpy1.nega = false;
            Cpy2.nega = false;
            bigNumSub(pAns, &Cpy2, &Cpy1);
        } else if ( !pObj1->nega && !pObj2->nega ) { // + - +
            for ( int32_t idx=0; idx<=len; ++idx )
                pAns->data[idx] = pObj1->data[idx] - pObj2->data[idx];
        } else { // - - +
            Cpy1.nega = false;
            bigNumAdd(pAns, &Cpy1, &Cpy2);
            pAns->nega = true;
        }
    }
    bigNumFree(&Cpy1);
    bigNumFree(&Cpy2);
    return __carry(pAns);
}

int32_t bigNumMul( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 ) {
    __bigNumInitSize(pAns, len1+1+len2+1+1);
    bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
    int32_t len1 = __getIdx(pObj1), len2 = __getIdx(pObj2);
    for ( int32_t idx1=0; idx1<=len1; ++idx1 ) {
        for ( int32_t idx2=0; idx2<=len2; ++idx2 )
            pAns->data[idx1+idx2] += pObj1->data[idx1] * pObj2->data[idx2];
        if ( __carry(pAns) == -1 )  return -1;
    }
    pAns->nega = ((pObj1->nega)^(pObj2->nega));
    return 0;
}

int32_t bigNumDiv( BigNum *pQuotient, BigNum *pRemainder, const BigNum *pObj1, const BigNum *pObj2 ) {
    if ( __getIdx(pObj2) == 0 && pObj2->data[0] == 0 )  return -1;
    __bigNumInitSize(pQuotient, 1);
    __bigNumInitSize(pRemainder, 1);
    __bigNumCpy(pQuotient, pObj1);
    __bigNumCpy(pRemainder, pObj2);
    bigNumSetDec(pQuotient, "0", __FILE__, __LINE__, __func__);
    bigNumSetDec(pRemainder, "0", __FILE__, __LINE__, __func__);
    int32_t res = bigNumCmp(pObj1, pObj2);
    if ( res == -1 ) {
        __bigNumCpy(pRemainder, pObj1);
    } else if ( res == 0 ) {
        bigNumSetDec(pQuotient, "1", __FILE__, __LINE__, __func__);
    } else {
        int32_t len1 = __getIdx(pObj1), len2 = __getIdx(pObj2);
        int32_t cnt = len1 - len2;
        BigNum p1, p2;
        __bigNumInitSize(&p1, pObj1->size);
        __bigNumInitSize(&p2, pObj1->size);
        __bigNumCpy(&p1, pObj1);
        __bigNumCpy(&p2, pObj2);
        if ( cnt > 0 ) {
            BigNum pAns, base;
            __bigNumInitSize(&base, cnt+1);
            char *s = malloc(sizeof(char)*cnt*4);
            s[0] = '1';
            for ( int32_t idx=1; idx<cnt*4-1; ++idx )   s[idx] = '0';
            s[cnt*4-1] = '\0';
            bigNumSetDec(&base, s, __FILE__, __LINE__, __func__);
            bigNumMul(&pAns, &p2, &base);
            __bigNumCpy(&p2, &pAns);
            free(s);
            bigNumFree(&pAns);
            bigNumFree(&base);
        }
        len2 = __getIdx(&p2);
        while ( len2 < len1 || p2.data[len2]*10 <= p1.data[len1] ) {
            for ( int32_t idx=0; idx<=len2; ++idx ) p2.data[idx] *= 10;
            __carry(&p2);
            len2 = __getIdx(&p2);
        }
        char *s = malloc(sizeof(char)*((len1+1)*4+1));
        int pt = 0;
        for ( int32_t idx=0; idx<(len1+1)*4; ++idx) s[idx] = '0';
        while ( bigNumCmp(&p2, pObj2) >= 0 ) {
            if ( bigNumCmp(&p1,&p2) > 0 ) {
                s[pt]++;
                BigNum pAns;
                bigNumSub(&pAns,&p1,&p2);
                __bigNumCpy(&p1,&pAns);
                bigNumFree(&pAns);
            } else  {
                if ( bigNumCmp(&p1,&p2) == 0 ) {
                    bigNumSetDec(&p1, "0", __FILE__, __LINE__, __func__);
                    s[pt]++;
                }
                __divideTen(&p2);
                pt++;
            }
        }
        s[pt] = '\0';
        char *t = s;
        while ( t[0] == '0' )   t++;
        if ( bigNumSetDec(pQuotient,t,__FILE__,__LINE__,__func__) == -1 )   return -1;
        __bigNumCpy(pRemainder,&p1);
        bigNumFree(&p1);
        bigNumFree(&p2);
        free(s);
    }
    pQuotient->nega = ((pObj1->nega)^(pObj2->nega));
    return 0;
}

int32_t bigNumPow( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 ) {
    __bigNumInitSize(pAns, 1);
    bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
    if ( __getIdx(pObj1) == 0 && pObj1->data[0] == 0 )  return 0;
    int len1 = __getIdx(pObj1), len2 = __getIdx(pObj2);
    BigNum p1, p2, Cpy, Rem, Two;
    __bigNumInitSize(&p1, (len1+1)*(len2+1)+1);
    __bigNumInitSize(&p2, len2+1+1);
    __bigNumCpy(&p1, pObj1);
    __bigNumCpy(&p2, pObj2);
    __bigNumInitSize(&Cpy, (len1+1)*(len2+1)+1);
    bigNumInit(&Rem, 32);
    bigNumInit(&Two, 32);
    bigNumSetDec(&Two, "2", __FILE__, __LINE__, __func__);
    bigNumSetDec(pAns, "1", __FILE__, __LINE__, __func__);
    if ( __getIdx(pObj2) == 0 && pObj2->data[0] == 0 )  return 0;
    while ( __getIdx(&p2) > 0 || p2.data[0] != 0 ) {
        if ( ((p2.data[0])&1) ) {
            __bigNumCpy(&Cpy, pAns);
            bigNumMul(pAns, &Cpy, &p1);
        }
        __bigNumCpy(&Cpy, &p1);
        bigNumMul(&p1, &Cpy, &Cpy);
        __bigNumCpy(&Cpy, &p2);
        bigNumDiv(&p2, &Rem, &Cpy, &Two);
    }
    bigNumFree(&p1);
    bigNumFree(&p2);
    bigNumFree(&Cpy);
    bigNumFree(&Rem);
    bigNumFree(&Two);
    return 0;
}

int32_t bigNumFactorial( BigNum *pAns, const BigNum *pObj ) {
    if ( pObj->nega )   return -1;
    __bigNumInitSize(pAns, 1);
    if ( __getIdx(pObj) == 0 && pObj->data[0] == 0 ) {
        bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
        __increment(pAns);
        return 0;
    }
    BigNum Obj, Cpy;
    __bigNumInitSize(&Cpy, pObj->size);
    __bigNumInitSize(&Obj, pObj->size);
    __bigNumCpy(&Obj, pObj);
    __bigNumCpy(pAns, pObj);
    __decrement(&Obj);
    while ( __getIdx(&Obj) > 0 || Obj.data[0] > 0 ) {
        __bigNumCpy(&Cpy, pAns);
        bigNumMul(pAns, &Cpy, &Obj);
        __decrement(&Obj);
    }
    bigNumFree(&Obj);
    bigNumFree(&Cpy);
    return 0;
}

int32_t bigNumPermutation( BigNum *pAns, const BigNum *pN, const BigNum *pK ) {
    __bigNumInitSize(pAns, 1);
    int32_t res = bigNumCmp(pN, pK);
    if ( res < 0 || pN->nega || pK->nega )  return -1;
    else if ( res == 0 || (__getIdx(pK) == 0 && pK->data[0] == 0) ) {
        bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
        __increment(pAns);
        return 0;
    }
    BigNum Obj, Cpy, Mul;
    __bigNumInitSize(&Obj, pK->size);
    __bigNumInitSize(&Cpy, pN->size);
    __bigNumInitSize(&Mul, pN->size);
    __bigNumCpy(&Obj, pK);
    __bigNumCpy(&Mul, pN);
    __bigNumCpy(pAns, pN);
    __decrement(&Obj);
    __decrement(&Mul);
    while ( __getIdx(&Obj) > 0 || Obj.data[0] > 0 ) {
        __bigNumCpy(&Cpy, pAns);
        bigNumMul(pAns, &Cpy, &Mul);
        __decrement(&Obj);
        __decrement(&Mul);
    }
    bigNumFree(&Obj);
    bigNumFree(&Cpy);
    bigNumFree(&Mul);
    return 0;
}

int32_t bigNumCombination( BigNum *pAns, const BigNum *pN, const BigNum *pK ) {
    __bigNumInitSize(pAns, 1);
    int32_t res = bigNumCmp(pN, pK);
    if ( res < 0 || pN->nega || pK->nega )  return -1;
    else if ( res == 0 || (__getIdx(pK) == 0 && pK->data[0] == 0) ) {
        bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
        __increment(pAns);
        return 0;
    }
    BigNum Obj, Cpy, Mul, Rem;
    __bigNumInitSize(&Obj, pK->size);
    __bigNumInitSize(&Cpy, pN->size);
    __bigNumInitSize(&Mul, pN->size);
    __bigNumInitSize(&Rem, 1);
    __bigNumCpy(&Obj, pK);
    __bigNumCpy(&Mul, pN);
    __bigNumCpy(pAns, pN);
    __decrement(&Obj);
    __decrement(&Mul);
    while ( __getIdx(&Obj) > 0 || Obj.data[0] > 0 ) {
        __bigNumCpy(&Cpy, pAns);
        bigNumMul(pAns, &Cpy, &Mul);
        __decrement(&Obj);
        __decrement(&Mul);
    }
    __bigNumCpy(&Mul, pK);
    bigNumFactorial(&Obj, &Mul);
    __bigNumCpy(&Cpy, pAns);
    bigNumDiv(pAns, &Rem, &Cpy, &Obj);
    bigNumFree(&Obj);
    bigNumFree(&Cpy);
    bigNumFree(&Mul);
    bigNumFree(&Rem);
    return 0;
}

int32_t bigNumGCD( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 ) {
    if ( pObj1->nega || pObj2->nega || (__getIdx(pObj1) == 0 && pObj1->data[0] == 0) || (__getIdx(pObj2) == 0 && pObj2->data[0] == 0))  return -1;
    __bigNumInitSize(pAns, 1);
    __bigNumCpy(pAns, pObj1);
    BigNum Quo, Cpy, p2;
    __bigNumInitSize(&Quo, 1);
    __bigNumInitSize(&Cpy, max(pObj1->size, pObj2->size));
    __bigNumInitSize(&p2, max(pObj1->size, pObj2->size));
    __bigNumCpy(&p2, pObj2);
    while ( __getIdx(&p2) > 0 || p2.data[0] > 0 ) {
        bigNumDiv(&Quo, &Cpy, pAns, &p2);
        __bigNumCpy(pAns, &p2);
        __bigNumCpy(&p2, &Cpy);
    }
    bigNumFree(&Quo);
    bigNumFree(&Cpy);
    bigNumFree(&p2);
    return 0;
}

int32_t bigNumLCM( BigNum *pAns, const BigNum *pObj1, const BigNum *pObj2 ) {
    if ( pObj1->nega || pObj2->nega || (__getIdx(pObj1) == 0 && pObj1->data[0] == 0) || (__getIdx(pObj2) == 0 && pObj2->data[0] == 0))  return -1;
    __bigNumInitSize(pAns, 1);
    __bigNumCpy(pAns, pObj1);
    BigNum Quo, Cpy, p1, p2;
    __bigNumInitSize(&Quo, 1);
    __bigNumInitSize(&Cpy, max(pObj1->size, pObj2->size));
    __bigNumInitSize(&p1, max(pObj1->size, pObj2->size));
    __bigNumInitSize(&p2, max(pObj1->size, pObj2->size));
    __bigNumCpy(&p2, pObj2);
    while ( __getIdx(&p2) > 0 || p2.data[0] > 0 ) {
        bigNumDiv(&Quo, &Cpy, pAns, &p2);
        __bigNumCpy(pAns, &p2);
        __bigNumCpy(&p2, &Cpy);
    }
    bigNumDiv(&p1, &Quo, pObj1, pAns);
    bigNumDiv(&p2, &Quo, pObj2, pAns);
    __bigNumCpy(&Cpy, pAns);
    bigNumMul(pAns, &Cpy, &p1);
    __bigNumCpy(&Cpy, pAns);
    bigNumMul(pAns, &Cpy, &p2);
    bigNumFree(&Quo);
    bigNumFree(&Cpy);
    bigNumFree(&p1);
    bigNumFree(&p2);
    return 0;
}

int32_t isPrime( const BigNum *pObj ) {
    BigNum Idx, SQRT, Quo, Rem;
    __bigNumInitSize(&Idx, pObj->size);
    __bigNumInitSize(&SQRT, pObj->size);
    __bigNumInitSize(&Quo, 1);
    __bigNumInitSize(&Rem, 1);
    bigNumSQRT(&SQRT, pObj);
    bigNumSetDec(&Idx, "2", __FILE__, __LINE__, __func__);
    int32_t isPrime = 1;
    if ( bigNumCmp(pObj, &Idx) < 0 || ((pObj->data[0])&1) == 0 )    isPrime = 0;
    if ( isPrime == 1 ) {
        __increment(&Idx);
        while ( bigNumCmp(&Idx, &SQRT) <= 0 ) {
            bigNumDiv(&Quo, &Rem, pObj, &Idx);
            if ( __getIdx(&Rem) == 0 && Rem.data[0] == 0 ) {
                isPrime = 0;
                break;
            }
            __increment(&Idx);
            __increment(&Idx);
        }
    }
    bigNumFree(&Idx);
    bigNumFree(&SQRT);
    bigNumFree(&Quo);
    bigNumFree(&Rem);
    return isPrime;
}

int32_t bigNumSQRT( BigNum *pAns, const BigNum *pObj ) {
    __bigNumInitSize(pAns, 1);
    __bigNumCpy(pAns, pObj);
    bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
    BigNum Left, Right, Mid, Rem, Two;
    __bigNumInitSize(&Left, pObj->size);
    __bigNumInitSize(&Right, pObj->size);
    __bigNumInitSize(&Mid, pObj->size);
    __bigNumInitSize(&Rem, 1);
    __bigNumInitSize(&Two, 1);
    __bigNumCpy(&Right, pObj);
    bigNumSetDec(&Two, "2", __FILE__, __LINE__, __func__);
    while ( bigNumCmp(&Left, &Right) < 0 ) {
        bigNumFree(pAns);
        bigNumAdd(pAns, &Left, &Right);
        bigNumDiv(&Mid, &Rem, pAns, &Two);
        bigNumMul(pAns, &Mid, &Mid);
        int32_t res = bigNumCmp(pAns, pObj);
        if ( res == 0 ) {
            __bigNumCpy(pAns, &Mid);
            break;
        } else if ( res < 0 ) {
            __bigNumCpy(&Left, &Mid);
        } else {
            __bigNumCpy(&Right, &Mid);
        }
        bigNumSub(pAns, &Right, &Left);
        if ( __getIdx(pAns) == 0 && pAns->data[0] == 1 ) {
            __bigNumCpy(pAns, &Left);
            break;
        }
    }
    bigNumFree(&Left);
    bigNumFree(&Right);
    bigNumFree(&Mid);
    bigNumFree(&Rem);
    bigNumFree(&Two);
    return 0;
}

void BxM(BigNum B[2][2], BigNum M[2][2], BigNum temp[2][2]) {
    for ( int32_t i=0; i<2; ++i )
        for ( int32_t j=0; j<2; ++j )
            __bigNumInitSize(&temp[i][j], B[0][0].size+M[0][0].size);
    BigNum Cpy1, Cpy2;
    __bigNumInitSize(&Cpy1, B[0][0].size+M[0][0].size);
    __bigNumInitSize(&Cpy2, B[0][0].size+M[0][0].size);
    for ( int32_t i=0; i<2; ++i )
        for ( int32_t j=0; j<2; ++j )
            for ( int32_t k=0; k<2; ++k ) {
                bigNumMul(&Cpy1, &B[i][k], &M[k][j]);
                __bigNumCpy(&Cpy2, &temp[i][j]);
                bigNumAdd(&temp[i][j], &Cpy1, &Cpy2);
            }
    for ( int32_t i=0; i<2; ++i )
        for ( int32_t j=0; j<2; ++j ) {
            __bigNumCpy(&B[i][j], &temp[i][j]);
            bigNumFree(&temp[i][j]);
        }
    bigNumFree(&Cpy1);
    bigNumFree(&Cpy2);
}
void MxM(BigNum M[2][2], BigNum temp[2][2]) {
    for ( int32_t i=0; i<2; ++i )
        for ( int32_t j=0; j<2; ++j )
            __bigNumInitSize(&temp[i][j], M[0][0].size+M[0][0].size);
    BigNum Cpy1, Cpy2;
    __bigNumInitSize(&Cpy1, M[0][0].size+M[0][0].size);
    __bigNumInitSize(&Cpy2, M[0][0].size+M[0][0].size);
    for ( int32_t i=0; i<2; ++i )
        for ( int32_t j=0; j<2; ++j )
            for ( int32_t k=0; k<2; ++k ) {
                bigNumMul(&Cpy1, &M[i][k], &M[k][j]);
                __bigNumCpy(&Cpy2, &temp[i][j]);
                bigNumAdd(&temp[i][j], &Cpy1, &Cpy2);
            }
    for ( int32_t i=0; i<2; ++i )
        for ( int32_t j=0; j<2; ++j ) {
            __bigNumCpy(&M[i][j], &temp[i][j]);
            bigNumFree(&temp[i][j]);
        }
    bigNumFree(&Cpy1);
    bigNumFree(&Cpy2);
}
int32_t NthFibonacci( BigNum *pAns, const BigNum *pN ) {
    __bigNumInitSize(pAns, 1);
    bigNumSetDec(pAns, "0", __FILE__, __LINE__, __func__);
    BigNum M[2][2], B[2][2], temp[2][2];
    for ( int32_t i=0; i<2; ++i )
        for ( int32_t j=0; j<2; ++j ) {
            bigNumInit(&M[i][j], 32);
            bigNumInit(&B[i][j], 32);
            if ( i != 1 || j != 1 ) bigNumSetDec(&M[i][j], "1", __FILE__, __LINE__, __func__);
            if ( i == j )   bigNumSetDec(&B[i][j], "1", __FILE__, __LINE__, __func__);
        }
    BigNum Cnt, Cpy, Rem, Two;
    __bigNumInitSize(&Cnt, 1);
    __bigNumCpy(&Cnt, pN);
    __bigNumInitSize(&Cpy, 1);
    __bigNumInitSize(&Rem, 1);
    __bigNumInitSize(&Two, 1);
    bigNumSetDec(&Two, "2", __FILE__, __LINE__, __func__);
    if ( bigNumCmp(&Cnt, &Two) >= 0 ) {
        __decrement(&Cnt); // Cnt -= 1;
        __decrement(&Cnt); // Cnt -= 1;
        while ( __getIdx(&Cnt) > 0 || Cnt.data[0] > 0 ) {
            if ( ((Cnt.data[0])&1) )    BxM(B, M, temp);
            MxM(M, temp);
            __bigNumCpy(&Cpy, &Cnt);
            bigNumDiv(&Cnt, &Rem, &Cpy, &Two);
        }
        bigNumAdd(pAns, &B[0][0], &B[0][1]);
    } else {
        __bigNumCpy(pAns, &B[1][Cnt.data[0]]);
    }
    for ( int32_t i=0; i<2; ++i )
        for ( int32_t j=0; j<2; ++j ) {
            bigNumFree(&M[i][j]);
            bigNumFree(&B[i][j]);
        }
    bigNumFree(&Cpy);
    bigNumFree(&Rem);
    bigNumFree(&Two);
    return 0;
}

int32_t bigNumLog( BigNum *pAns, const BigNum *pB, const BigNum *pX ) {
    __bigNumInitSize(pAns, 1);
    BigNum Left, Right, Mid, Rem, Two;
    __bigNumInitSize(&Left, pX->size);
    __bigNumInitSize(&Right, pX->size);
    __bigNumInitSize(&Mid, pX->size);
    __bigNumInitSize(&Rem, 1);
    __bigNumInitSize(&Two, 1);
    __bigNumCpy(&Right, pX);
    bigNumSetDec(&Two, "2", __FILE__, __LINE__, __func__);
    while ( bigNumCmp(&Left, &Right) < 0 ) {
        bigNumAdd(pAns, &Left, &Right);
        bigNumDiv(&Mid, &Rem, pAns, &Two);
        bigNumPow(pAns, pB, &Mid);
        int32_t res = bigNumCmp(pAns, pX);
        if ( res == 0 ) {
            __bigNumCpy(pAns, &Mid);
            break;
        } else if ( res < 0 )   __bigNumCpy(&Left, &Mid);
        else    __bigNumCpy(&Right, &Mid);
        bigNumSub(pAns, &Right, &Left);
        if ( __getIdx(pAns) == 0 && pAns->data[0] == 1 ) {
            __bigNumCpy(pAns, &Left);
            break;
        }
    }
    bigNumFree(&Left);
    bigNumFree(&Right);
    bigNumFree(&Mid);
    bigNumFree(&Rem);
    bigNumFree(&Two);
    return 0;
}

int32_t isPalindrome( const BigNum *pNum ) {
    int32_t len = __getIdx(pNum), isPalindrome = 1, pt = 0, base[4] = {1000, 100, 10, 1};
    char *s = malloc(sizeof(char)*(len+1)*4+1);
    for ( int32_t idx=len; idx>=0; --idx )
        for ( int32_t j=0; j<4; ++j )
            s[pt++] = '0'+pNum->data[idx]/base[j]%10;
    s[pt] = '\0';
    char *t = s;
    while ( *t == '0' ) t++;
    len = strlen(t);
    for ( int32_t i=0; i<len; ++i )
        if ( t[i] != t[len-1-i] )
            isPalindrome = 0;
    free(s);
    return isPalindrome;
}

int32_t bigNumPrint( const BigNum *pObj, int32_t n ) {
    if ( n < 2 || n > 16 )  return -1;
    BigNum Power, base, Rem, Cpy, Obj;
    int32_t len = __getIdx(pObj);
    __bigNumInitSize(&Power, pObj->size);
    __bigNumInitSize(&base, 1);
    __bigNumInitSize(&Rem, 1);
    __bigNumInitSize(&Cpy, pObj->size);
    __bigNumInitSize(&Obj, pObj->size);
    char s[3];
    if ( n > 9 ) s[0] = '0' + n/10, s[1] = '0' + n%10, s[2] = '\0';
    else s[0] = '0' + n, s[1] = '\0';
    bigNumSetDec(&Power, "1", __FILE__, __LINE__, __func__);
    bigNumSetDec(&base, s, __FILE__, __LINE__, __func__);
    __bigNumCpy(&Obj, pObj);
    Obj.nega = false;
    int32_t exp = 0;
    while ( bigNumCmp(&Power, &Obj) <= 0 ) {
        exp++;
        __bigNumCpy(&Cpy, &Power);
        bigNumMul(&Power, &Cpy, &base);
        // if ( bigNumMul(&Power, &Cpy, &base) == -1 )  printf("overflow\n");
    }
    if ( exp > 0 )  __bigNumCpy(&Power, &Cpy);
    exp = max(exp-1, 0);
    int32_t *bit = calloc(exp+1, sizeof(int32_t));
    for ( int32_t idx=exp; idx>=0; --idx ) {
        while ( bigNumCmp(&Power, &Obj) <= 0 ) {
            bit[exp-idx]++;
            __bigNumCpy(&Cpy, &Obj);
            bigNumSub(&Obj, &Cpy, &Power);
        }
        __bigNumCpy(&Cpy, &Power);
        bigNumDiv(&Power, &Rem, &Cpy, &base);
    }
    if ( pObj->nega )   printf("-");
    for ( int32_t idx=0; idx<=exp; ++idx ) {
        if ( bit[idx] >= 10 )   printf("%c", ('A'+bit[idx]-10));
        else    printf("%d", bit[idx]);
    }
    printf("\n");
    bigNumFree(&Power);
    bigNumFree(&base);
    bigNumFree(&Rem);
    bigNumFree(&Cpy);
    bigNumFree(&Obj);
    return 0;
}