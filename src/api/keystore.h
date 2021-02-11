#ifndef KEY_STORE_H
#define KEY_STORE_H

#include <QCoreApplication>
#include <QString>

#ifndef KEYSTORE_NAME
#define KEYSTORE_NAME "org.digital-stage.client"
#endif

using namespace std;

class KeyStore : QObject
{
    Q_OBJECT

public:
    struct Credentials {
        QString email;
        QString password;
    };

    KeyStore();
    ~KeyStore();

    bool store(Credentials credentials);
    Credentials* restore(QString email) const;
    bool remove(QString email) const;

protected:
private:
};

#endif // KEY_STORE_H
