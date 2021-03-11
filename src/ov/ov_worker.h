#ifndef OV_WORKER
#define OV_WORKER

#include "libov/src/ov_types.h"
#include <QObject>
#include <QString>

class OvWorker : public QObject {
  Q_OBJECT

public:
  OvWorker();
  const QString& getToken();

public slots:
  void start();
  void stop();
  void setFrontend(const QString& frontend);
  void setToken(const QString& token);

signals:
  void started(const QString& frontend);
  void stopped();
  void errored(const QString& error);
  void frontendChanged(const QString& frontend);
  void tokenChanged(const QString& token);

private:
  ov_client_base_t* client;
  QString frontendType;
  QString token;
  bool isRunning;
};

#endif