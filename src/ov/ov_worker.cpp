#include "ov_worker.h"
#include "libov/src/ov_client_digitalstage.h"
#include "libov/src/ov_client_orlandoviols.h"
#include "libov/src/ov_render_tascar.h"
#include <iostream>
#include <stdexcept>
#if defined(__APPLE__) || defined(__linux__)
#include <sys/socket.h>
#endif

#if defined(__APPLE__) || defined(__linux__)
int OvWorker::sighupFd[2];
int OvWorker::sigtermFd[2];
int OvWorker::sigintFd[2];
#endif

OvWorker::OvWorker(QObject* parent)
    : QObject(parent), client(nullptr), isRunning(false)
{
#if defined(__APPLE__) || defined(__linux__)
  // Bind unix signal handlers to qt functions
  if(::socketpair(AF_UNIX, SOCK_STREAM, 0, sighupFd))
    qFatal("Couldn't create HUP socketpair");

  if(::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
    qFatal("Couldn't create TERM socketpair");

  if(::socketpair(AF_UNIX, SOCK_STREAM, 0, sigintFd))
    qFatal("Couldn't create INT socketpair");
  snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
  connect(snHup, SIGNAL(activated(QSocketDescriptor)), this,
          SLOT(handleSigHup()));
  snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
  snInt = new QSocketNotifier(sigintFd[1], QSocketNotifier::Read, this);
  connect(snTerm, SIGNAL(activated(QSocketDescriptor)), this,
          SLOT(handleSigInt()));
#endif

  const std::string mac = getmacaddr();
  const int pinglogport(0);
  renderer = std::make_shared<ov_render_tascar_t>(mac, pinglogport);
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

#if defined(__APPLE__) || defined(__linux__)
static int setup_unix_signal_handlers()
{
  struct sigaction hup, term, intH;

  hup.sa_handler = OvWorker::hupSignalHandler;
  sigemptyset(&hup.sa_mask);
  hup.sa_flags = 0;
  hup.sa_flags |= SA_RESTART;

  if(sigaction(SIGHUP, &hup, 0))
    return 1;

  term.sa_handler = OvWorker::termSignalHandler;
  sigemptyset(&term.sa_mask);
  term.sa_flags = 0;
  term.sa_flags |= SA_RESTART;

  if(sigaction(SIGTERM, &term, 0))
    return 2;

  intH.sa_handler = OvWorker::intSignalHandler;
  sigemptyset(&hup.sa_mask);
  hup.sa_flags = 0;
  hup.sa_flags |= SA_RESTART;

  if(sigaction(SIGINT, &intH, 0))
    return 3;

  return 0;
}

void OvWorker::hupSignalHandler(int)
{
  char a = 1;
  ::write(sighupFd[0], &a, sizeof(a));
}

void OvWorker::termSignalHandler(int)
{
  char a = 1;
  ::write(sigtermFd[0], &a, sizeof(a));
}

void OvWorker::intSignalHandler(int unused) {
  char a = 1;
  ::write(sigintFd[0], &a, sizeof(a));
}

void OvWorker::handleSigTerm()
{
  snTerm->setEnabled(false);
  char tmp;
  ::read(sigtermFd[1], &tmp, sizeof(tmp));

  // do Qt stuff
  std::cout << "SIGTERM" << std::endl;

  snTerm->setEnabled(true);
}

void OvWorker::handleSigHup()
{
  snHup->setEnabled(false);
  char tmp;
  ::read(sighupFd[1], &tmp, sizeof(tmp));

  // do Qt stuff
  std::cout << "SIGHUP" << std::endl;

  snHup->setEnabled(true);
}

void OvWorker::handleSigInt()
{
  snInt->setEnabled(false);
  char tmp;
  ::read(sigintFd[1], &tmp, sizeof(tmp));

  // do Qt stuff
  std::cout << "SIGINT" << std::endl;

  snHup->setEnabled(true);
}

OvWorker::~OvWorker()
{
  if(client) {
    std::cout << "Stopping client" << std::endl;
    client->stop_service();
  }
}
#endif