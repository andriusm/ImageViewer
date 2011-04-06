#ifndef MAIN_H
#define MAIN_H

#define APP_NAME "Image Viewer 0.0.1"

#define FOV      45.0f
#define V_NEAR     0.1f
#define V_FAR      1000.0f

typedef struct
{
    unsigned char colorBytes;
    unsigned int length;
    unsigned int width;
    unsigned int height;
    unsigned char *data;
}BitmapResource;

typedef struct
{
    float x;
    float y;
    float z;
}Point3D;

#endif // MAIN_H
