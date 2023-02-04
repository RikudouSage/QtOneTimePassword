#ifndef ONETIMEPASSWORDGENERATOR_H
#define ONETIMEPASSWORDGENERATOR_H

#include <QObject>

class OneTimePasswordGenerator : public QObject
{
    Q_OBJECT
public:
    explicit OneTimePasswordGenerator(QObject *parent = nullptr);

    QByteArray generateHOTP(const QByteArray &rawSecret, quint64 counter, int length);
    QByteArray generateHOTP(const QString &secret, quint64 counter, int length);

    QByteArray generateTOTP(const QByteArray &rawSecret, int length);
    QByteArray generateTOTP(const QString &secret, int length);
private:
    QByteArray fromBase32(const QString &input);

signals:

};

#endif // ONETIMEPASSWORDGENERATOR_H
