#include <qilqay.h>

PROGRAM START

int y=100;
color(100,125,30);
for (int i=0; i<600; i+=5) {
    clear();
    circle(i,y,50);
    g->repaint();
    usleep(10000);
}

END
