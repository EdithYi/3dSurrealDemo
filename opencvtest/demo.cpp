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
//-----------------------------------���궨�岿�֡�--------------------------------------------  
//  ����������һЩ������  
//------------------------------------------------------------------------------------------------  
#define WINDOW_NAME1 "Corner detection original"        //Ϊ���ڱ��ⶨ��ĺ�  
#define WINDOW_NAME2 "Corner detection gray"        //Ϊ���ڱ��ⶨ��ĺ�  

//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------
//		������ȫ�ֱ�������
//-----------------------------------------------------------------------------------------------
Mat g_srcImage, g_srcImage1, g_grayImage;
int thresh = 30; //��ǰ��ֵ
int max_thresh = 175; //�����ֵ


//-----------------------------------��ȫ�ֺ����������֡�--------------------------------------
//		������ȫ�ֺ�������
//-----------------------------------------------------------------------------------------------
void on_CornerHarris(int, void*);//�ص�����
static void ShowHelpText();

//-----------------------------------��main( )������--------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼִ��
//-----------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//��0���ı�console������ɫ
	system("color 3F");

	//��0����ʾ��������
	ShowHelpText();

	//��1������ԭʼͼ�����п�¡����
	g_srcImage = imread("G:\\open cv\\test\\opencvtest\\opencvtest\\1.jpg", 1);
	if (!g_srcImage.data) { printf("Error reading image, please make sure that the image specified by imread function exists in the directory~�� \n"); return false; }
	imshow("original", g_srcImage);
	g_srcImage1 = g_srcImage.clone();

	//��2������һ�ŻҶ�ͼ
	cvtColor(g_srcImage1, g_grayImage, CV_BGR2GRAY);

	//��3���������ں͹�����
	namedWindow(WINDOW_NAME1, CV_WINDOW_AUTOSIZE);
	createTrackbar(" threshold value : ", WINDOW_NAME1, &thresh, max_thresh, on_CornerHarris);
	//��4������һ�λص����������г�ʼ��
	on_CornerHarris(0, 0);
	//�ԻҶ�ģʽ����ͼ����ʾ
	Mat srcImage = imread("G:\\open cv\\test\\opencvtest\\opencvtest\\1.jpg", 0);
	imshow("gray", srcImage);

	//����Harris�ǵ����ҳ��ǵ�
	Mat cornerStrength;
	cornerHarris(srcImage, cornerStrength, 2, 3, 0.01);

	//�ԻҶ�ͼ������ֵ�������õ���ֵͼ����ʾ  
	Mat harrisCorner;
	threshold(cornerStrength, harrisCorner, 0.00001, 255, THRESH_BINARY);
	imshow("Binary Image renderings after corner detection", harrisCorner);
	Mat contours;
	Canny(srcImage, contours, 125, 350);   //�Ҷ�ͼ //�������//����ֵ  //����ֵ  

	//��Ϊ����������������÷������ر�ʾ ���Ƿ�ת�ڰ�ֵ  
	Mat contoursInv;    //��ת���ͼ��  
	threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);//���ڸ�ֵ������ //�����255  
	imshow("contoursInv", contoursInv);
	waitKey(0);
	return 0;
	
}

//-----------------------------------��on_HoughLines( )������--------------------------------
//		�������ص�����
//----------------------------------------------------------------------------------------------

void on_CornerHarris(int, void*)
{
	//---------------------------��1������һЩ�ֲ�����-----------------------------
	Mat dstImage;//Ŀ��ͼ
	Mat normImage;//��һ�����ͼ
	Mat scaledImage;//���Ա任��İ�λ�޷������͵�ͼ

	//---------------------------��2����ʼ��---------------------------------------
	//���㵱ǰ��Ҫ��ʾ������ͼ���������һ�ε��ô˺���ʱ���ǵ�ֵ
	dstImage = Mat::zeros(g_srcImage.size(), CV_32FC1);
	g_srcImage1 = g_srcImage.clone();

	//---------------------------��3����ʽ���-------------------------------------
	//���нǵ���
	cornerHarris(g_grayImage, dstImage, 2, 3, 0.04, BORDER_DEFAULT);

	// ��һ����ת��
	normalize(dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(normImage, scaledImage);//����һ�����ͼ���Ա任��8λ�޷������� 

	//---------------------------��4�����л���-------------------------------------
	// ����⵽�ģ��ҷ�����ֵ�����Ľǵ���Ƴ���
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
	//---------------------------��4����ʾ����Ч��---------------------------------
	imshow(WINDOW_NAME1, g_srcImage1);
	imshow(WINDOW_NAME2, scaledImage);

}

//-----------------------------------��ShowHelpText( )������----------------------------------
//		���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//���һЩ������Ϣ
	printf("\n\n\n\t\t\t��Image detection~!��\n\n");
	printf("\n\n\n\tPlease adjust the scroll bar to observe the image effect~\n\n");
	printf("\n\n\t\t\t\t\t\t\t\t by group 2"
	);
}
/*
//-----------------------------------��ͷ�ļ��������֡�----------------------------------------- 
//    ����������������������ͷ�ļ� 
//---------------------------------------------------------------------------------------------- 
#include <cv.h> 
#include <highgui.h> 
#include "opencv2/core/core.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <iostream>

//-----------------------------------�������ռ��������֡�-------------------------------------- 
//    ����������������ʹ�õ������ռ� 
//--------------------------------------------------------------------------------------------- 
using namespace std;
using namespace cv;

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------- 
//    ������ȫ�ֱ������� 
//--------------------------------------------------------------------------------------------- 
int g_nContrastValue; //�Աȶ�ֵ 
int g_nBrightValue;  //����ֵ 
int Thresh = 50;

//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------- 
//    ������ȫ�ֺ������� 
//--------------------------------------------------------------------------------------------- 
static void ContrastAndBright(int);
void   ShowHelpText();

//-----------------------------------������ͼ��IplImageָ�롿---------------------------------- 
//����������ͼ��IplImageָ�� 
//--------------------------------------------------------------------------------------------- 
IplImage* g_pSrcImg = NULL;

IplImage* g_pGrayImage = NULL;

IplImage* g_pCannyImg = NULL;

IplImage *g_pBinaryImage = NULL;

IplImage *g_pDstImage = NULL;



//��������Ӧ����

void onTrackerSlid(int thresh)

{

	cvCanny(g_pGrayImage, g_pCannyImg, (float)thresh, (float)thresh * 3, 3);

	cvShowImage("Canny Image", g_pCannyImg);

}

//-----------------------------��on_trackbar( )������------------------------------------ 
//    ������ͼ���ֵ����ֵ�ص����� 
//----------------------------------------------------------------------------------------------- 
void on_trackbar(int pos)
{
	// תΪ��ֵͼ  
	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);
	// ��ʾ��ֵͼ  
	cvShowImage("Binary Image", g_pBinaryImage);
}


//-----------------------------------��main( )������-------------------------------------------- 
//    ����������̨Ӧ�ó������ں�������������￪ʼִ�� 
//----------------------------------------------------------------------------------------------- 
int main(int argc, char** argv)

{
	system("color 5F");
	ShowHelpText();
	if (argc != 2)

		argv[1] = "G:\\open cv\\test\\opencvtest\\opencvtest\\1";

	//�趨�ԱȶȺ����ȵĳ�ֵ

	g_nContrastValue = 80;
	g_nBrightValue = 80;

	//����ͼ��ǿ��ת��ΪGray

	if ((g_pSrcImg = cvLoadImage(argv[1], 1)) != 0)

	{

		cout << "Press ESC to Quit" << endl;

		//����ɫ�ռ���RGBת��ΪGray

		g_pGrayImage = cvCreateImage(cvGetSize(g_pSrcImg), 8, 1);

		cvCvtColor(g_pSrcImg, g_pGrayImage, CV_RGB2GRAY);

		// ������ֵͼ  
		g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);

		//canny��Ե���

		g_pCannyImg = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);

		cvNamedWindow("ContrastandBrightnessWnd", CV_WINDOW_AUTOSIZE);

		//�����켣��

		cvCreateTrackbar("Contrast��", "ContrastandBrightnessWnd", &g_nContrastValue, 300, ContrastAndBright);
		cvCreateTrackbar("Brightness��", "ContrastandBrightnessWnd", &g_nBrightValue, 200, ContrastAndBright);


		//���ûص����� 
		ContrastAndBright(g_nContrastValue);
		ContrastAndBright(g_nBrightValue);

		//��������

		cvNamedWindow("Source Image", CV_WINDOW_AUTOSIZE);

		cvNamedWindow("Gray Image", CV_WINDOW_AUTOSIZE);

		cvNamedWindow("Canny Image", CV_WINDOW_AUTOSIZE);

		cvNamedWindow("Binary Image", CV_WINDOW_AUTOSIZE);


		// ������    
		int nThreshold = 0;
		cvCreateTrackbar("Threshold��", "Binary Image", &nThreshold, 254, on_trackbar);
		on_trackbar(1);

		//��ӻ����������ڱ�Ե������ֵ

		onTrackerSlid(Thresh);

		cvShowImage("Gray Image", g_pCannyImg);

		cvCreateTrackbar("Threshold", "Canny Image", &Thresh, 300, onTrackerSlid);

		//��ʾͼ��

		cvShowImage("Source Image", g_pSrcImg);

		cvShowImage("Gray Image", g_pGrayImage);

		//����ͼ��

		cvSaveImage("Gray_Image.jpg", g_pGrayImage);

		cvSaveImage("Canny_Image.jpg", g_pCannyImg);

		cvSaveImage("Binary Image.jpg", g_pBinaryImage);


		//�ȴ���"ESC"���˳�

		while (1)

			if (cvWaitKey(100) == 27)

				break;

		//���ٴ���

		cvWaitKey(0);

		cvDestroyWindow("Source Image");

		cvDestroyWindow("Canny Image");

		cvDestroyWindow("Gray Image");

		cvDestroyWindow("Binary Image");

		//�ͷ�ͼ��

		cvReleaseImage(&g_pGrayImage);

		cvReleaseImage(&g_pCannyImg);

		cvReleaseImage(&g_pSrcImg);

		cvReleaseImage(&g_pBinaryImage);

		return 0;

	}

	return -1;

}


//-----------------------------��ContrastAndBright( )������------------------------------------ 
//    �������ı�ͼ��ԱȶȺ�����ֵ�Ļص����� 
//--------------------------------------------------------------------------------------------- 
static void ContrastAndBright(int)
{

	// ����forѭ����ִ������ dstImg(i,j) = a*srcImg(i,j) + b 
	Mat srcImg, dstImg;
	srcImg = imread("lena.jpg");
	if (!srcImg.data)
	{
		printf("��ȡsrcImgͼƬ����~�� \n");
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

	// ��ʾͼ�� 
	imshow("ContrastandBrightnessWnd", dstImg);
}

//-----------------------------------��ShowHelpText( )������---------------------------------- 
//         ���������һЩ������Ϣ 
//---------------------------------------------------------------------------------------------- 
void ShowHelpText()
{

	printf("\n\n\t\t\t                ����ƽ̨Ϊwin32 VS2013\n");
	printf("\n\n\t\t\t                OpenCVһ��ֵ����ѧϰ��ǿ���Ӿ���\n");
	printf("\n\n\t\t\t                ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ------------------------------------------------------------------------------------------------------------------\n");
}#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main()
{
	Mat image2 = imread("G:\\open cv\\test\\opencvtest\\opencvtest\\tp2.jpg");
	imshow("��ʾͼƬ2", image2);
	waitKey(0);
	return 0;
}
import cv2
{
	image = cv2.imread("G:\\open cv\\test\\opencvtest\\opencvtest\\tp2.jpg");
	cv2.imshow("����", image);
	image = cv2.Canny(image, 200, 300);
	cv2.imshow("����2", image);
	cv2.waitKey(0);
}*/