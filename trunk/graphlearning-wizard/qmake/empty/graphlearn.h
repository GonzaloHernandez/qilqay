#ifndef DRIVER
#define DRIVER

#include <graphlibrary.h>

#define START \\
    GraphLibrary* g; \\
    \\
    void circle(int x,int y,int r) { g->circle(x,y,r);  } \\
    void rectangle(int x1,int y1,int x2,int y2) { g->rectangle(x1,y1,x2,y2); } \\
    \\
    int main(int argCount,char* argValues[]) { \\
        QApplication app(argCount,argValues); \\
        g = new GraphLibrary();

#define END \\
        g->show(); \\
        app.exec(); \\
    }


#endif // DRIVER

