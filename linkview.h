#ifndef LINKVIEW_H
#define LINKVIEW_H

#include <QWidget>
#include <QGraphicsView>

namespace Ui {
class LinkView;
}

class LinkView : public QWidget
{
    Q_OBJECT

public:
    explicit LinkView(QWidget *parent = 0);
    ~LinkView();

    void setRowHeight(int rowH) {row_height = rowH;}
    void clear();
    void addLink(int offsetL, int offsetR, int direction);
    void addLink(int rowFrom, int rowTo);
    void drawTest();
    void paintEvent(QPaintEvent* e);

    void enableMapBtn(bool enabled);

Q_SIGNALS:
    void sendMapBtn();

private Q_SLOTS:
    void on_pushButtonMap_clicked();

private:
    Ui::LinkView *ui;
    QGraphicsScene *scene;
    int row_height;
};

#endif // LINKVIEW_H
