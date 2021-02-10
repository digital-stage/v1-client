#ifndef LOGINPANE_H
#define LOGINPANE_H

#include <QWidget>
#include <QString>

namespace Ui {
class LoginPane;
}

class LoginPane : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPane(QWidget *parent = nullptr);
    ~LoginPane();

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
    Ui::LoginPane *ui;
};

#endif // LOGINPANE_H
