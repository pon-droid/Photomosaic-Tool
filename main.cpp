
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


string extend = "-0-0";
string x = "x";

struct i_rgb_values {
    int r, g, b;
};


struct s_rgb_values {
    string img_name;
    int a_r, a_g, a_b;
    float dist = 0;
};




vector<i_rgb_values>  return_i_rgb(int G_IMAGE_H, int G_IMAGE_W);
tuple<unsigned char, unsigned char, unsigned char> average_rgb (Image &image);
vector<s_rgb_values> load_sources();
int formula(int r, int g, int b, int rT, int gT, int bT);
list<Image> calc_colour_diff(vector<i_rgb_values> input_rgb, vector<s_rgb_values> source_rgb);
void construct_image(list<Image> img_names, string tile_input, string main_input);
void process_strings(string main_input, string tile_input);


int main(int argc,char **argv)
{

    InitializeMagick(*argv);
    try{
    string tile_input, w_input, h_input;
    cout << "Specify tile dimensions: " << endl;
    cin >> tile_input;
    cout << endl;
    cout << "Registered" << endl;
    cout << endl;
    cout << "Specify output width" << endl;
    cin >> w_input;
    cout << endl;
    cout << "Specify output height" << endl;
    cin >> h_input;
    cout << endl;
    cout << "Registered" << endl;
    cout << endl;
    /*
    process_strings(main_input,tile_input);

    */
    int G_IMAGE_W = stoi(w_input);
    int G_IMAGE_H = stoi(h_input);

    string main_input = w_input + 'x' + h_input;
    tile_input.append(extend);

    vector<s_rgb_values> source_rgb = load_sources();
    vector<i_rgb_values> input_rgb = return_i_rgb(G_IMAGE_H, G_IMAGE_W);

    list<Image> img_names = calc_colour_diff(input_rgb, source_rgb);
    construct_image(img_names, tile_input, main_input);
    cout << "DONE" << endl;

    }
    catch( Exception &error_ )
      {
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
      }
    return 0;
}

vector<i_rgb_values>  return_i_rgb(int G_IMAGE_H, int G_IMAGE_W){
    Image input_image;
    input_image.read("input.jpg");
    Geometry geo = Geometry(G_IMAGE_W,G_IMAGE_H);
    geo.aspect(true);
    input_image.resize(geo);
    cout << "Getting input image values..." << endl;
    vector<i_rgb_values> input_rgb;

    for(int y = 0; y<G_IMAGE_H; y++){
        for(int x = 0; x<G_IMAGE_W; x++){
            ColorRGB colour = input_image.pixelColor(x,y);

            unsigned char red = colour.red() * MAX_COLOUR;
            unsigned char green = colour.green() * MAX_COLOUR;
            unsigned char blue = colour.blue() * MAX_COLOUR;

            input_rgb.push_back({red,green,blue});
        }
    }
    cout << "Finished getting input image values" << endl;
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
    cout << "Loading source images...." << endl;

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
    cout << "Finished processing source images" << endl;
    return source_rgb;

}

int formula(int r, int g, int b, int rT, int gT, int bT){

    int result = (r-rT)*(r-rT) + (g-gT)*(g-gT) + (b-bT)*(b-bT);

    return result;
}


bool sort_colour_diff(const s_rgb_values& x, const s_rgb_values& y) { return x.dist < y.dist; }



list<Image> calc_colour_diff(vector<i_rgb_values> input_rgb, vector<s_rgb_values> source_rgb){
// Get list of colour difference for each pixel in the input image
// and store it in source_rgb struct
    cout << "Calculating colour distances....." << endl;
    Image image;
    list<Image> img_names;
    for(int i = 0; i<input_rgb.size(); i++){

        for(int j = 0; j < source_rgb.size(); j++){
            source_rgb[j].dist = sqrt(formula(input_rgb[i].r,input_rgb[i].g,input_rgb[i].b,source_rgb[j].a_r,source_rgb[j].a_g,source_rgb[j].a_b));
        }
        sort(source_rgb.begin(), source_rgb.end(), sort_colour_diff);
        image.read(source_rgb[0].img_name);
        img_names.push_back(source_rgb[0].img_name);
    }
    cout << "Finished calculating colour distances" << endl;
return img_names;
}

void construct_image(list<Image> img_names, string tile_input, string main_input){
    cout << "Constructing montage..." << endl;
    /*
    list<Image>img_list;
    Image image;
    cout << "Loading images into vector...." << endl;
    for(int i = 0; i<img_names.size(); i++){
        image.read(img_names[i]);
        img_list.push_back(image);
    }
    cout << "Done" << endl;
    */
    Montage montage_settings;
    montage_settings.geometry(tile_input);
    montage_settings.tile(main_input);
    list<Magick::Image> montagelist;
    montageImages(&montagelist, img_names.begin(), img_names.end(), montage_settings);
    cout << "Writing..." << endl;
    writeImages(montagelist.begin(), montagelist.end(), "DONE.PNG");
}
/*
void process_strings(string main_input, string tile_input){
    string sub_w = main_input.substr(0, main_input.find(x));
    string sub_h = main_input.substr(1, main_input.find(x));
    G_IMAGE_W = stoi(sub_w);
    G_IMAGE_H = stoi(sub_h);
    tile_input.append(extend);

}
*/
