
#include "utilz.hxx" //contains the rgb2gray function
#include <iostream> //
#include <ctime> //for timing
int main(int argc, char *argv[]){

    //empty templates meaning defauls which in CImg is type double
    cimg_library::CImg<> image;
    cimg_library::CImg<> grayscale_image;
    cimg_library::CImg<> output_image;
    cimg_library::CImg<> edge_Img_x;
    cimg_library::CImg<> edge_Img_y;
    cimg_library::CImg<> edge_Img_x_y;
    cimg_library::CImg<> edge_Img_x2;
    cimg_library::CImg<> edge_Img_y2;
    cimg_library::CImg<> edge_Img_x_y2;
    cimg_library::CImg<> thresh;
    


    //print out the usage if the user enters the wrong number of cmd inputs
    if(argc<2){
        std::cout<<"Usage: executable_name image filename"<<std::endl;
    }

    if(argc>1){
//start the clock for determining execution times
        std::clock_t start;
        start = std::clock();
//lets try to load the image as specified by the user..
        //throw an exception otherwise
        try{
        image.load(argv[1]);
        std::cout << "Time taken to load the image: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
        }catch(std::exception &err){
            std::cout<<"Error loading image.."<<std::endl;
            std::cout<<err.what()<<std::endl;

        }

        //call the function to convert
        start = std::clock();
        edge_detection(image,grayscale_image,output_image,edge_Img_x,edge_Img_y,edge_Img_x_y,edge_Img_x_y2,edge_Img_x2,edge_Img_y2);
        std::cout << "Time taken to convert the rgb image to grayscale: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

        //(image,edge_Img_x,edge_Img_y,edge_Img_x_y,edge_Img_x2,edge_Img_y2,edge_Img_x_y2).display("Original, Edge_x, Edge_y, Magnitude,(Bonus->Sobel filters applied twice)");

	thresh_img(edge_Img_x_y, thresh);
	
	(image, thresh).display("New img");


    }
    //all programs must be written the Unix way
    //ANSI C++ standards will be applied to all assignments
    return 0;

}

