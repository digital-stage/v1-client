#include "ov_worker.h"
#include "../helper/filedownloader.h"
#include "libov/src/ov_client_digitalstage.h"
#include "libov/src/ov_client_orlandoviols.h"
#include "libov/src/ov_render_tascar.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>
#include <QUrl>
#include <iostream>
#include <stdexcept>

OvWorker::OvWorker(QObject* parent)
    : QObject(parent), client(nullptr), isRunning(false)
{
  localDataPath =
      (QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) +
       QDir::separator());
  QDir dir(localDataPath);
  if(!dir.exists())
    dir.mkpath(localDataPath);
  startWebmixer();
  const std::string mac = getmacaddr();
  const int pinglogport(0);
  renderer = std::make_shared<ov_render_tascar_t>(mac, pinglogport);
  renderer->set_runtime_folder(localDataPath.toStdString());
}

void OvWorker::start(const QString& frontend)
{
  std::cout << "OvWorker::start()" << std::endl;
  if(isRunning)
    stop();

  try {
    if(frontend == "ds") {
      std::cout << "Creating new ds client" << std::endl;
      auto dsClient = std::make_shared<ov_client_digitalstage_t>(
          *renderer, "wss://single.dstage.org/api/endpoint/");
      dsClient->set_token(token.toStdString());
      client = dsClient;
    } else if(frontend == "ov") {
      std::cout << "Creating new ov client" << std::endl;
      const std::string lobby(
          ovstrrep("http://oldbox.orlandoviols.com/", "\\/", "/"));
      client = std::make_shared<ov_client_orlandoviols_t>(*renderer, lobby);
    } else {
      throw std::runtime_error("Unknown frontend type: " +
                               frontend.toStdString());
    }
    client->set_runtime_folder(localDataPath.toStdString());
    isRunning = true;
    client->start_service();
    emit started(frontend);
  }
  catch(const std::exception& e) {
    QString error(e.what());
    emit errored(error);
  }
}

void OvWorker::stop()
{
  std::cout << "OvWorker::stop()" << std::endl;
  if(isRunning) {
    std::cout << "OvWorker::stop() - isRunning" << std::endl;
    client->stop_service();
    isRunning = false;
    emit stopped();
  }
}

void OvWorker::setToken(const QString& value)
{
  token = value;
  emit tokenChanged(value);
}

const QString& OvWorker::getToken()
{
  return token;
}

void OvWorker::startWebmixer()
{
  const QString program("webmixer");
  const QString programFilePath = localDataPath + program;
  if(QFile::exists(programFilePath)) {
    webmixerProcess = new QProcess(this);
    connect(webmixerProcess, &QProcess::errorOccurred, this,
            [](QProcess::ProcessError error) {
              std::cerr << "Could not start webmixer:" << std::endl;
              std::cerr << error << std::endl;
            });
    connect(webmixerProcess, &QProcess::readyReadStandardOutput, this,
            [=]() {
              QString output(webmixerProcess->readAllStandardOutput());
              std::cout << "OUTPUT:" << output.toStdString() << std::endl;
            });
    connect(webmixerProcess, &QProcess::readyReadStandardError, this,
            [=]() {
              QString errorOutput(webmixerProcess->readAllStandardError());
              std::cerr << "ERROR:" << errorOutput.toStdString() << std::endl;
            });
    connect(webmixerProcess,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
              std::cout << "Webmixer finsihed with return code " << exitCode
                        << std::endl;
            });
    webmixerProcess->setWorkingDirectory(localDataPath);
    webmixerProcess->start(programFilePath, QStringList() << "-n"
                                                          << "webmixer");
    webmixerProcess->waitForStarted();
    std::cout << "Started webmixer" << std::endl;
  } else {
    std::cout << "Downloading webmixer..." << std::endl;
    // Download first and then start
    const QUrl url(
        "https://github.com/digital-stage/ov-webmixer/releases/download/"
        "Release/ov-webmixer-macos");
    auto* fileDownloader = new FileDownloader(url, programFilePath, this);
    connect(
        fileDownloader, &FileDownloader::downloaded, this,
        [=](const QString& targetPath) {
          QFile(program).setPermissions(QFileDevice::ExeOwner |
                                        QFileDevice::ReadOwner);
          std::cout << "Starting webmixer..." << std::endl;
          webmixerProcess = new QProcess(this);
          connect(webmixerProcess, &QProcess::errorOccurred, this,
                  [](QProcess::ProcessError error) {
                    std::cerr << "Could not start webmixer:" << std::endl;
                    std::cerr << error << std::endl;
                  });
          connect(webmixerProcess,
                  QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                  [=](int exitCode, QProcess::ExitStatus exitStatus) {
                    std::cout << "Webmixer finsihed with return code "
                              << exitCode << std::endl;
                  });
          webmixerProcess->start(programFilePath, QStringList() << "-n"
                                                                << "webmixer");
          webmixerProcess->waitForStarted();
        });
  }
}
