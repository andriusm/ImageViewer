#ifndef VIEWER_H
#define VIEWER_H

#include "main.h"
#include <GL/glfw.h>

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
        int scrWidth, scrHeight;
        int imgWidth, imgHeight;

        void draw();
        void readImage(char *fname);
        void putImage(float x, float y, float z);
        void loadImageAsTexture(BitmapResource bmp);
};

#endif // VIEWER_H
