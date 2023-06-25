#ifndef CTPUZZLE_CAESARCHIFFRE_H_
#define CTPUZZLE_CAESARCHIFFRE_H_

#include <QString>

class CaesarCipher {
public:
    static QString decrypt(QString const& encrypted, int shift);
    static QString encrypt(QString const& plain, int shift);
};

#endif
