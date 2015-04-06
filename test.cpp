#include <qilqay.h>

PROGRAM START

int x,y;
for (int i=0; i<5; i++) {
    getposition(x,y,"Seleccione una ubicación");
    circle(x,y,100);
}

END
