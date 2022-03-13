//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef UTILS_H
#define UTILS_H

#include <QString>

inline QString templateAddStr(QString sourceName, QString targetName){
    const QString titleStr = "Add New "
                             + sourceName
                             + " to "
                             + targetName;
    return titleStr;
}

long long fact(long long n);

long long nCr(long long n, long long r);

#endif // UTILS_H
