#include <iostream>
#include <Magick++.h>
using namespace std;
using namespace Magick;

int main(int argc,char **argv)
{
     const unsigned char MAX_COLOUR = 255;
    InitializeMagick(*argv);
    try{


    Image image;



    Geometry geo = Geometry(50,50);
    image.read("source.jpg");
    image.scale(geo);
    const int IMAGE_H = image.rows();
    const int IMAGE_W = image.columns();

    image.display();

    for(int y = 0; y<IMAGE_H; y++){
        for(int x = 0; x<IMAGE_W; x++){
            ColorRGB colour = image.pixelColor(x,y);

            unsigned char red = colour.red() * MAX_COLOUR;
            unsigned char green = colour.green() * MAX_COLOUR;
            unsigned char blue = colour.blue() * MAX_COLOUR;


        }
    }

    }
    catch( Exception &error_ )
      {
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
      }
    return 0;
}
