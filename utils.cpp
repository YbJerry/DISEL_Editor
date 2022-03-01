#include "utils.h"



long long fact(long long n){
    if(n == 0 || n == 1){
        return 1;
    }
    return n * fact(n-1);
}

long long nCr(long long n, long long r){
    return fact(n)/(fact(n-r)*fact(r));
}
