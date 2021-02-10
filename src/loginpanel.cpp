#include "loginpanel.h"
#include "ui_loginpanel.h"
#include <QDebug>

LoginPanel::LoginPanel(QMainWindow *parent) :
    QWidget(parent),
    ui(new Ui::LoginPanel)
{
    ui->setupUi(this);
}

LoginPanel::~LoginPanel()
{
    delete ui;
}

void LoginPanel::setEmail(QString email){
    ui->emailEdit->setText(email);
}

void LoginPanel::setPassword(QString password){
    ui->passwordEdit->setText(password);
}

QString LoginPanel::getEmail(){
    ui->emailEdit->text();
}

QString LoginPanel::getPassword(){
    ui->passwordEdit->text();
}

void LoginPanel::setError(QString error) {
    ui->labelError->setText(error);
}

void LoginPanel::resetError() {
    ui->labelError->setText("");
}
