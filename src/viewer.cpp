#include "main.h"
#include "viewer.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>
#include <webp/decode.h>

Viewer::Viewer() {}

Viewer::~Viewer() {}

int Viewer::init()
{
    glfwInit();
    glfwOpenWindowHint(GLFW_REFRESH_RATE, 30);
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 0);

    if( !glfwOpenWindow( 1280, 1024, 0, 0, 0, 0, 0, 0, GLFW_FULLSCREEN ) )
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowTitle(APP_NAME);
    glfwSwapInterval(1);

    glfwGetWindowSize( &scrWidth, &scrHeight );
    scrHeight = scrHeight > 0 ? scrHeight : 1;
    glViewport( 0, 0, scrWidth, scrHeight );
    scrAspect = (float)scrWidth / (float)scrHeight;

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    position.x = 0;
    position.y = 0;
    position.z = 1;

    rect = { -1,  1, 0,
              1,  1, 0,
              1, -1, 0,
             -1, -1, 0 };

    texCoords = { 0, 0,
                  1, 0,
                  1, 1,
                  0, 1 };

    running = true;

    return 0;
}

void Viewer::draw()
{
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(FOV, scrAspect, V_NEAR, V_FAR);
    gluLookAt(0, 0, 128, 0, 0, 0, 0, 1, 0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    putImage(position.x, position.y, position.z);

    glfwSwapBuffers();
}

void Viewer::display(char *fname)
{
    readImage(fname);
    draw();

    while(running)
    {
        glfwWaitEvents();

        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED);

        if(glfwGetKey(GLFW_KEY_KP_SUBTRACT)) position.z -= 1;
        if(glfwGetKey(GLFW_KEY_KP_ADD)) position.z += 1;
        if(glfwGetKey(GLFW_KEY_LEFT)) position.x += 1;
        if(glfwGetKey(GLFW_KEY_RIGHT)) position.x -= 1;
        if(glfwGetKey(GLFW_KEY_UP)) position.y -= 1;
        if(glfwGetKey(GLFW_KEY_DOWN)) position.y += 1;

        position.z = glfwGetMouseWheel();

        draw();
    }

    glfwTerminate();
}

// --= private methods =--

void Viewer::readImage(char *fname)
{
    int ok;
    void* data = NULL;
    int width, height;
    unsigned char* out = NULL;
    unsigned int data_size = 0;

    FILE* const in = fopen(fname, "rb");
    if (!in) {
      fprintf(stderr, "cannot open input file '%s'\n", fname);
      return;
    }
    fseek(in, 0, SEEK_END);
    data_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    data = malloc(data_size);
    ok = (fread(data, data_size, 1, in) == 1);
    fclose(in);
    if (!ok) {
      free(data);
      return;
    }

    out = WebPDecodeRGB((const uint8_t*)data, data_size, &width, &height);

    imgWidth = width;
    imgHeight = height;
    imgAspect = (float)imgWidth / (float)imgHeight;

    BitmapResource bmp;
    bmp.height = height;
    bmp.width = width;
    bmp.colorBytes = 3; //TODO: change the hardcoded RGB
    bmp.data = out;

    loadImageAsTexture(bmp);

    free(out);
    free(data);
}

void Viewer::putImage(float x, float y, float z)
{
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glVertexPointer(3, GL_FLOAT, 0, rect);

    glPushMatrix();
    glTranslatef(x, y, z);

    glScalef(40*imgAspect, 40/imgAspect, 1);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void Viewer::loadImageAsTexture(BitmapResource bmp)
{
    int colorType = GL_RGB;
    if(bmp.colorBytes==4) colorType = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, 0);

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexImage2D(GL_TEXTURE_2D, 0, bmp.colorBytes, bmp.width, bmp.height, 0, colorType, GL_UNSIGNED_BYTE, bmp.data);
    gluBuild2DMipmaps(GL_TEXTURE_2D, bmp.colorBytes, bmp.width, bmp.height, colorType, GL_UNSIGNED_BYTE, bmp.data);
}
