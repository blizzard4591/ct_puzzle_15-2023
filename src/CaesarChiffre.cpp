#include "CaesarChiffre.h"


QString CaesarCipher::decrypt(QString const& encrypted, int shift) {
    QString decrypted;

    for (auto const& c : encrypted) {
        if (c.isLetter()) {
            if (c.isUpper()) {
                decrypted.append(QChar('A' + (c.unicode() - 'A' + 26 - shift) % 26));
            } else {
                decrypted.append(QChar('a' + (c.unicode() - 'a' + 26 - shift) % 26));
            }
        } else {
            decrypted.append(c);
        }
    }

    return decrypted;
}

QString CaesarCipher::encrypt(QString const& plain, int shift) {
    QString encrypted;

    for (auto const& c : plain) {
        if (c.isLetter()) {
            if (c.isUpper()) {
                encrypted.append(QChar('A' + (c.unicode() - 'A' + shift) % 26));
            } else {
                encrypted.append(QChar('a' + (c.unicode() - 'a' + shift) % 26));
            }
        } else {
            encrypted.append(c);
        }
    }

    return encrypted;
}
