#ifndef OV_WORKER
#define OV_WORKER

#include "libov/src/ov_render_tascar.h"
#include "libov/src/ov_types.h"
#include <QObject>
#include <QString>
#if defined(__APPLE__) || defined(__linux__)
#include <QSocketNotifier>
#endif

class OvWorker : public QObject {
  Q_OBJECT

public:
  OvWorker(QObject* parent = nullptr);
  ~OvWorker();
  const QString& getToken();

public slots:
  void start(const QString& frontend = "ds");
  void stop();
  void setToken(const QString& token);

signals:
  void started(const QString& frontend);
  void stopped();
  void errored(const QString& error);
  void tokenChanged(const QString& token);

private:
  std::shared_ptr<ov_client_base_t> client;
  std::shared_ptr<ov_render_tascar_t> renderer;
  QString token;
  bool isRunning;

#if defined(__APPLE__) || defined(__linux__)
public:
  // Unix signal handlers.
  static void hupSignalHandler(int unused);
  static void termSignalHandler(int unused);
  static void intSignalHandler(int unused);
public slots:
  void handleSigHup();
  void handleSigInt();
  void handleSigTerm();

private:
  static int sighupFd[2];
  static int sigtermFd[2];
  static int sigintFd[2];
  QSocketNotifier* snHup;
  QSocketNotifier* snTerm;
  QSocketNotifier* snInt;
#endif
};

#endif