#include <stdio.h>
#include <ctype.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "cvblob/cvblob.h"

#include "spritecutter.h"

using namespace cvb;

CSprite::CSprite(IplImage *img, const char *prefix, const char *outDir, const char *imgFormat)    {

    if (img == NULL)    {
        fprintf(stderr, "Couldn't load image file.\n");
        exit(0);
    }

    gImg = img;
    gImgFormat = DEFAULT_FORMAT;
    gOutDir = (outDir == NULL) ? DEFAULT_OUT_DIR : outDir;
    gPrefix = (prefix == NULL) ? DEFAULT_PREFIX : prefix;
    
    gLabelImg = cvCreateImage(cvGetSize(gImg), IPL_DEPTH_LABEL, 1);
    gRenderedLabelImg = cvCreateImage(cvGetSize(gImg), IPL_DEPTH_8U, 3); 

    findBlobs();
}

CSprite::~CSprite() {
    cvReleaseImage(&gLabelImg);
    cvReleaseImage(&gRenderedLabelImg);
}

void CSprite::findBlobs()    {
  
    IplImage *tmpImg; 
    tmpImg = cvCreateImage(cvGetSize(gImg), 8, 1);
  
    //cvLabel needs a 1 channel binary image. If the image is not 1 channel already, convert it.  
    if (gImg->nChannels != 1)   { 
        cvCvtColor(gImg, tmpImg, CV_RGB2GRAY);
    }

    //Apply a binary threshold and slight gaussian blur to eliminate jaggies
    cvThreshold(tmpImg, tmpImg, 254, 255, CV_THRESH_BINARY_INV);
    cvSmooth(tmpImg, tmpImg, CV_GAUSSIAN, 3, 0, 3, 0);

    int result = cvLabel(tmpImg, gLabelImg, gBlobs);
    
    if (result == 0)    {
        printf("cvLabel failed\n");
        exit(0);
    }

    cvReleaseImage(&tmpImg);
}

void CSprite::writeSprites()    {

    char buf[MAXPATH];
    const char *pathstr;
    int sprNum = 0;
    int height, width;

    pathstr = "%s/%s%d.%s";

    for (CvBlobs::const_iterator it = gBlobs.begin(); it!=gBlobs.end(); ++it)   {

        height = it->second->maxy - it->second->miny;
        width =  it->second->maxx - it->second->minx;

        cvSetImageROI(gImg, cvRect(it->second->minx, it->second->miny, width, height));

        sprintf(buf, pathstr, gOutDir, gPrefix, sprNum, gImgFormat);

        if (width * height > MIN_SPRITE_AREA)
            cvSaveImage(buf, gImg);

        sprNum++;
    }

    cvSetImageROI(gImg, cvRect(0, 0, gImg->width, gImg->height));
}

void CSprite::writeXML()    {

    char buf[2048];
    const char *pathstr = "%s/%s.xml";

    int x, y, width, height, i;

    const char *openTag =    "<spritesheet format=\"%s\">\n";
    const char *closeTag =   "</spritesheet>";
    const char *spriteElement = "    <sprite x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\">%s%d.%s</sprite>\n";

    sprintf(buf, pathstr, gOutDir, gPrefix);
    FILE *fp = fopen(buf, "w+");

    if (!fp)    {
        printf("Error opening file\n");
        exit(0);
    }

    fprintf(fp, "<?xml version=\"1.0\"?>\n\n");
    sprintf(buf, openTag, gImgFormat);
    fwrite(buf, 1, strlen(buf), fp);

    i = 0;

    for (CvBlobs::const_iterator it = gBlobs.begin(); it!=gBlobs.end(); ++it)   {
        x = it->second->minx;
        y = it->second->miny;
        width =  it->second->maxx - x;
        height = it->second->maxy - y; 

        sprintf(buf, spriteElement, x, y, width, height, gPrefix, i, gImgFormat);  
        fwrite(buf, 1, strlen(buf), fp);
        
        i++;
    }

    fwrite(closeTag, 1, strlen(closeTag), fp);
    fclose(fp);
    
}

void CSprite::writeCSS()    {
    //TODO
}

void CSprite::writeLabelImage() {

    char buf[MAXPATH];
    const char *pathstr;

    pathstr = "%s/%slabel.%s";
    sprintf(buf, pathstr, gOutDir, gPrefix, gImgFormat);

    cvRenderBlobs(gLabelImg, gBlobs, gRenderedLabelImg, gRenderedLabelImg, CV_BLOB_RENDER_BOUNDING_BOX|CV_BLOB_RENDER_COLOR);
    cvSaveImage(buf, gRenderedLabelImg);
}
