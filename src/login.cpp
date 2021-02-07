#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //connect(ui, SIGNAL(ui->buttonBox->accepted()), this, SLOT(Login::closedlg()) );
}

Login::~Login()
{
    delete ui;
}

void Login::closedlg(){
    ;
}

void Login::setUser(QString user){
    ui->userEdit->setText(user);
}

Login::UserId Login::getSettings() const{
    UserId userId{ ui->userEdit->text(), ui->passwordEdit->text()};
    return userId;
}
