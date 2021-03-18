#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QObject>

class FileDownloader : public QObject {
  Q_OBJECT

public:
  explicit FileDownloader(const QUrl& url, const QString& targetPath, QObject* parent = nullptr);
  ~FileDownloader() override;

signals:
  void downloaded(const QString& targetPath);

private slots:
  void fileDownloaded(QNetworkReply* pReply);

private:
  QNetworkAccessManager webCtrl;
  QString targetPath;
};

#endif // FILEDOWNLOADER_H