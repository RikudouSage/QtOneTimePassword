#include "onetimepasswordgenerator.h"

#include <QMessageAuthenticationCode>
#include <QtEndian>
#include <QDateTime>

OneTimePasswordGenerator::OneTimePasswordGenerator(QObject *parent)
    : QObject{parent}
{

}

QByteArray OneTimePasswordGenerator::generateHOTP(const QByteArray &rawSecret, quint64 counter, int length)
{
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        counter = qToBigEndian(counter);
#endif
        QByteArray data;
        data.reserve(8);
        for (int i = 7; i >= 0; --i) {
            data.append(counter & 0xff);
            counter >>= 8;
        }
        QMessageAuthenticationCode mac(QCryptographicHash::Sha1);
        mac.setKey(rawSecret);
        mac.addData(data);
        QByteArray hmac = mac.result();
        int offset = hmac.at(hmac.length() - 1) & 0xf;
        quint32 truncatedHash = ((hmac.at(offset) & 0x7f) << 24)
                                | ((hmac.at(offset + 1) & 0xff) << 16)
                                | ((hmac.at(offset + 2) & 0xff) << 8)
                                | (hmac.at(offset + 3) & 0xff);
        int modulus = int(qPow(10, length));
        return QByteArray::number(truncatedHash % modulus, 10).rightJustified(length, '0');
}

QByteArray OneTimePasswordGenerator::generateHOTP(const QString &secret, quint64 counter, int length)
{
    return generateHOTP(fromBase32(secret), counter, length);
}

QByteArray OneTimePasswordGenerator::generateTOTP(const QByteArray &rawSecret, int length)
{
    const qint64 counter = QDateTime::currentDateTime().toMSecsSinceEpoch() / 30000;
    return generateHOTP(rawSecret, counter, length);
}

QByteArray OneTimePasswordGenerator::generateTOTP(const QString &secret, int length)
{
    return generateTOTP(fromBase32(secret), length);
}

QByteArray OneTimePasswordGenerator::fromBase32(const QString &input)
{
    static const char base32Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QByteArray result;
    result.reserve((input.length() * 5 + 7) / 8);
    int buffer = 0;
    int bitsLeft = 0;
    for (int i = 0; i < input.length(); i++) {
        int ch = input[i].toLatin1();
        int value;
        if (ch >= 'A' && ch <= 'Z')
            value = ch - 'A';
        else if (ch >= '2' && ch <= '7')
            value = 26 + ch - '2';
        else
            continue;
        buffer = (buffer << 5) | value;
        bitsLeft += 5;
        if (bitsLeft >= 8) {
            result.append(buffer >> (bitsLeft - 8));
            bitsLeft -= 8;
        }
    }
    return result;
}
