#ifndef LOGIN_PANEL_H
#define LOGIN_PANEL_H

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class LoginPanel;
}

class LoginPanel : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPanel(QMainWindow *parent = nullptr);
    ~LoginPanel();

public slots:
    QString getEmail();
    QString getPassword();
    void setEmail(QString user);
    void setPassword(QString password);
    void setError(QString error);
    void resetError();

private:
    Ui::LoginPanel *ui;
};

#endif // LOGIN_PANEL_H
