#ifndef _SPRITECUTTER_H_
#define _SPRITECUTTER_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "cvblob/cvblob.h"

#define MAXBUF 1024
#define MAXPATH 1024
#define MIN_SPRITE_AREA 20 //Minimum sprite area in pixels

#define PNG "png"
#define JPG "jpg"
#define GIF "gif"
#define BMP "bmp"
#define DEFAULT_FORMAT "png"
#define DEFAULT_PREFIX "spr_"
#define DEFAULT_OUT_DIR "./"

using namespace cvb;

class CSprite {

    IplImage *gImg;
    IplImage *gLabelImg;
    IplImage *gRenderedLabelImg;
    
    const char* gPrefix;
    const char* gOutDir;
    const char* gImgFormat;

    CvBlobs gBlobs;

    int gThreshold;
    int gBlurRadius;

    void findBlobs();

    public:
    CSprite (IplImage *img, const char* prefix, const char* outDir, const char *imgFormat);
    ~CSprite ();
    

    void writeSprites();
    void writeLabelImage();
    void writeXML();
    void writeCSS();
};

#endif
