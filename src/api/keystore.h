#ifndef KEY_STORE_H
#define KEY_STORE_H

#include <QCoreApplication>
#include <QString>

#ifndef KEYSTORE_NAME
#define KEYSTORE_NAME "org.digital-stage.client"
#endif

using namespace std;

class KeyStore : QObject {
  Q_OBJECT

public:
  struct Credentials {
    QString email;
    QString password;
  };

  KeyStore();
  ~KeyStore();

  bool store(Credentials credentials);
  Credentials* restore(const QString& email) const;
  bool remove(const QString& email) const;

  QString restoreEmail();
  void storeEmail(const QString& email);
  QString restoreFrontendSelection();
  void storeFrontendSelection(const QString& frontend);
};

#endif // KEY_STORE_H
