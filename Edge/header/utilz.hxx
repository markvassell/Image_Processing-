
#ifndef _utilz__
#define _utilz__
#include "CImg.h"
#include <iostream>
#include <math.h>
#include <ctime>
#define the_size 5
int max(int x, int y){
	if(x > y){
		return x;
	}else{
		return y;
	}
}
bool check_boundary(int m, int n, cimg_library::CImg<> &image){
	if(m>-1 && n>-1 && m<image.height() && n <image.width()){
        return true;
    }else{ 
    	return false;
    }
}
double gauss(double sigma,int x,int y){
	return exp(-((pow(2,x)+(pow(2,y)))/(2*pow(2,sigma))));
}
int get_pixel_value(cimg_library::CImg<> &image,int i, int j){
	return image(j,i,0,0);
}
double get_spatial_weight(int m, int n,int i,int j, int kernel_radius, double **kernel_d){
	return kernel_d[(int)(i-m + kernel_radius)][(int)(j-n + kernel_radius)];
}
double get_similarity(int p, int s, double * gauss_similarity){
	return gauss_similarity[abs(p-s)];
}
void apply_filter(cimg_library::CImg<> &image, int i, int j,int kernel_radius, double **kernel_d, double * gauss_similarity, cimg_library::CImg<> &output){
	if(i>0 && j>0 && i<image.height() && j< image.width()){
		double sum = 0;
		double t_weight = 0;
		int px_value = get_pixel_value(image, i, j);

		int m_max = i + kernel_radius;
		int n_max = j + kernel_radius;
		double weight = 0;

		for (int m = i - kernel_radius; m < m_max; m++){
			for(int n = j - kernel_radius; n < n_max; n++){
				//std::cout<<"hi"<<std::endl;
				if (check_boundary(m,n, image)){
					int intensityKernelPos = get_pixel_value(image,m,n);
					weight = get_spatial_weight(m,n,i,j,kernel_radius, kernel_d) * get_similarity(intensityKernelPos,px_value, gauss_similarity);
					t_weight += weight;
					sum += (weight+ intensityKernelPos);
				}

			}
		}
		int new_px_value = (int)floor(sum/t_weight);
		output(j,i) = new_px_value;
		
	}
}
void run_filter(cimg_library::CImg<> &input, cimg_library::CImg<> &output, int kernel_radius, double **kernel_d, double * gauss_similarity){
	for (int i = 0; i < input.height(); i++){
		for(int j = 0; j < input.width(); j++){
			//std::cout<<"hi"<<std::endl;
			apply_filter(input,i,j, kernel_radius, kernel_d, gauss_similarity, output);
		}
	}

}

void edge_detection(cimg_library::CImg<> &image,cimg_library::CImg<> &new_image,cimg_library::CImg<> &grayscale_image, cimg_library:: CImg<> &output_img, cimg_library:: CImg<> &edege_detected_img_x, cimg_library:: CImg<> &edege_detected_img_y, cimg_library:: CImg<> &edege_detected_img_x_y,cimg_library:: CImg<> &edege_detected_img_x_y2, cimg_library:: CImg<> &edege_detected_img_x2,cimg_library:: CImg<> &edege_detected_img_y2){

	grayscale_image.assign(image.width(),image.height(),1,1);
	output_img.assign(grayscale_image.width()+2,grayscale_image.height()+2,1,1);
	edege_detected_img_x.assign(image.width(),image.height(),1,1,0);
	edege_detected_img_y.assign(image.width(),image.height(),1,1,0);
	edege_detected_img_x_y.assign(image.width(),image.height(),1,1,0);
	edege_detected_img_x2.assign(image.width(),image.height(),1,1,0);
	edege_detected_img_y2.assign(image.width(),image.height(),1,1,0);
	edege_detected_img_x_y2.assign(image.width(),image.height(),1,1,0);

    //grayscale_image.fill(0);

    	if(image.spectrum()>1){
        	int R,G,B;
        	for (int r = 0 ; r< image.height(); r++){
           		for (int c= 0 ; c < image.width(); c++){
                		R = (int)image(c,r,0,0);
                		G = (int)image(c,r,0,1);
                		B = (int)image(c,r,0,2);
                		grayscale_image(c,r,0,0) = (float) (0.2989 * R + 0.5870 * G + 0.1140 * B);
            			}
        		}
    	}else if(image.spectrum()==1){
        	grayscale_image=image;
    	}
	for (int r = 0; r < output_img.height(); r++){
		for(int c = 0; c < output_img.width(); c++){
			if(c == 0){
				output_img(c,r,0,0) = 255;
			}else if(c == output_img.width()-1){
				output_img(c,r,0,0) = 255;
			}else if(r == 0){
				output_img(c,r,0,0) = 255;
			}else if( r == output_img.height()-1){
				output_img(c,r,0,0) = 255;
			}else{
				output_img(c,r,0,0) = grayscale_image(c-1,r-1,0,0);
			}
		}
	}
	output_img.normalize(0,255);

std::clock_t start;
std::clock_t end;
start = std::clock();
//Implimentation of bilateral filter
int sigma_d = 1, sigma_r = 2;

int sigma_max = max(sigma_d,sigma_r);

int kernel_radius = ceil((double)2 * sigma_max);

double _2_sigma_r_squared = 2 * pow(2,sigma_r);

int kernel_size = kernel_radius * 2 + 1;

double **kernel_d = new double*[kernel_size];

for (int i = 0; i < kernel_size; i++){
	kernel_d[i] = new double[kernel_size];
}

int center = (kernel_size-1)/2;

for (int x = -center; x < -center + kernel_size; x++){
	for(int y = -center; y < -center + kernel_size; y++){
		kernel_d[x+center][y+center] = gauss(sigma_d,x,y);
	}
}

double *gauss_similarity = new double[256];

for(int i = 0; i < 256; i++){
	gauss_similarity[i] = exp((double)-((i)/_2_sigma_r_squared));
}

new_image.assign(output_img.width(),output_img.height(),1,1);
//I.assign(output_img);

run_filter(output_img, new_image, kernel_radius, kernel_d, gauss_similarity);
        
end = std::clock();
std::cout << "Time taken to run the complete bilateral filter: " << (end - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

(output_img,new_image).display();


/*output_img.blur_bilateral(output_img,3,6);
output_img.display();*/

//Gradiant Magnitude with 3 by 3
	double x[3][3] = {	{0.25,0,-0.25},
				{0.50,0,-0.50},
				{0.25,0,-0.25}};

	double y[3][3] = {	{0.25,0.50,0.25},
				{0,0,0},
				{-0.25,-0.50,-0.25}};

	float pix_x = 0, pix_y = 0, value = 0;
	double theta = 0;
	//printf("value1 = %f", value);
	for (int j = 1; j < output_img.height()-1; j++){
		for(int i = 1; i < output_img.width()-1; i++){
			//printf("Hello");
/*
			pix_x = (x[0][0]*output_img(i-1,j-1,0,0))
				+(x[0][1]*output_img(i,j-1,0,0))
				+(x[0][2]*output_img(i+1,j-1,0,0))
				+(x[1][0]*output_img(i-1,j,0,0))
				+(x[1][1]*output_img(i,j,0,0))
				+(x[1][2]*output_img(i+1,j,0,0))
				+(x[2][0]*output_img(i-1,j+1,0,0))
				+(x[2][1]*output_img(i,j+1,0,0))
				+(x[2][2]*output_img(i+1,j+1,0,0));

			//printf("%f -> xval", pix_x);
			pix_y = (y[0][0]*output_img(i-1,j-1,0,0))
				+(y[0][1]*output_img(i,j-1,0,0))
				+(y[0][2]*output_img(i+1,j-1,0,0))
				+(y[1][0]*output_img(i-1,j,0,0))
				+(y[1][1]*output_img(i,j,0,0))
				+(y[1][2]*output_img(i+1,j,0,0))
				+(y[2][0]*output_img(i-1,j+1,0,0))
				+(y[2][1]*output_img(i,j+1,0,0))
				+(y[2][2]*output_img(i+1,j+1,0,0));*/

				pix_x = (x[0][0]*new_image(i-1,j-1,0,0))
				+(x[0][1]*new_image(i,j-1,0,0))
				+(x[0][2]*new_image(i+1,j-1,0,0))
				+(x[1][0]*new_image(i-1,j,0,0))
				+(x[1][1]*new_image(i,j,0,0))
				+(x[1][2]*new_image(i+1,j,0,0))
				+(x[2][0]*new_image(i-1,j+1,0,0))
				+(x[2][1]*new_image(i,j+1,0,0))
				+(x[2][2]*new_image(i+1,j+1,0,0));

			//printf("%f -> xval", pix_x);
			pix_y = (y[0][0]*new_image(i-1,j-1,0,0))
				+(y[0][1]*new_image(i,j-1,0,0))
				+(y[0][2]*new_image(i+1,j-1,0,0))
				+(y[1][0]*new_image(i-1,j,0,0))
				+(y[1][1]*new_image(i,j,0,0))
				+(y[1][2]*new_image(i+1,j,0,0))
				+(y[2][0]*new_image(i-1,j+1,0,0))
				+(y[2][1]*new_image(i,j+1,0,0))
				+(y[2][2]*new_image(i+1,j+1,0,0));


			value = sqrt((pix_x*pix_x)+(pix_y*pix_y));
			theta = atan2(pix_y,pix_x);
			edege_detected_img_x(i-1,j-1,0,0) = pix_x;
			edege_detected_img_y(i-1,j-1,0,0) = pix_y;
			edege_detected_img_x_y(i-1,j-1,0,0) = value;
			theta = (theta/3.14)*180;
			if(theta < 0){
				theta = 360 + theta;
			}

			//std::cout<<"theta = "<<theta<<" degrees"<<std::endl;
			//HSVtoRGB(H,S,V)
			//H = theta/360    S = 1   V = 1
		}
	}
	for (int j = 1; j < output_img.height()-1; j++){
		for(int i = 1; i < output_img.width()-1; i++){
			//printf("Hello");

			pix_x = (x[0][0]*edege_detected_img_x(i-1,j-1,0,0))
				+(x[0][1]*edege_detected_img_x(i,j-1,0,0))
				+(x[0][2]*edege_detected_img_x(i+1,j-1,0,0))
				+(x[1][0]*edege_detected_img_x(i-1,j,0,0))
				+(x[1][1]*edege_detected_img_x(i,j,0,0))
				+(x[1][2]*edege_detected_img_x(i+1,j,0,0))
				+(x[2][0]*edege_detected_img_x(i-1,j+1,0,0))
				+(x[2][1]*edege_detected_img_x(i,j+1,0,0))
				+(x[2][2]*edege_detected_img_x(i+1,j+1,0,0));

			pix_y = (y[0][0]*edege_detected_img_y(i-1,j-1,0,0))
				+(y[0][1]*edege_detected_img_y(i,j-1,0,0))
				+(y[0][2]*edege_detected_img_y(i+1,j-1,0,0))
				+(y[1][0]*edege_detected_img_y(i-1,j,0,0))
				+(y[1][1]*edege_detected_img_y(i,j,0,0))
				+(y[1][2]*edege_detected_img_y(i+1,j,0,0))
				+(y[2][0]*edege_detected_img_y(i-1,j+1,0,0))
				+(y[2][1]*edege_detected_img_y(i,j+1,0,0))
				+(y[2][2]*edege_detected_img_y(i+1,j+1,0,0));


			value = sqrt((pix_x*pix_x)+(pix_y*pix_y));
			edege_detected_img_x2(i-1,j-1,0,0) = pix_x;
			edege_detected_img_y2(i-1,j-1,0,0) = pix_y;
			edege_detected_img_x_y2(i-1,j-1,0,0) = value;

		}
	}



}
void thresh_img(cimg_library::CImg<> &image,cimg_library::CImg<> &output_image){
	output_image.assign(image.width(),image.height(),1,1);
	image.normalize(0,255);
	for (int r = 0; r < image.height(); r++){
		for(int c = 0; c < image.width(); c++){
			//std::cout<<image(c,r,0,0)<<std::endl;
			if (image(c,r,0,0) >= 25){

				output_image(c,r,0,0) = 0;
			}else{
				output_image(c,r,0,0) = 255;
			}
		}
	}
	//image.display();
}
#endif
