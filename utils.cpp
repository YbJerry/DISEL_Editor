//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
