#include <iostream>
#include <opencv2/core/core.hpp>      
#include <opencv2/highgui/highgui.hpp>   
#include "opencv2/highgui/highgui.hpp"
#include <imgproc/imgproc.hpp> 
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgproc/imgproc.hpp"  
using namespace std;
using namespace cv;
//-----------------------------------【宏定义部分】--------------------------------------------  
//  描述：定义一些辅助宏  
//------------------------------------------------------------------------------------------------  
#define WINDOW_NAME1 "Corner detection original"        //为窗口标题定义的宏  
#define WINDOW_NAME2 "Corner detection gray"        //为窗口标题定义的宏  

//-----------------------------------【全局变量声明部分】--------------------------------------
//		描述：全局变量声明
//-----------------------------------------------------------------------------------------------
Mat g_srcImage, g_srcImage1, g_grayImage;
int thresh = 30; //当前阈值
int max_thresh = 175; //最大阈值


//-----------------------------------【全局函数声明部分】--------------------------------------
//		描述：全局函数声明
//-----------------------------------------------------------------------------------------------
void on_CornerHarris(int, void*);//回调函数
static void ShowHelpText();

//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始执行
//-----------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//【0】改变console字体颜色
	system("color 3F");

	//【0】显示帮助文字
	ShowHelpText();

	//【1】载入原始图并进行克隆保存
	g_srcImage = imread("G:\\open cv\\test\\opencvtest\\opencvtest\\1.jpg", 1);
	if (!g_srcImage.data) { printf("Error reading image, please make sure that the image specified by imread function exists in the directory~！ \n"); return false; }
	imshow("original", g_srcImage);
	g_srcImage1 = g_srcImage.clone();

	//【2】存留一张灰度图
	cvtColor(g_srcImage1, g_grayImage, CV_BGR2GRAY);

	//【3】创建窗口和滚动条
	namedWindow(WINDOW_NAME1, CV_WINDOW_AUTOSIZE);
	createTrackbar(" threshold value : ", WINDOW_NAME1, &thresh, max_thresh, on_CornerHarris);
	//【4】调用一次回调函数，进行初始化
	on_CornerHarris(0, 0);
	//以灰度模式载入图像并显示
	Mat srcImage = imread("G:\\open cv\\test\\opencvtest\\opencvtest\\1.jpg", 0);
	imshow("gray", srcImage);

	//进行Harris角点检测找出角点
	Mat cornerStrength;
	cornerHarris(srcImage, cornerStrength, 2, 3, 0.01);

	//对灰度图进行阈值操作，得到二值图并显示  
	Mat harrisCorner;
	threshold(cornerStrength, harrisCorner, 0.00001, 255, THRESH_BINARY);
	imshow("Binary Image renderings after corner detection", harrisCorner);
	Mat contours;
	Canny(srcImage, contours, 125, 350);   //灰度图 //输出轮廓//低阈值  //高阈值  

	//因为正常情况下轮廓是用非零像素表示 我们反转黑白值  
	Mat contoursInv;    //反转后的图像  
	threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);//低于该值的像素 //将变成255  
	imshow("contoursInv", contoursInv);
	waitKey(0);
	return 0;
	
}

//-----------------------------------【on_HoughLines( )函数】--------------------------------
//		描述：回调函数
//----------------------------------------------------------------------------------------------

void on_CornerHarris(int, void*)
{
	//---------------------------【1】定义一些局部变量-----------------------------
	Mat dstImage;//目标图
	Mat normImage;//归一化后的图
	Mat scaledImage;//线性变换后的八位无符号整型的图

	//---------------------------【2】初始化---------------------------------------
	//置零当前需要显示的两幅图，即清除上一次调用此函数时他们的值
	dstImage = Mat::zeros(g_srcImage.size(), CV_32FC1);
	g_srcImage1 = g_srcImage.clone();

	//---------------------------【3】正式检测-------------------------------------
	//进行角点检测
	cornerHarris(g_grayImage, dstImage, 2, 3, 0.04, BORDER_DEFAULT);

	// 归一化与转换
	normalize(dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(normImage, scaledImage);//将归一化后的图线性变换成8位无符号整型 

	//---------------------------【4】进行绘制-------------------------------------
	// 将检测到的，且符合阈值条件的角点绘制出来
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
	//---------------------------【4】显示最终效果---------------------------------
	imshow(WINDOW_NAME1, g_srcImage1);
	imshow(WINDOW_NAME2, scaledImage);

}

//-----------------------------------【ShowHelpText( )函数】----------------------------------
//		描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//输出一些帮助信息
	printf("\n\n\n\t\t\t【Image detection~!】\n\n");
	printf("\n\n\n\tPlease adjust the scroll bar to observe the image effect~\n\n");
	printf("\n\n\t\t\t\t\t\t\t\t by group 2"
	);
}
/*
//-----------------------------------【头文件包含部分】----------------------------------------- 
//    描述：包含程序所依赖的头文件 
//---------------------------------------------------------------------------------------------- 
#include <cv.h> 
#include <highgui.h> 
#include "opencv2/core/core.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <iostream>

//-----------------------------------【命名空间声明部分】-------------------------------------- 
//    描述：包含程序所使用的命名空间 
//--------------------------------------------------------------------------------------------- 
using namespace std;
using namespace cv;

//-----------------------------------【全局变量声明部分】-------------------------------------- 
//    描述：全局变量声明 
//--------------------------------------------------------------------------------------------- 
int g_nContrastValue; //对比度值 
int g_nBrightValue;  //亮度值 
int Thresh = 50;

//-----------------------------------【全局函数声明部分】-------------------------------------- 
//    描述：全局函数声明 
//--------------------------------------------------------------------------------------------- 
static void ContrastAndBright(int);
void   ShowHelpText();

//-----------------------------------【声明图像IplImage指针】---------------------------------- 
//描述：声明图像IplImage指针 
//--------------------------------------------------------------------------------------------- 
IplImage* g_pSrcImg = NULL;

IplImage* g_pGrayImage = NULL;

IplImage* g_pCannyImg = NULL;

IplImage *g_pBinaryImage = NULL;

IplImage *g_pDstImage = NULL;



//滑动条响应函数

void onTrackerSlid(int thresh)

{

	cvCanny(g_pGrayImage, g_pCannyImg, (float)thresh, (float)thresh * 3, 3);

	cvShowImage("Canny Image", g_pCannyImg);

}

//-----------------------------【on_trackbar( )函数】------------------------------------ 
//    描述：图像二值化阈值回调函数 
//----------------------------------------------------------------------------------------------- 
void on_trackbar(int pos)
{
	// 转为二值图  
	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);
	// 显示二值图  
	cvShowImage("Binary Image", g_pBinaryImage);
}


//-----------------------------------【main( )函数】-------------------------------------------- 
//    描述：控制台应用程序的入口函数，程序从这里开始执行 
//----------------------------------------------------------------------------------------------- 
int main(int argc, char** argv)

{
	system("color 5F");
	ShowHelpText();
	if (argc != 2)

		argv[1] = "G:\\open cv\\test\\opencvtest\\opencvtest\\1";

	//设定对比度和亮度的初值

	g_nContrastValue = 80;
	g_nBrightValue = 80;

	//载入图像，强制转化为Gray

	if ((g_pSrcImg = cvLoadImage(argv[1], 1)) != 0)

	{

		cout << "Press ESC to Quit" << endl;

		//将颜色空间由RGB转化为Gray

		g_pGrayImage = cvCreateImage(cvGetSize(g_pSrcImg), 8, 1);

		cvCvtColor(g_pSrcImg, g_pGrayImage, CV_RGB2GRAY);

		// 创建二值图  
		g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);

		//canny边缘检测

		g_pCannyImg = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);

		cvNamedWindow("ContrastandBrightnessWnd", CV_WINDOW_AUTOSIZE);

		//创建轨迹条

		cvCreateTrackbar("Contrast：", "ContrastandBrightnessWnd", &g_nContrastValue, 300, ContrastAndBright);
		cvCreateTrackbar("Brightness：", "ContrastandBrightnessWnd", &g_nBrightValue, 200, ContrastAndBright);


		//调用回调函数 
		ContrastAndBright(g_nContrastValue);
		ContrastAndBright(g_nBrightValue);

		//创建窗口

		cvNamedWindow("Source Image", CV_WINDOW_AUTOSIZE);

		cvNamedWindow("Gray Image", CV_WINDOW_AUTOSIZE);

		cvNamedWindow("Canny Image", CV_WINDOW_AUTOSIZE);

		cvNamedWindow("Binary Image", CV_WINDOW_AUTOSIZE);


		// 滑动条    
		int nThreshold = 0;
		cvCreateTrackbar("Threshold：", "Binary Image", &nThreshold, 254, on_trackbar);
		on_trackbar(1);

		//添加滑动条来调节边缘检测的阈值

		onTrackerSlid(Thresh);

		cvShowImage("Gray Image", g_pCannyImg);

		cvCreateTrackbar("Threshold", "Canny Image", &Thresh, 300, onTrackerSlid);

		//显示图像

		cvShowImage("Source Image", g_pSrcImg);

		cvShowImage("Gray Image", g_pGrayImage);

		//保存图像

		cvSaveImage("Gray_Image.jpg", g_pGrayImage);

		cvSaveImage("Canny_Image.jpg", g_pCannyImg);

		cvSaveImage("Binary Image.jpg", g_pBinaryImage);


		//等待按"ESC"键退出

		while (1)

			if (cvWaitKey(100) == 27)

				break;

		//销毁窗口

		cvWaitKey(0);

		cvDestroyWindow("Source Image");

		cvDestroyWindow("Canny Image");

		cvDestroyWindow("Gray Image");

		cvDestroyWindow("Binary Image");

		//释放图像

		cvReleaseImage(&g_pGrayImage);

		cvReleaseImage(&g_pCannyImg);

		cvReleaseImage(&g_pSrcImg);

		cvReleaseImage(&g_pBinaryImage);

		return 0;

	}

	return -1;

}


//-----------------------------【ContrastAndBright( )函数】------------------------------------ 
//    描述：改变图像对比度和亮度值的回调函数 
//--------------------------------------------------------------------------------------------- 
static void ContrastAndBright(int)
{

	// 三个for循环，执行运算 dstImg(i,j) = a*srcImg(i,j) + b 
	Mat srcImg, dstImg;
	srcImg = imread("lena.jpg");
	if (!srcImg.data)
	{
		printf("读取srcImg图片错误~！ \n");
		return;
	}
	dstImg = Mat::zeros(srcImg.size(), srcImg.type());

	for (int y = 0; y < srcImg.rows; y++)
	{
		for (int x = 0; x < srcImg.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				dstImg.at<Vec3b>(y, x)[c] = saturate_cast<uchar>((g_nContrastValue*0.01)*(srcImg.at<Vec3b>(y, x)[c]) + g_nBrightValue);
			}
		}
	}

	// 显示图像 
	imshow("ContrastandBrightnessWnd", dstImg);
}

//-----------------------------------【ShowHelpText( )函数】---------------------------------- 
//         描述：输出一些帮助信息 
//---------------------------------------------------------------------------------------------- 
void ShowHelpText()
{

	printf("\n\n\t\t\t                运行平台为win32 VS2013\n");
	printf("\n\n\t\t\t                OpenCV一个值得你学习的强大视觉库\n");
	printf("\n\n\t\t\t                当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ------------------------------------------------------------------------------------------------------------------\n");
}#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main()
{
	Mat image2 = imread("G:\\open cv\\test\\opencvtest\\opencvtest\\tp2.jpg");
	imshow("显示图片2", image2);
	waitKey(0);
	return 0;
}
import cv2
{
	image = cv2.imread("G:\\open cv\\test\\opencvtest\\opencvtest\\tp2.jpg");
	cv2.imshow("测试", image);
	image = cv2.Canny(image, 200, 300);
	cv2.imshow("测试2", image);
	cv2.waitKey(0);
}*/