#include <iostream>
#include <Magick++.h>
#include <tuple>
using namespace std;
using namespace Magick;

const unsigned char MAX_COLOUR = 255;



tuple<int, int, int> average_rgb (Image &image){
    int aver_r = 0;
    int aver_g = 0;
    int aver_b = 0;

    const int IMAGE_W = image.rows();
    const int IMAGE_H = image.columns();

    for(int y = 0; y<IMAGE_H; y++){
        for(int x = 0; x<IMAGE_W; x++){
            ColorRGB colour = image.pixelColor(x,y);

            unsigned char red = colour.red() * MAX_COLOUR;
            unsigned char green = colour.green() * MAX_COLOUR;
            unsigned char blue = colour.blue() * MAX_COLOUR;

            aver_r += red;
            aver_g += green;
            aver_b += blue;
        }
    }

    aver_r = aver_r/(IMAGE_W*IMAGE_H);
    aver_g = aver_g/(IMAGE_W*IMAGE_H);
    aver_b = aver_b/(IMAGE_W*IMAGE_H);



    return make_tuple(aver_r, aver_g, aver_b);
}

int main(int argc,char **argv)
{

    InitializeMagick(*argv);
    try{


    Image image;

    int a_r;
    int a_g;
    int a_b;


    image.read("source.jpg");



    tie(a_r, a_g, a_b) = average_rgb(image);

    cout << a_r << "||" << a_g << "||" << a_b << endl;



    }
    catch( Exception &error_ )
      {
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
      }
    return 0;
}
