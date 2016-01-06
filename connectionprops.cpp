#include "connectionprops.h"
#include "ui_connectionprops.h"

ConnectionProps::ConnectionProps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionProps)
{
    ui->setupUi(this);
}

ConnectionProps::~ConnectionProps()
{
    delete ui;
}
