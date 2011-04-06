#ifndef VIEWER_H
#define VIEWER_H

#include "main.h"

class Viewer
{
    public:
        Viewer();
        virtual ~Viewer();

        int init();
        void display(char* fname);

    protected:

    private:
        bool running;
        float imgAspect;
        float scrAspect;
        Point3D position;
        int scrWidth, scrHeight;
        int imgWidth, imgHeight;
        float rect[12];
        float texCoords[8];

        void draw();
        void readImage(char *fname);
        void putImage(float x, float y, float z);
        void loadImageAsTexture(BitmapResource bmp);
};

#endif // VIEWER_H
