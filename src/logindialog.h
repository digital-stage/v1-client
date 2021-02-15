#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    const QString getEmail();
    const QString getPassword();
    void setEmail(QString user);
    void setPassword(QString password);
    void setError(QString error);
    void resetError();

protected slots:
    void on_buttonSignIn_clicked();

signals:
    void logIn(const QString email, const QString password);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
