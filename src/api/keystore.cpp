#include "keystore.h"
#include <QCoreApplication>
#include <QSettings>
#include <iostream>
#include <qt5keychain/keychain.h>

KeyStore::KeyStore() {}

KeyStore::~KeyStore() {}

bool KeyStore::store(Credentials credentials)
{
  QKeychain::WritePasswordJob job(QLatin1String(KEYSTORE_NAME));
  job.setAutoDelete(false);
  job.setKey(credentials.email);
  job.setBinaryData(credentials.password.toUtf8());
  QEventLoop loop;
  job.connect(&job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()));
  job.start();
  loop.exec();
  if(job.error()) {
    std::cerr << "Storing binary password failed: "
              << qPrintable(job.errorString()) << std::endl;
    return false;
  }
  return true;
}

KeyStore::Credentials* KeyStore::restore(const QString& email) const
{
  QKeychain::ReadPasswordJob job(QLatin1String(KEYSTORE_NAME));
  job.setAutoDelete(false);
  job.setKey(email);
  QEventLoop loop;
  job.connect(&job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()));
  job.start();
  loop.exec();
  const QString password = job.textData();
  if(job.error()) {
    std::cerr << "Restoring password failed: " << qPrintable(job.errorString())
              << std::endl;
    return NULL;
  }
  return new KeyStore::Credentials({email, password});
}

bool KeyStore::remove(const QString& email) const
{
  QKeychain::DeletePasswordJob job(QLatin1String(KEYSTORE_NAME));
  job.setAutoDelete(false);
  job.setKey(email);
  QEventLoop loop;
  job.connect(&job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()));
  job.start();
  loop.exec();
  if(job.error()) {
    std::cerr << "Deleting password failed: " << qPrintable(job.errorString())
              << std::endl;
    return false;
  }
  return true;
}

void KeyStore::storeFrontendSelection(const QString& frontend)
{
  QSettings settings("org.digital-stage", "Client");
  settings.setValue("frontend", frontend);
}
QString KeyStore::restoreFrontendSelection()
{
  QSettings settings("org.digital-stage", "Client");
  return settings.value("frontend", "ds").toString();
}

QString KeyStore::restoreEmail()
{
  QSettings settings("org.digital-stage", "Client");
  return settings.value("email", "").toString();
}

void KeyStore::storeEmail(const QString& email)
{
  QSettings settings("org.digital-stage", "Client");
  settings.setValue("email", email);
}