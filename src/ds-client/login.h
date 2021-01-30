#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    struct UserId {
        QString User;
        QString ID;
    };

public slots:
    UserId getSettings() const;
    void setUser(QString user);
    void closedlg();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
