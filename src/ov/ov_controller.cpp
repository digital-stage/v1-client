#include "ov_controller.h"
#include <iostream>

OvController::OvController()
{
  auto* worker = new OvWorker;
  worker->moveToThread(&workerThread);
  connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

  connect(this, &OvController::starting, worker, &OvWorker::start);
  connect(this, &OvController::stopping, worker, &OvWorker::stop);
  connect(this, &OvController::tokenChanging, worker, &OvWorker::setToken);

  connect(worker, &OvWorker::started, this, &OvController::started);
  connect(worker, &OvWorker::stopped, this, &OvController::stopped);
  connect(worker, &OvWorker::tokenChanged, this, &OvController::tokenChanged);
  connect(worker, &OvWorker::errored, this, &OvController::errored);
  workerThread.start();
}

OvController::~OvController()
{
  workerThread.quit();
  workerThread.wait();
}

void OvController::start(const QString& frontend)
{
  std::cout << "Emit starting..." << std::endl;
  emit starting(frontend);
}

void OvController::stop()
{
  std::cout << "Emit stopping..." << std::endl;
  emit stopping();
}

void OvController::setToken(const QString& token)
{
  std::cout << "Emit changing token..." << std::endl;
  emit tokenChanging(token);
}