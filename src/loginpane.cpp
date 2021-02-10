#include "loginpane.h"
#include "ui_loginpane.h"
#include <QDebug>

LoginPane::LoginPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPane)
{
    ui->setupUi(this);
}

LoginPane::~LoginPane()
{
    delete ui;
}

void LoginPane::setEmail(QString email){
    ui->emailEdit->setText(email);
}

void LoginPane::setPassword(QString password){
    ui->passwordEdit->setText(password);
}

const QString LoginPane::getEmail(){
    return ui->emailEdit->text();
}

const QString LoginPane::getPassword(){
    return ui->passwordEdit->text();
}

void LoginPane::setError(QString error) {
    ui->labelError->setText(error);
}

void LoginPane::resetError() {
    ui->labelError->setText("");
}

void LoginPane::on_buttonSignIn_clicked()
{
    logIn(getEmail(), getPassword());
}
