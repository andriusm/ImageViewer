#include "main.h"
#include "viewer.h"
#include <stdio.h>
#include <stdlib.h>

Point3D position;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: imgview image.webp\n");
        exit(0);
    }

    Viewer viewer;

    if(viewer.init()) return -1;
    viewer.display(argv[1]);

    return 0;
}
