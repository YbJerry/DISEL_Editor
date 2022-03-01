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
