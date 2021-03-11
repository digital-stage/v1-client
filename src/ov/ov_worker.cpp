#include "ov_worker.h"
#include "libov/src/ov_client_digitalstage.h"
#include "libov/src/ov_client_orlandoviols.h"
#include "libov/src/ov_render_tascar.h"
#include <iostream>
#include <stdexcept>

OvWorker::OvWorker() : client(nullptr), frontendType("ds"), isRunning(false) {}

void OvWorker::start()
{
  if(isRunning)
    stop();

  std::cout << "Starting" << std::endl;

  const std::string lobby(
      ovstrrep("http://oldbox.orlandoviols.com/", "\\/", "/"));
  const int pinglogport(0);

  try {
    const std::string mac = getmacaddr();
    ov_render_tascar_t renderer(mac, pinglogport);
    if(frontendType == "ds") {
      auto* dsClient = new ov_client_digitalstage_t(
          renderer, "wss://single.dstage.org/api/endpoint/");
      dsClient->set_token(token.toStdString());
      client = dsClient;
    } else {
      client = new ov_client_orlandoviols_t(renderer, lobby);
    }
    isRunning = true;
    client->start_service();
    emit started(frontendType);
  }
  catch(const std::exception& e) {
    QString error(e.what());
    emit errored(error);
  }
}

void OvWorker::stop()
{
  if(isRunning) {
    std::cout << "Finsished" << std::endl;
    client->stop_service();
    delete client;
    isRunning = false;
    emit stopped();
  }
}

void OvWorker::setFrontend(const QString& frontend)
{
  frontendType = frontend;
  emit frontendChanged(frontend);
  if(isRunning) {
    try {
      stop();
      start();
    } catch(const std::exception& e) {
      QString error(e.what());
      errored(error);
    }
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