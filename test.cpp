#include <qilqay.h>

PROGRAM

void cross() {
    rectangle(10,10,20,20);
}

START
cross();
int x,y;
getposition(x,y,"here");
circle(x,y,100);

END
