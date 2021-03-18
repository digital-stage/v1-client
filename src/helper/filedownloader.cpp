#include "filedownloader.h"
#include <QFile>
#include <QNetworkRequest>
#include <iostream>

FileDownloader::FileDownloader(const QUrl& url, const QString& targetPath_,
                               QObject* parent)
    : QObject(parent), targetPath(targetPath_)
{
  std::cout << "Downloading " << url.toString().toStdString() << " to "
            << targetPath.toStdString() << std::endl;
  connect(&webCtrl, &QNetworkAccessManager::finished, this,
          [=](QNetworkReply* pReply) {
            QFile localFile(targetPath);
            if(!localFile.open(QIODevice::WriteOnly))
              return;
            localFile.write(pReply->readAll());
            localFile.close();
            // emit a signal
            pReply->deleteLater();
            emit downloaded(targetPath);
          });
  /*
  connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
          SLOT(fileDownloaded(QNetworkReply*)));*/
  QNetworkRequest request(url);
  request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
  webCtrl.get(request);
}

FileDownloader::~FileDownloader() {}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
  QFile localFile(targetPath);
  if(!localFile.open(QIODevice::WriteOnly))
    return;
  localFile.write(pReply->readAll());
  localFile.close();
  // emit a signal
  pReply->deleteLater();
  emit downloaded(targetPath);
}