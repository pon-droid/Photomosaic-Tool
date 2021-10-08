#include <iostream>
#include <Magick++.h>
#include <tuple>
#include <filesystem>
using namespace std;
using namespace Magick;
namespace fs = std::filesystem;

const unsigned char MAX_COLOUR = 255;


struct rgb_values {

    int a_r, a_g, a_b;

};



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

vector<rgb_values> load_sources(){
    std::string doc = fs::path("./image_cells/");
    vector<rgb_values> source_rgb;
    for (const auto & entry : fs::directory_iterator(doc)){
        Image image_cell;

        int a_r;
        int a_g;
        int a_b;
        try{
        image_cell.read(entry.path());


        tie(a_r, a_g, a_b) = average_rgb(image_cell);
        source_rgb.push_back({a_r,a_g,a_b});

        cout << a_r << "||" << a_g << "||" << a_b << endl;
        }
        //These below exceptions occur if the file is not an image, it will skip if the file is not an image
        catch(Magick::ErrorMissingDelegate &error){
            continue;
        }
        catch(Magick::ErrorCorruptImage &error){
            continue;
        }

    }

    return source_rgb;
}



int main(int argc,char **argv)
{

    InitializeMagick(*argv);
    try{


    vector<rgb_values> source_rgb = load_sources();

    cout << "NEW" << endl;
    cout << source_rgb[3].a_r << "||" << source_rgb[3].a_g << "||" << source_rgb[3].a_b;


    }
    catch( Exception &error_ )
      {
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
      }
    return 0;
}
