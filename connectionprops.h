#ifndef CONNECTIONPROPS_H
#define CONNECTIONPROPS_H

#include <QWidget>

namespace Ui {
class ConnectionProps;
}

class ConnectionProps : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionProps(QWidget *parent = 0);
    ~ConnectionProps();

private:
    Ui::ConnectionProps *ui;
};

#endif // CONNECTIONPROPS_H
