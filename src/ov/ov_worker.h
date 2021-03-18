#ifndef OV_WORKER
#define OV_WORKER

#include "libov/src/ov_render_tascar.h"
#include "libov/src/ov_types.h"
#include <QObject>
#include <QProcess>
#include <QString>

class OvWorker : public QObject {
  Q_OBJECT

public:
  explicit OvWorker(QObject* parent = nullptr);
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
  void startWebmixer();

  std::shared_ptr<ov_client_base_t> client;
  std::shared_ptr<ov_render_tascar_t> renderer;
  QString localDataPath;
  QString token;
  bool isRunning;
  QProcess* webmixerProcess;
};

#endif