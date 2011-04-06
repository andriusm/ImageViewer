#include "main.h"
#include "viewer.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        cout << "Usage: imgview image.webp" << endl;
        exit(0);
    }

    Viewer viewer;
    if(viewer.init()) return -1;

    viewer.display(argv[1]);

    return 0;
}
