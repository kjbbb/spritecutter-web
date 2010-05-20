#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "cvblob/cvblob.h"
#include "spritecutter.h"

using namespace std;
using namespace cvb;

void printHelp(char **argv)    {
    cout << "Usage: " << argv[0] << " [options] arguments" << endl;
}

int main(int argc, char **argv) {

    IplImage *img;
    int pFlag = 0;
    int oFlag = 0;
    int fFlag = 0;
    const char *outDir = NULL;
    const char *prefix = NULL;    
    const char *imagePath = NULL;;
    const char *imgFormat = NULL;
    int c;

    //Parse options
    while ((c = getopt(argc, argv, "ho:p:f:")) != -1)  {
        switch (c)  {
            case 'h':
                printHelp(argv);
                exit(0);
                break;
            case 'o':
                oFlag = 1;
                outDir = optarg;
                break;
            case 'p':
                pFlag = 1;
                prefix = optarg;
                break;
            case 'f':
                fFlag = 1;
                imgFormat = optarg;
                break; 
            case ':':
                fprintf(stderr, "Missing argument: image");
                exit(0);
                break;
            case '?':
                if (optopt == 'o' || optopt == 'p')  {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                printHelp(argv);
                exit(0);
                break;
        }
    }
    
    //Parse argument
    
    if (argv[optind] == NULL)   {
        fprintf(stderr, "No image file\n");
        exit(0);
    }
    imagePath = argv[optind];

    //create image
    img = cvLoadImage(imagePath, 1);
    if (!img)   {
        fprintf(stderr, "Could not load image file: %s\n", imagePath);
    }
    
    CSprite sprite(img, prefix, outDir, imgFormat);

    sprite.writeLabelImage();
    sprite.writeSprites();
    sprite.writeXML();

    cvReleaseImage(&img);
    return 0;
}
