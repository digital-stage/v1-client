#ifndef OV_CONTROLLER
#define OV_CONTROLLER

#include "ov_worker.h"
#include <QObject>
#include <QString>
#include <QThread>

class OvController : public QObject {
  Q_OBJECT
  QThread workerThread;

public:
  OvController();
  ~OvController();

public:
  void start();
  void stop();
  void setToken(const QString& token);
  void changeFrontend(const QString& frontend);

signals:
  void started(const QString& frontend);
  void stopped();
  void errored(const QString& error);
  void frontendChanged(const QString& frontend);
  void tokenChanged(const QString& token);

  // The following events are used to communicate with the worker thread
  void starting();
  void stopping();
  void frontendChanging(const QString& frontend);
  void tokenChanging(const QString& token);
};

#endif