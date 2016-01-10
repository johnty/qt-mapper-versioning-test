#include "linkview.h"
#include "ui_linkview.h"
#include <QDebug>

LinkView::LinkView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkView)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    //don't put in scroll bars
    ui->links->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->links->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    ui->links->setScene(scene);
    //scene->setSceneRect((ui->links->viewport()->rect()));
    ui->links->setDragMode(QGraphicsView::NoDrag);

    ui->checkBox->setVisible(false);

    row_height = 20;

    //drawTest();
}

LinkView::~LinkView()
{
    delete ui;
}

void LinkView::clear()
{
//    scene->clear();
}

void LinkView::enableMapBtn(bool enabled)
{
    ui->pushButtonMap->setEnabled(enabled);
}

void LinkView::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    //scene->setSceneRect((ui->links->viewport()->rect()));
    //scene->setSceneRect(-10, -10, ui->links->width(), ui->links->height());
    drawTest();

    qDebug() <<"linkview viewport rect = " << ui->links->viewport()->rect();

}

void LinkView::addLink(int yL, int yR, int direction)
{
    printf("LINKVIEW: adding link\n");
    //yL = 100;
    //yR = 200;
    QPainterPath *path = new QPainterPath();
//    float xL = ui->links->viewport()->rect().left();
//    float xR = ui->links->viewport()->rect().right();
    float width = ui->links->width();
    path->moveTo(width*-0.5, yL);
    path->cubicTo(width*0.5, yL, width*0.5, yR, width*1.5, yR);

//    scene->addRect(ui->links->viewport()->rect());
//    QPen *pen = new QPen(QBrush()0)
    scene->addPath(*path, QPen(Qt::black, 2));
    QPointF endPoint, arrowP1, arrowP2;
    if (direction) {
        endPoint = QPointF(width * -0.5, yL);
        arrowP1 = endPoint + QPointF(8.66025403784439, 5);
        arrowP2 = endPoint + QPointF(8.66025403784439, -5);
    }
    else {
        endPoint = QPointF(width * 1.5, yR);
        arrowP1 = endPoint + QPointF(-8.66025403784439, 5);
        arrowP2 = endPoint + QPointF(-8.66025403784439, -5);
    }
    scene->addPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2, QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), QBrush(Qt::black));

    scene->update();

//    painter.setPen(QColor(255, 255, 255));
//    painter.strokePath(*path, painter.pen());
}

void LinkView::drawTest()
{
    printf("LINKVIEW: drawTest\n");
    scene->clear();

    int direction = 0;
    float yL = 0;
    float yR = 0;
    QPainterPath *path = new QPainterPath();
//    float xL = ui->links->viewport()->rect().left();
//    float xR = ui->links->viewport()->rect().right();
    float width = ui->links->width();
    float height = ui->links->height();

    qDebug()<<"width = " <<width << "height = " <<height;

    //path->moveTo(-width/2.0, -height/2.0);

    path->addRect(-width/2.0, -height/2.0, width, height);

    float originX = -width/2.0;
    float originY = -height/2.0;
    float startX = originX;
    float startY;;
    float endX = width/2.0;
    float endY = startY;

    //item 2 to item 3
    startY = originY + row_height * (1.0 + .5);
    endY = originY + row_height * (2.0 + 0.5);
    path->moveTo(startX, startY);
    path->cubicTo(startX+25, startY, endX-25, endY, endX, endY);

    //path->moveTo(0, 0);
    //path->cubicTo(width*0.5, yL, width*0.5, yR, width, yR);

    //scene->addRect(ui->links->viewport()->rect());
//    QPen *pen = new QPen(QBrush()0)
    scene->addPath(*path, QPen(Qt::red, 1));

    QPointF endPoint, arrowP1, arrowP2;
    if (direction) {
        endPoint = QPointF(startX+4, startY);
        arrowP1 = endPoint + QPointF(5, 3);
        arrowP2 = endPoint + QPointF(5, -3);
    }
    else {
        endPoint = QPointF(endX-2, endY);
        arrowP1 = endPoint + QPointF(-5, 3);
        arrowP2 = endPoint + QPointF(-5, -3);
    }
    scene->addPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2, QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), QBrush(Qt::red));

    //chop off scene at exactly the viewport
    scene->setSceneRect(-width/2.0, -height/2.0, width, height);
    scene->update();
}

void LinkView::addLink(int rowFrom, int rowTo)
{

    qDebug()<<"addLink from " << rowFrom << " to " <<rowTo;
    int direction = 0;

    QPainterPath *path = new QPainterPath();
//    float xL = ui->links->viewport()->rect().left();
//    float xR = ui->links->viewport()->rect().right();
    float width = ui->links->width();
    float height = ui->links->height();

    float originX = -width/2.0;
    float originY = -height/2.0;
    float startX = originX;
    float startY;;
    float endX = width/2.0;
    float endY = startY;

    //item 2 to item 3
    startY = originY + row_height * (rowFrom + .5);
    endY = originY + row_height * (rowTo + 0.5);
    path->moveTo(startX, startY);
    path->cubicTo(startX+25, startY, endX-25, endY, endX, endY);

    //path->moveTo(0, 0);
    //path->cubicTo(width*0.5, yL, width*0.5, yR, width, yR);

    //scene->addRect(ui->links->viewport()->rect());
//    QPen *pen = new QPen(QBrush()0)
    scene->addPath(*path, QPen(Qt::red, 1));

    QPointF endPoint, arrowP1, arrowP2;
    if (direction) {
        endPoint = QPointF(startX+4, startY);
        arrowP1 = endPoint + QPointF(5, 3);
        arrowP2 = endPoint + QPointF(5, -3);
    }
    else {
        endPoint = QPointF(endX-2, endY);
        arrowP1 = endPoint + QPointF(-5, 3);
        arrowP2 = endPoint + QPointF(-5, -3);
    }
    scene->addPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2, QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), QBrush(Qt::red));

    //chop off scene at exactly the viewport
    scene->setSceneRect(-width/2.0, -height/2.0, width, height);
    scene->update();

}

void LinkView::on_pushButtonMap_clicked()
{
    drawTest();
    Q_EMIT sendMapBtn();
}
