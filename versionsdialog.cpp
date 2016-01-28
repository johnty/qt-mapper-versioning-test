#include "versionsdialog.h"
#include "ui_versionsdialog.h"

VersionsDialog::VersionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionsDialog)
{
    ui->setupUi(this);
}

VersionsDialog::~VersionsDialog()
{
    delete ui;
}
