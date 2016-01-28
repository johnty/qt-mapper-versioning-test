#ifndef VERSIONSDIALOG_H
#define VERSIONSDIALOG_H

#include <QDialog>

namespace Ui {
class VersionsDialog;
}

class VersionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VersionsDialog(QWidget *parent = 0);
    ~VersionsDialog();

private:
    Ui::VersionsDialog *ui;
};

#endif // VERSIONSDIALOG_H
