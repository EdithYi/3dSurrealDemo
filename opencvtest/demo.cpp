#include <iostream>
#include "opencv2/core.hpp"      
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  

using namespace std;
using namespace cv;
//-----------------------------------¡¾Macro definition¡¿--------------------------------------------  
//  Description£ºMacro definition 
//------------------------------------------------------------------------------------------------
#define WINDOW_NAME_ORIG "Original Image"          
#define WINDOW_NAME_GRAY "Gray Image"  
#define WINDOW_NAME_CORD_COLOR "Corner detection original"   
#define WINDOW_NAME_CORD_GRAY "Corner detection gray"       
#define WINDOW_NAME_BINARY_IMG "Binary Image renderings" 
#define WINDOW_NAME_EDGES "Edges detection gray" 

//-----------------------------------¡¾Global Variables¡¿--------------------------------------
//		Description£ºGlobal variable declaration
//-----------------------------------------------------------------------------------------------
Mat g_srcImage, g_srcImage1, g_grayImage;
int thresh = 30; //threshold
int max_thresh = 175; //max threshold


//-----------------------------------¡¾Global functions¡¿--------------------------------------
//		Description£ºGlobal function  declaration
//-----------------------------------------------------------------------------------------------
void on_CornerHarris(int, void*);//callback
static void ShowHelpText();
void showImageWindow(const String &winname, Mat imageArray);
void showImageScalWindow(const String &winname, Mat imageArray, int rate);

//-----------------------------------¡¾main( )¡¿--------------------------------------------
//		Description£ºEntry function
//-----------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//¡¾0¡¿change console font color
	system("color 3F");

	//¡¾0¡¿show help information
	ShowHelpText();

	//¡¾1¡¿load and clone original image 
	g_srcImage = imread("original/Hunder01.jpg", 1);
	if (!g_srcImage.data) 
	{
		printf("Error reading image, please make sure that the image specified by imread function exists in the directory~£¡ \n");
		return false; 
	}
	showImageWindow(WINDOW_NAME_ORIG, g_srcImage);
	g_srcImage1 = g_srcImage.clone();

	//¡¾2¡¿Convert to gray and show it
	cvtColor(g_srcImage1, g_grayImage, CV_BGR2GRAY);
	showImageWindow(WINDOW_NAME_GRAY, g_grayImage);

	//¡¾3¡¿new window with scroll bar
	namedWindow(WINDOW_NAME_CORD_COLOR, CV_WINDOW_FREERATIO);
	resizeWindow(WINDOW_NAME_CORD_COLOR, g_srcImage.cols / 1, g_srcImage.rows / 1);  //notice: special process to be consistent with other windows
	createTrackbar(" threshold value : ", WINDOW_NAME_CORD_COLOR, &thresh, max_thresh, on_CornerHarris);

	//¡¾4¡¿Initiation, evoke callback function on_CornerHarris()
	on_CornerHarris(0, 0);

	//Harris Corner Detection
	Mat cornerStrength;
	cornerHarris(g_grayImage, cornerStrength, 2, 3, 0.01);

	//get binary image by shresholding on gray image
	Mat harrisCorner;
	threshold(cornerStrength, harrisCorner, 0.00001, 255, THRESH_BINARY);
	showImageWindow(WINDOW_NAME_BINARY_IMG, harrisCorner);

	//edges detection using Canny algorithm
	Mat contours;           // edges output
	int low_valve = 100;    // low threshold value
	int high_valve = 350;   // high threshold value
	Canny(g_grayImage, contours, low_valve, high_valve);

	//Invert to show edges in 255   
	Mat contoursInv;    //output Inverted Contour Image  
	threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);//threshold:128, all values below it will becom 255
	showImageWindow(WINDOW_NAME_EDGES, contoursInv);

	waitKey(0);
	return 0;
	
}
void showImageScalWindow(const String &winname, Mat imageArray, int rate)
{
	if (rate == 0)
		return;
	namedWindow(winname, CV_WINDOW_FREERATIO);
	resizeWindow(winname, imageArray.cols / rate, imageArray.rows / rate);
	imshow(winname, imageArray);
}

void showImageWindow(const String &winname, Mat imageArray)
{
	int rate = 1;
	showImageScalWindow(winname, imageArray, rate);
}


//-----------------------------------¡¾callback function: on_CornerHarris( )¡¿--------------------------------
//		description£ºcallback function for trackbar
//----------------------------------------------------------------------------------------------

void on_CornerHarris(int, void*)
{
	//---------------------------¡¾1¡¿local variable definition-----------------------------
	Mat dstImage;//output image
	Mat normImage;//normalized image
	Mat scaledImage;//8-bit image after scaling, taking absolute value

	//---------------------------¡¾2¡¿initializtion---------------------------------------
	dstImage = Mat::zeros(g_srcImage.size(), CV_32FC1);
	g_srcImage1 = g_srcImage.clone();

	//---------------------------¡¾3¡¿detection-------------------------------------
	//Harris Corner Detection
	cornerHarris(g_grayImage, dstImage, 2, 3, 0.04, BORDER_DEFAULT);

	// normalization and scaling
	normalize(dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(normImage, scaledImage);

	//---------------------------¡¾4¡¿mark corners -------------------------------------
	// draw detected corners
	for (int j = 0; j < normImage.rows; j++)
	{
		for (int i = 0; i < normImage.cols; i++)
		{
			if ((int)normImage.at<float>(j, i) > thresh + 80)
			{
				circle(g_srcImage1, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
		}
	}
	//---------------------------¡¾4¡¿show results ---------------------------------
	showImageWindow(WINDOW_NAME_CORD_COLOR, g_srcImage1);
	showImageWindow(WINDOW_NAME_CORD_GRAY, scaledImage);

}

//-----------------------------------¡¾ShowHelpText( )¡¿----------------------------------
//		description£ºoutput some help information
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//output some help information
	printf("\n\n\n\t\t\t¡¾Image detection~!¡¿\n\n");
	printf("\n\n\n\tPlease adjust the scroll bar to observe the image effect~\n\n");
	printf("\n\n\t\t\t\t\t\t\t\t by group 2"
	);
}
