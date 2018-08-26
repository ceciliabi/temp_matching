//-----------------------------------【头文件包含部分】---------------------------------------  
//     描述：包含程序所依赖的头文件  
//----------------------------------------------------------------------------------------------                                                                                      
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
//#include<opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <cmath>
#include <stdio.h>  
#include <stdlib.h> 
#include<vector>
//#include<opencv2\features2d\features2d.hpp>

//-----------------------------------【命名空间声明部分】---------------------------------------  
//     描述：包含程序所使用的命名空间  
//-----------------------------------------------------------------------------------------------    
using namespace cv;
using namespace std;

//-----------------------------------【声明函数与变量部分】---------------------------------------  
//     描述：声明函数与变量  
//-----------------------------------------------------------------------------------------------    

Point MatchingMethod(int, void*, Mat img, Mat T, string s);

int main()
{
	//读取模板图片temp和测试图片test
	string temp_name = "temp";
	//string test_name = "test2_1";
	string test_name = "test";

	Mat temp = imread(temp_name+".jpg");
	Mat test = imread(test_name+".jpg");
	Mat temp_gray, test_gray;

	//灰度图转换
	cv::cvtColor(temp, temp_gray, CV_RGB2GRAY);
	cv::cvtColor(test, test_gray, CV_RGB2GRAY);

	//rgb三通道图提取
	vector<cv::Mat> temp_split;
	split(temp, temp_split);
	cv::Mat temp_b = temp_split.at(0);
	cv::Mat temp_g = temp_split.at(1);
	cv::Mat temp_r = temp_split.at(2);	

	vector<cv::Mat> test_split;
	split(test, test_split);
	cv::Mat test_b = test_split.at(0);
	cv::Mat test_g = test_split.at(1);
	cv::Mat test_r = test_split.at(2);


	//确定模板块&匹配
	Point position, position_r, position_g, position_b;
	int counts = 0;
	
	//imshow("原图", temp);
	position = MatchingMethod(0, 0, test, temp,test_name+"_result.jpg");

	//灰度图匹配
	//position = MatchingMethod(0, 0, test_gray, temp_gray);
	//imshow("temp_gray", temp_gray);

	//r通道匹配
	//position_r = MatchingMethod(0, 0, test_r, temp_r);

	//g通道匹配
	//position_g = MatchingMethod(0, 0, test_g, temp_g);

	//b通道匹配
	//position_b = MatchingMethod(0, 0, test_b, temp_b);

	//if (position_b == position_g && position_g == position_r)
	//{
		cout << position << endl << position_r << endl << position_g << endl << position_b << endl;
		counts = counts + 1;
		cout << counts<<endl<<endl;				
		waitKey(2000);
	//}
	
	waitKey(0);
	return 0;
}


//在img图片寻找模板T 
Point MatchingMethod(int, void*, Mat img, Mat T,string str_name)
{
	Mat img_display,img_t;
	
	img.copyTo(img_display);
	img.copyTo(img_t);
	//cout << "img" << img.cols << "\r" << img.rows << endl;
	//cout << "T" << T.cols << "\r" << T.rows << endl;

	int result_cols = img.cols - T.cols + 1;
	int result_rows = img.rows - T.rows + 1;
	Mat result=Mat(result_cols, result_rows, CV_32FC4);

	matchTemplate(img, T, result, CV_TM_SQDIFF);

	//cout << result<<endl<<endl;

	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;

	rectangle(img_display, matchLoc, Point(matchLoc.x + T.cols, matchLoc.y + T.rows), Scalar(250, 128, 10), 2, 8, 0);
	cout << "minVal: " << minVal << endl;
	
	do
	{
		rectangle(img_t, matchLoc, Point(matchLoc.x + T.cols, matchLoc.y + T.rows), Scalar(255, 255, 255), -1, 4);
		matchTemplate(img_t, T, result, CV_TM_SQDIFF);
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		matchLoc = minLoc;
		rectangle(img_display, matchLoc, Point(matchLoc.x + T.cols, matchLoc.y + T.rows), Scalar(250,128,10), 2, 8, 0);
		cout << "minVal: " << minVal << endl;
	} while (minVal < 4.0e+06);

	//imshow("Origin Image", img_display);
	imwrite(str_name, img_display);
	return matchLoc;
}
