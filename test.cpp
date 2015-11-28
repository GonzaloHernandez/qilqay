#include <qilqay.h>

PROGRAM START

int a,b;
getposition(a,b,"Give me one coordinate");

for (int x=100; x<600; x+=1) {
    clear();
    circle(x,20,20,true);
    msleep(10);
}

END
