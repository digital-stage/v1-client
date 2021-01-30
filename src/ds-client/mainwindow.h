#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int showLogin();

private:
    Ui::MainWindow *ui;
    Login *mylogin;
    QString mySettingsFile;
    QString myUsername;
    QString myId;
protected:
    int loadSettings();
    int saveSettings(QString user, QString pw);

};
#endif // MAINWINDOW_H
