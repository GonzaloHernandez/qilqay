#include <qilqay.h>
#include <iostream>

using namespace std;

PROGRAM START

bool finish = false;
while ( !finish )
{
    int k = getkey();
    if (k > 0)
    {
        cout << "The pressed key was " << k << endl;
    }
    if (k == 88)  // key x
    {
        finish = true;
    }
    msleep(10);
}

g->close();

END
