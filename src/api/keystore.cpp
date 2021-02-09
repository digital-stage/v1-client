#include "keystore.h"
#include <qt5keychain/keychain.h>
#include <iostream>
#include <QCoreApplication>

KeyStore::KeyStore() {

}

KeyStore::~KeyStore() {

}

bool KeyStore::store(Credentials credentials) {
    QKeychain::WritePasswordJob job( QLatin1String("ds-client") );
    job.setAutoDelete( false );
    job.setKey( credentials.email );
    job.setBinaryData( credentials.password.toUtf8() );
    QEventLoop loop;
    job.connect( &job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()) );
    job.start();
    loop.exec();
    if( job.error() ) {
        std::cerr << "Storing binary password failed: "
                  << qPrintable(job.errorString()) << std::endl;
        return false;
    }
    std::cout << "Password stored successfully" << std::endl;
    return true;
}


KeyStore::Credentials KeyStore::restore(QString email) const {

}

bool KeyStore::remove(QString email) const {
    QKeychain::DeletePasswordJob job( QLatin1String("ds-client") );
    job.setAutoDelete( false );
    job.setKey( email );
    QEventLoop loop;
    job.connect( &job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()) );
    job.start();
    loop.exec();
    if ( job.error() ) {
        std::cerr << "Deleting password failed: " << qPrintable(job.errorString()) << std::endl;
        return false;
    }
    std::cout << "Password deleted successfully" << std::endl;
    return true;
}
