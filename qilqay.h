#ifndef DRIVER
#define DRIVER

#include <graphlibrary.h>

#define BLACK  0
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4

#define PROGRAM \
    GraphLibrary* g; \
    \
    void grid(int scale) { g->grid(scale); } \
    void color(int c) { g->setColor(c); } \
    void line(int x1,int y1,int x2,int y2) { g->line(x1,y1,x2,y2); } \
    void rectangle(int x1,int y1,int x2,int y2) { g->rectangle(x1,y1,x2,y2); } \
    void circle(int x,int y,int r) { g->circle(x,y,r);  } \
    void oval(int x,int y,int rh,int rv) { g->oval(x,y,rh,rv);  } \
    void arc(int x,int y,int rh,int rv,int ai,int af) { g->arc(x,y,rh,rv,ai,af);  } \

#define START \
    int main(int argCount,char* argValues[]) { \
        QApplication app(argCount,argValues); \
        g = new GraphLibrary(); \

#define END \
        g->show(); \
        app.exec(); \
    } \

#endif // DRIVER

