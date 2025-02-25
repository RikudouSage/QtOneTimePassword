#ifndef ONETIMEPASSWORDGENERATOR_H
#define ONETIMEPASSWORDGENERATOR_H

#include <QObject>

#include <QCryptographicHash>

class OneTimePasswordGenerator : public QObject
{
    Q_OBJECT
public:
    explicit OneTimePasswordGenerator(QObject *parent = nullptr);

    QByteArray generateHOTP(const QByteArray &rawSecret, quint64 counter, int length, const QCryptographicHash::Algorithm hash = QCryptographicHash::Sha1);
    Q_INVOKABLE QString generateHOTP(const QString &secret, quint64 counter, int length, const QString &hash = "SHA1");

    QByteArray generateTOTP(const QByteArray &rawSecret, int length, const QCryptographicHash::Algorithm hash = QCryptographicHash::Sha1);
    Q_INVOKABLE QString generateTOTP(const QString &secret, int length, const QString &hash = "SHA1");
private:
    QByteArray fromBase32(const QString &input);
    QCryptographicHash::Algorithm convertAlgorithm(const QString &algorithmName);

signals:

};

#endif // ONETIMEPASSWORDGENERATOR_H
