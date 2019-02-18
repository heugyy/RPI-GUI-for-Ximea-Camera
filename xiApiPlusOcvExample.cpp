// xiApiPlusOcvExample.cpp : program opens first camera, captures and displays 40 images

#include <stdio.h>
#include "xiApiPlusOcv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include <gtkmm.h>
#include <cstdlib>
#include <string>
//#include <gtk/gtk.h>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;
//using namespace Gtk;

bool Saveflag=0;
bool Exitflag=0;

void CallBackFunc(int event, int x, int y, int flag, void* userdata)
{
	if(event==EVENT_LBUTTONDOWN)
	{
		if((x>=350)&&(x<=430)&&(y<=390)&&(y>=310))
		{
			Saveflag=1;
		}
		if((x>=700)&&(x<=760)&&(y<=80)&&(y>=20))
		{
			Exitflag=1;
		}
	}
}
char *rand_str(int in_len){
	char *__r=(char*)malloc(in_len+1);
	int i;
	if(!__r)
		return 0;
	srand(time(NULL)+rand());
	for(i=0;i<in_len;i++)
		__r[i]=rand()%10+48;

	__r[i]=0;
	return __r;
}

int main(int argc, char* argv[])
{

	try
	{
		// Sample for XIMEA OpenCV
		xiAPIplusCameraOcv cam;

		// Retrieving a handle to the camera device
		printf("Opening first camera...\n");
		cam.OpenFirst();
		
		//Set exposure
		cam.SetExposureTime(10000); //10000 us = 10 ms
		// Note: The default parameters of each camera might be different in different API versions
		
		printf("Starting acquisition...\nget");
		cam.StartAcquisition(); 
		
		printf("First pixel value \n");

		//#define EXPECTED_IMAGES 40
		namedWindow("Image from camera",1);
		moveWindow("Image from camera",0,0);
		while(!Exitflag)
		{
			Mat cv_mat_image(cam.GetNextImageOcvMat());
			Mat overlayI;
			Mat cv_mat_imageo;
			cv_mat_image.copyTo(cv_mat_imageo);
			cv::resize(cv_mat_imageo,cv_mat_imageo,cv::Size(780,400),0,0,INTER_LINEAR);

			cv_mat_imageo.copyTo(overlayI);
			circle(overlayI, Point(390,350),25,Scalar(125,125,125),3,8,0);
			circle(overlayI, Point(390,350),20,Scalar(125,125,125),CV_FILLED,8,0);
			line(overlayI, Point(710,30),Point(750,70),Scalar(125,125,125),10,2);
			line(overlayI, Point(750,30),Point(710,70),Scalar(125,125,125),10,2);
			addWeighted(overlayI,0.3,cv_mat_imageo,0.7,0,cv_mat_imageo);


			cv::imshow("Image from camera",cv_mat_imageo);			
			waitKey(30);	
			
			setMouseCallback("Image from camera",CallBackFunc,0);
			if(Saveflag)
			{
				//circle flashes when saving
				cv::circle(overlayI, cv::Point(390,350),25,cv::Scalar(125,0,0),3,8,0);
				cv::circle(overlayI, cv::Point(390,350),20,cv::Scalar(125,0,0),CV_FILLED,8,0);
				cv::addWeighted(overlayI,0.3,cv_mat_imageo,0.7,0,cv_mat_imageo);
				imshow("Image from camera",cv_mat_imageo);
				//create a 10-digit random number as the picture name
				char *p=rand_str(10);
			
				char picname[27];
				strcpy(picname,"/home/pi/Desktop/Images");
				//catenate the picture name
				strcat(picname,p);
				strcat(picname,".bmp");
				printf("%s\n",picname);
				//save image
				std::vector<int> compression_param;
				compression_param.push_back(CV_IMWRITE_PNG_COMPRESSION);
				compression_param.push_back(9);
				imwrite(string(picname),cv_mat_image, compression_param);
				Saveflag=0;
			}
			
			if(waitKey(30)==27)
				break;
		}
		destroyAllWindows();
		cam.StopAcquisition();
		cam.Close();
		printf("Done\n");
		cvWaitKey(30);
		
	}
	catch(xiAPIplus_Exception& exp){
		printf("Error:\n");
		exp.PrintError();
		cvWaitKey(2000);
	}
	return 0;
}
