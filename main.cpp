
#include <iostream>
#include <Magick++.h>
#include <tuple>
#include <filesystem>
using namespace std;
using namespace Magick;
namespace fs = std::filesystem;

const unsigned char MAX_COLOUR = 255;

struct i_rgb_values {
    int r, g, b;
};


struct s_rgb_values {
    string img_name = "HI";
    int a_r, a_g, a_b;

};

vector<i_rgb_values>  return_i_rgb(){
    Image input_image;
    input_image.read("input.jpg");
    Geometry geo = Geometry(50,50);
    geo.aspect(true);
    input_image.resize(geo);

    const int IMAGE_W = input_image.rows();
    const int IMAGE_H = input_image.columns();

    vector<i_rgb_values> input_rgb;

    for(int y = 0; y<IMAGE_H; y++){
        for(int x = 0; x<IMAGE_W; x++){
            ColorRGB colour = input_image.pixelColor(x,y);

            unsigned char red = colour.red() * MAX_COLOUR;
            unsigned char green = colour.green() * MAX_COLOUR;
            unsigned char blue = colour.blue() * MAX_COLOUR;

            cout << (int)red << "<<" << (int)green << "<<" << (int)blue << endl;
            input_rgb.push_back({red,green,blue});
        }
    }
return input_rgb;
}


tuple<double, double, double> average_rgb (Image &image){
    double aver_r = 0;
    double aver_g = 0;
    double aver_b = 0;

    const int IMAGE_W = image.rows();
    const int IMAGE_H = image.columns();

    for(int y = 0; y<IMAGE_H; y++){
        for(int x = 0; x<IMAGE_W; x++){
            ColorRGB colour = image.pixelColor(x,y);

            double red = colour.red() * MAX_COLOUR;
            double green = colour.green() * MAX_COLOUR;
            double blue = colour.blue() * MAX_COLOUR;

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

vector<s_rgb_values> load_sources(){
    std::string doc = fs::path("./image_cells/");
    vector<s_rgb_values> source_rgb;
    Image image_cell;
    int i = 0;
    for (const auto & entry : fs::directory_iterator(doc)){


        int a_r;
        int a_g;
        int a_b;
        try{
        image_cell.read(entry.path());


        tie(a_r, a_g, a_b) = average_rgb(image_cell);
        source_rgb.push_back({entry.path(),a_r,a_g,a_b});
        cout << source_rgb[i].img_name << endl;
        cout << a_r << "||" << a_g << "||" << a_b << endl;

        i++;
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


    vector<s_rgb_values> source_rgb = load_sources();
    vector<i_rgb_values> input_rgb = return_i_rgb();

    }
    catch( Exception &error_ )
      {
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
      }
    return 0;
}
