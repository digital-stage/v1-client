#include "ov_worker.h"
#include "libov/src/ov_client_digitalstage.h"
#include "libov/src/ov_client_orlandoviols.h"
#include "libov/src/ov_render_tascar.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <iostream>
#include <stdexcept>

OvWorker::OvWorker(QObject* parent)
    : QObject(parent), client(nullptr), isRunning(false)
{
  const std::string mac = getmacaddr();
  const int pinglogport(0);
  renderer = std::make_shared<ov_render_tascar_t>(mac, pinglogport);
  renderer->set_runtime_folder(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toStdString() + "/");
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
    client->set_runtime_folder(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toStdString() + "/");
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