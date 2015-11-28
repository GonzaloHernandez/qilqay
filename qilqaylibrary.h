#ifndef QILQAYLIBRARY
#define QILQAYLIBRARY

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QToolTip>
#include <QFileInfo>
#include <QMenu>
#include <QFileDialog>
#include <QWheelEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDesktopServices>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>

#define dx 5
#define dy 5

/**
 * @brief The About class
 */
class About : public QDialog
{
    Q_OBJECT
private:
    QLabel* info[6];

    void mousePressEvent(QMouseEvent*)
    {
        close();
    }

public:
    About(QWidget* p) : QDialog(p)
    {
        setWindowTitle("About of this Application");
        QSize fixedSize(300,150);
        resize(fixedSize);
        QVBoxLayout* layout = new QVBoxLayout();
        setLayout(layout);
        info[0] = new QLabel("Universidad de Nariño");
        info[1] = new QLabel("<a href='http://sonar.udenar.edu.co/qilqay'>http://sonar.udenar.edu.co</a>");
        info[2] = new QLabel("-------------------------------------");
        info[3] = new QLabel("[ Release 25 ]");
        info[4] = new QLabel("GonzaloHernandez@udenar.edu.co");

        QFont font = info[4]->font();
        font.setPointSize(9);

        for (int i=0; i<5; i++)
        {
            info[i]->setFont(font);
            info[i]->setAlignment(Qt::AlignHCenter);
            layout->addWidget(info[i]);
        }

        info[1]->setTextFormat(Qt::RichText);
        info[1]->setTextInteractionFlags(Qt::TextBrowserInteraction);
        info[1]->setOpenExternalLinks(true);

        setMinimumSize(fixedSize);
        setMaximumSize(fixedSize);
        setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

        setModal(true);
    }
};

/**
 * @brief The Shape class
 */
class Shape
{
protected:
    QColor  color;
    bool    fill;
public:
    Shape(QColor color, bool fill=true) : color(color), fill(fill)
    {
    }

    virtual void draw(QPainter& painter) = 0;
};

/**
 * @brief The Line class
 */
class Line :public Shape
{
private:
    int x1,y1,x2,y2;
public:
    Line(int x1,int y1,int x2,int y2,QColor color)
        : Shape(color), x1(x1), y1(y1), x2(x2), y2(y2)
    {
    }

    void draw(QPainter& painter)
    {
        QPen pen(color);
        painter.setPen(pen);
        painter.drawLine(dx+x1,dy+y1,dx+x2,dy+y2);
    }
};

/**
 * @brief The Rectangle class
 */
class Rectangle :public Shape
{
private:
    int x1,y1,x2,y2;
public:
    Rectangle(int x1,int y1,int x2,int y2,QColor color, bool fill=true)
        : Shape(color,fill), x1(x1), y1(y1), x2(x2), y2(y2)
    {
    }

    void draw(QPainter& painter)
    {
        QPen pen(color);
        painter.setPen(pen);
        if (fill) {
            painter.setBrush(QBrush(color));
        }
        else {
            painter.setBrush(QBrush());
        }
        painter.drawRect(dx+x1,dy+y1,x2-x1,y2-y1);
    }
};

/**
 * @brief The Circle class
 */
class Circle :public Shape
{
private:
    int x,y,r;
public:
    Circle(int x,int y,int r,QColor color, bool fill=true)
        : Shape(color,fill), x(x), y(y), r(r)
    {
    }

    void draw(QPainter& painter) {
        QPen pen(color);
        painter.setPen(pen);
        if (fill) {
            painter.setBrush(QBrush(color));
        }
        else {
            painter.setBrush(QBrush());
        }
        painter.drawEllipse(dx+x-r,dy+y-r,r*2,r*2);
    }
};

class Arc :public Shape
{
private:
    int x,y,rh,rv,ai,af;
public:
    Arc(int x,int y,int rh,int rv,int ai,int af,QColor color, bool fill=true)
        : Shape(color,fill), x(x), y(y), rh(rh), rv(rv), ai(ai), af(af)
    {
    }

    void draw(QPainter& painter)
    {
        QPen pen(color);
        painter.setPen(pen);
        if (fill) {
            painter.setBrush(QBrush(color));
            if (af>=ai)
            {
                painter.drawPie(dx+x-rh,dy+y-rv,rh*2,rv*2,ai*16,(af-ai)*16);
            }
            else
            {
                painter.drawPie(dx+x-rh,dy+y-rv,rh*2,rv*2,ai*16,(360+af-ai)*16);
            }
        }
        else {
            painter.setBrush(QBrush());
            if (af>=ai)
            {
                painter.drawArc(dx+x-rh,dy+y-rv,rh*2,rv*2,ai*16,(af-ai)*16);
            }
            else
            {
                painter.drawArc(dx+x-rh,dy+y-rv,rh*2,rv*2,ai*16,(360+af-ai)*16);
            }
        }
    }
};

class Image :public Shape
{
private:
    int x,y;
    QImage img;
public:
    Image(int x,int y,QString filename)
        : Shape(Qt::black), x(x), y(y), img(QImage(filename))
    {
    }

    void draw(QPainter& painter)
    {
        painter.drawImage(x,y,img);
    }
};

/**
 * @brief The GraphLibrary class
 */
class GraphLibrary : public QWidget
{
    Q_OBJECT
    friend class About;
private:
    int     width,height;
    bool    gridvisible;
    int     gridscale;
    QColor  currentcolor;
    int     xfocus,yfocus;
    QVector<Shape*> shapes;
    About*  about;
    bool    getpositionactive;
    QString message;
    int     posx,posy;
    bool    controler;

    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.drawRect(5,5,width,height);

        if (gridvisible) {
            QPen pen1(QColor(220,220,220));
            painter.setPen(pen1);

            for (int i=dx+gridscale; i<=width; i+=gridscale)
                painter.drawLine(i,5,i,height+dy);
            for (int i=dy+gridscale; i<=height; i+=gridscale)
                painter.drawLine(5,i,width+dx,i);
        }

        foreach (Shape* shape, shapes)
        {
            shape->draw(painter);
        }

        if (xfocus>=0 && yfocus>=0) {
            QPen pen2(Qt::black);
            painter.setPen(pen2);
            painter.drawArc(dx+xfocus-5,dy+yfocus-5,10,10,0,360*16);
        }

        if (getpositionactive)
        {
            QPen pen2(Qt::blue);
            painter.setPen(pen2);
            if (xfocus>=0)
            {
                painter.drawLine(xfocus+dx,0,xfocus+dx,410);
                painter.drawLine(0,yfocus+dy,610,yfocus+dy);
            }
            else
            {
                painter.drawLine(posx,0,posx,410);
                painter.drawLine(0,posy,610,posy);
            }
        }
    }

    void mousePressEvent(QMouseEvent* e)
    {
        if (e->button()==Qt::LeftButton && getpositionactive)
        {
            clicked.wakeAll();
            controler=false;
            getpositionactive=false;
            repaint();
        }
        else if (e->button()==Qt::RightButton)
        {
            showContextMenu(e->pos());
        }
    }

    void mouseReleaseEvent(QMouseEvent* e)
    {
        if (e->button()==Qt::LeftButton)
        {
        }
    }

    void mouseMoveEvent(QMouseEvent* e)
    {
        if (e->modifiers()==Qt::ControlModifier || e->modifiers()==Qt::AltModifier)
        {
            int x = e->x()-dx-1;
            int y = e->y()-dy-1;
            if (e->modifiers()==Qt::AltModifier)
            {
                int lowerx = (x/gridscale)*gridscale;
                int lowery = (y/gridscale)*gridscale;
                int upperx = (x/gridscale+1)*gridscale;
                int uppery = (y/gridscale+1)*gridscale;
                x = (x-lowerx<upperx-x)?lowerx:upperx;
                y = (y-lowery<uppery-y)?lowery:uppery;

                xfocus = x;
                yfocus = y;
                repaint();
            }
            else if (xfocus != 0)
            {
                yfocus = -1;
                xfocus = -1;
                repaint();
            }

            if (getpositionactive)
            {
                posx = x;
                posy = y;
                QString info = QString("%1,%2").arg(posx).arg(posy);
                QToolTip::showText( this->mapToGlobal( QPoint( e->x(), e->y() ) ), message+" ("+info+")"  );
            }
            else
            {
                QString info = QString("%1,%2").arg(x).arg(y);
                QToolTip::showText( this->mapToGlobal( QPoint( e->x(), e->y() ) ), info  );
            }
        }
        else
        {
            if (xfocus != 0)
            {
                yfocus = -1;
                xfocus = -1;
                QToolTip::hideText();
            }

            if (getpositionactive)
            {
                posx = e->x();
                posy = e->y();
                QString info = QString("%1,%2").arg(posx).arg(posy);
                QToolTip::showText( this->mapToGlobal( QPoint( e->x(), e->y() ) ), message+" ("+info+")"  );
                repaint();
            }
        }
    }

    void keyPressEvent(QKeyEvent* e)
    {
        if (e->modifiers()== Qt::ControlModifier)
        {
            if (e->key()==Qt::Key_S)
            {
                exportImage();
            }
            else if (e->key()==Qt::Key_G)
            {
                setGridVisible(!gridvisible);
            }
        }
    }

    void wheelEvent(QWheelEvent* e)
    {
        if (e->modifiers()==Qt::ControlModifier)
        {
            setGridScale(e->delta()>0?1:-1,e->pos());
        }
    }

    void showContextMenu(const QPoint& pos)
    {
        QPoint globalPos = this->mapToGlobal(pos);

        QMenu popMenu;
        popMenu.addAction("Export Image");
        popMenu.addAction("Help");
        popMenu.addSeparator();
        popMenu.addAction("About");

        QAction* selectedItem = popMenu.exec(globalPos);
        if (selectedItem)
        {
            if (selectedItem->text()=="Export Image") exportImage();
            else if (selectedItem->text()=="Help") goToHelp();
            else if (selectedItem->text()=="About")
            {
                about = new About(this);
                about->show();
            }
        }
    }

    void exportImage()
    {
        QString fileName = QFileInfo(QCoreApplication::applicationFilePath()).fileName().append(".png");
        fileName = QFileDialog::getSaveFileName(this,"Select the file name", fileName);
        if (fileName.isEmpty()) return;
        QPixmap pixmap(this->size());
        this->render(&pixmap);
        pixmap.save( fileName );
    }

    void goToHelp()
    {
        QDesktopServices::openUrl(QUrl("http://sonar.udenar.edu.co/qilqay-help"));
    }

public:
    QMutex mutex;
    QWaitCondition clicked;

    GraphLibrary()
        : width(600), height(400), // leftclickpressed(false),
          gridvisible(false), gridscale(10), currentcolor(Qt::red),
          xfocus(-1), yfocus(-1), shapes(), about(NULL),
          getpositionactive(false), message("")
    {
        setWindowTitle("Qilqay - Practice logic programing with graphical exercises");
        setStyleSheet("background-color:white;");
        QSize fixedSize(width+dx*2,height+dy*2);
        setMinimumSize(fixedSize);
        setMaximumSize(fixedSize);
        setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        setMouseTracking(true);
    }

    void setGridVisible(bool gridvisible)
    {
        this->gridvisible = gridvisible;
        repaint();
    }

    void setGridScale(int delta,QPoint mouse)
    {
        if (this->gridvisible && this->gridscale+delta>2)
        {
            this->gridscale += delta;
            repaint();
            QString info = QString("Scale %1").arg(gridscale);
            QToolTip::showText( this->mapToGlobal( mouse ), info );
        }
    }

    void setColor(int c)
    {
        switch(c)
        {
        case 0: currentcolor = Qt::black;  break;
        case 1: currentcolor = Qt::red;  break;
        case 2: currentcolor = Qt::green;  break;
        case 3: currentcolor = Qt::blue;  break;
        case 4: currentcolor = Qt::yellow;  break;
        }
    }

    void setColor(int r,int g,int b) {
        currentcolor = QColor(r,g,b);
    }

    void grid(int scale)
    {
        if (scale>=5)
        {
            gridvisible = true;
            gridscale = scale;
        }
    }

    void line(int x1,int y1,int x2,int y2)
    {
        shapes.append(new Line(x1,y1,x2,y2,currentcolor));
        repaint();
    }

    void rectangle(int x1,int y1,int x2,int y2,bool fill=false)
    {
        shapes.append(new Rectangle(x1,y1,x2,y2,currentcolor,fill));
        repaint();
    }

    void circle(int x,int y,int r,bool fill=false)
    {
        shapes.append(new Circle(x,y,r,currentcolor,fill));
        repaint();
    }

    void oval(int x,int y,int rh,int rv,bool fill=false)
    {
        shapes.append(new Arc(x,y,rh,rv,0,360,currentcolor,fill));
        repaint();
    }

    void arc(int x,int y,int rh,int rv,int ai,int af,bool fill=false)
    {
        shapes.append(new Arc(x,y,rh,rv,ai,af,currentcolor,fill));
        repaint();
    }

    void image(int x,int y,QString filename)
    {
        shapes.append(new Image(x,y,filename));
        repaint();
    }

    void clear()
    {
        shapes.clear();
        repaint();
    }

    void getposition(int& x,int& y,QString msg)
    {
        getpositionactive = true;
        message = msg;
        controler=true;
        mutex.lock();
        clicked.wait(&mutex);
        while(controler);
        mutex.unlock();
        if (xfocus>=0)
        {
            x = xfocus;
            y = yfocus;
        }
        else
        {
            x = posx-dx;
            y = posy-dy;
        }
    }
};

/**
 * @brief The Instructions class
 */
class Instructions :public QThread {
private:
    GraphLibrary* g;
public:
    Instructions(GraphLibrary* g) : g(g)
    {
    }

    void run();
};


#endif // QILQAYLIBRARY


