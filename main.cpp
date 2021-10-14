
#include <iostream>
#include <Magick++.h>
#include <tuple>
#include <filesystem>
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;
using namespace Magick;
namespace fs = std::filesystem;

const unsigned char MAX_COLOUR = 255;

const int IMAGE_W = 100;
const int IMAGE_H = 100;




struct i_rgb_values {
    int r, g, b;
};


struct s_rgb_values {
    string img_name;
    int a_r, a_g, a_b;
    float dist = 0;
};



vector<i_rgb_values>  return_i_rgb(){
    Image input_image;
    input_image.read("input.jpg");
    Geometry geo = Geometry(50,50);
    geo.aspect(true);
    input_image.resize(geo);

    vector<i_rgb_values> input_rgb;

    for(int y = 0; y<IMAGE_H; y++){
        for(int x = 0; x<IMAGE_W; x++){
            ColorRGB colour = input_image.pixelColor(x,y);

            unsigned char red = colour.red() * MAX_COLOUR;
            unsigned char green = colour.green() * MAX_COLOUR;
            unsigned char blue = colour.blue() * MAX_COLOUR;

            input_rgb.push_back({red,green,blue});
        }
    }
return input_rgb;
}


tuple<unsigned char, unsigned char, unsigned char> average_rgb (Image &image){
    long aver_r = 0;
    long aver_g = 0;
    long aver_b = 0;
const int IMAGE_H_2 = 50;
const int IMAGE_W_2 = 50;


    for(int y = 0; y<IMAGE_H_2; y++){
        for(int x = 0; x<IMAGE_W_2; x++){
            ColorRGB colour = image.pixelColor(x,y);

            unsigned char red = colour.red() * MAX_COLOUR;
            unsigned char green = colour.green() * MAX_COLOUR;
            unsigned char blue = colour.blue() * MAX_COLOUR;

            aver_r += red;
            aver_g += green;
            aver_b += blue;
        }
    }



    unsigned char a_red = aver_r/(IMAGE_W_2*IMAGE_H_2);
    unsigned char a_green = aver_g/(IMAGE_W_2*IMAGE_H_2);
    unsigned char a_blue = aver_b/(IMAGE_W_2*IMAGE_H_2);



    return make_tuple(a_red, a_green, a_blue);
}

vector<s_rgb_values> load_sources(){
    std::string doc = fs::path("./image_cells/");
    vector<s_rgb_values> source_rgb;
    Image image_cell;

    for (const auto & entry : fs::directory_iterator(doc)){


        int a_r;
        int a_g;
        int a_b;
        try{
        image_cell.read(entry.path());


        tie(a_r, a_g, a_b) = average_rgb(image_cell);
        source_rgb.push_back({entry.path(),a_r,a_g,a_b});
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

int formula(int r, int g, int b, int rT, int gT, int bT){

    int result = (r-rT)*(r-rT) + (g-gT)*(g-gT) + (b-bT)*(b-bT);

    return result;
}


bool sort_colour_diff(const s_rgb_values& x, const s_rgb_values& y) { return x.dist < y.dist; }



vector<string> calc_colour_diff(vector<i_rgb_values> input_rgb, vector<s_rgb_values> source_rgb){
// Get list of colour difference for each pixel in the input image
// and store it in source_rgb struct
    vector<string> img_names;
    for(int i = 0; i<IMAGE_H*IMAGE_W; i++){

        for(int j = 0; j < source_rgb.size(); j++){
            source_rgb[j].dist = sqrt(formula(input_rgb[i].r,input_rgb[i].g,input_rgb[i].b,source_rgb[j].a_r,source_rgb[j].a_g,source_rgb[j].a_b));
        }
        sort(source_rgb.begin(), source_rgb.end(), sort_colour_diff);
        img_names.push_back(source_rgb[0].img_name);
    }
return img_names;
}

void construct_image(vector<string> img_names){
        list<Image>img_list;
    Image image;
    for(int i = 0; i<IMAGE_H*IMAGE_W; i++){
        image.read(img_names[i]);
        img_list.push_back(image);
    }
    Montage montagesettings;
    montagesettings.tile("100x100");
    list<Magick::Image> montagelist;
    montageImages(&montagelist, img_list.begin(), img_list.end(), montagesettings);
    writeImages(montagelist.begin(), montagelist.end(), "DONE.PNG");
}


int main(int argc,char **argv)
{

    InitializeMagick(*argv);
    try{
    int i = 0;
    int j = 0;
    for(int y = 0; y<IMAGE_W; y++){
        j++;
        for(int x = 0; x<IMAGE_H; x++){
            i++;
        }
    }
    cout << i << endl;
    cout << j << endl;
    vector<s_rgb_values> source_rgb = load_sources();
    vector<i_rgb_values> input_rgb = return_i_rgb();

    vector<string> img_names = calc_colour_diff(input_rgb, source_rgb);
    construct_image(img_names);


    }
    catch( Exception &error_ )
      {
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
      }
    return 0;
}
