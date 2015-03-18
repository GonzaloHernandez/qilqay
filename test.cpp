#include <qilqay.h>

PROGRAM START

for (int i=100; i<450; i+=50)
{
    circle(i,100,50);
}

color(BLUE);

for (int i=50,j=200; i<400; i+=40,j+=10)
{
    rectangle(i,j,i+100,j+100);
}

END
