#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int showLogin();
    int showStatus();

protected:
    void closeEvent(QCloseEvent * event) override;
    int loadSettings();
    int saveSettings(QString user, QString pw);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void exit();

private:
    Ui::MainWindow *ui;
    Login *mylogin;
    QString mySettingsFile;
    QString myUsername;
    QString myId;
    QSystemTrayIcon *trayIcon;
    QMenu *loginMenu;
    QMenu *statusMenu;

    bool trayWarning = true;
};
#endif // MAINWINDOW_H
