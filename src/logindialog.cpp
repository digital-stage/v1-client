#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::setEmail(QString email){
    ui->emailEdit->setText(email);
}

void LoginDialog::setPassword(QString password){
    ui->passwordEdit->setText(password);
}

const QString LoginDialog::getEmail(){
    return ui->emailEdit->text();
}

const QString LoginDialog::getPassword(){
    return ui->passwordEdit->text();
}

void LoginDialog::setError(QString error) {
    ui->labelError->setText(error);
}

void LoginDialog::resetError() {
    ui->labelError->setText("");
}

void LoginDialog::on_buttonSignIn_clicked()
{
    logIn(getEmail(), getPassword());
}
