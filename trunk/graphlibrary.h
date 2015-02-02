#ifndef GRAPHLIBRARY
#define GRAPHLIBRARY

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QDebug>
#include <QMouseEvent>
#include <QToolTip>

#define dx 5
#define dy 5

class Shape {
public:
    virtual void draw(QPainter& painter) = 0;
};

class Circle :public Shape {
private:
    int x,y,r;
public:
    Circle(int x,int y,int r) : x(x), y(y), r(r) {
    }

    void draw(QPainter& painter) {
        painter.drawArc(dx+x-r,dy+y-r,r*2,r*2,0,360*16);
    }
};

class Rectangle :public Shape {
private:
    int x1,y1,x2,y2;
public:
    Rectangle(int x1,int y1,int x2,int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {
    }

    void draw(QPainter& painter) {
        painter.drawRect(dx+x1,dy+y1,x2-x1,y2-y1);
    }
};

class GraphLibrary : public QWidget {
    Q_OBJECT
private:
    QVector<Shape*> shapes;

    void paintEvent(QPaintEvent*) {
        QPainter painter(this);
        painter.drawRect(5,5,600,400);

        QPen pen1(QColor(220,220,220));
        painter.setPen(pen1);

        for (int i=15; i<600; i+=10) painter.drawLine(i,5,i,405);
        for (int i=15; i<400; i+=10) painter.drawLine(5,i,605,i);

        QPen pen2(Qt::red);
        painter.setPen(pen2);

        foreach (Shape* shape, shapes) {
            shape->draw(painter);
        }
    }

    void mousePressEvent(QMouseEvent* e) {

        QString info = QString("%1,%2").arg(e->x()-dx-1).arg(e->y()-dy-1);
        QToolTip::showText( this->mapToGlobal( QPoint( e->x(), e->y() ) ), info  );
    }

public:
    GraphLibrary() : shapes() {
        resize(610,410);
    }

    void circle(int x,int y,int r) {
        shapes.append(new Circle(x,y,r));
    }

    void rectangle(int x1,int y1,int x2,int y2) {
        shapes.append(new Rectangle(x1,y1,x2,y2));
    }
};

#endif // GRAPHLIBRARY

