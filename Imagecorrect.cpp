
//***************************************************************************
//
// FileName:      Imagecorrect.cpp  
// Author:        gaopc
// Date:      2022/12/30
// Description:   图像矫正
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/12/30    Build
//
//***************************************************************************









#include<iostream>
#include<opencv2/opencv.hpp>
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include <opencv2\imgproc\types_c.h>
#include<opencv2/opencv.hpp>
#include"math.h"
#include "chrono"

#include<iostream>
#include<opencv2/opencv.hpp>
#include<fstream>

using namespace std;
using namespace chrono;
using namespace cv;



#include <iostream>
#include <algorithm>
#include <vector>


#define SW16(x) \
  ((unsigned short)( \
  (((unsigned short)(x) & (unsigned short)0x00ffU) << 8) | \
  (((unsigned short)(x) & (unsigned short)0xff00U) >> 8) ))

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
        return -1;
    }
    long rawDataSize = 0;
    unsigned char* pInRawBuf = NULL;
    //读取RAW图
    pInRawBuf = ReadRawFromFile(argv[1], &rawDataSize);
    //高低字节转换
    unsigned short *pInRawBuf1 = (unsigned short*)pInRawBuf;
    for (unsigned long i = 0; i < filelen/2; i++)
        *pInRawBuf1++ = SW16(*pInRawBuf1);
    
    long m_lBufSize= filelen, m_nWidth, m_nHeight;

    if (m_lBufSize == 3840000)
    {
        m_nWidth = 1200;
        m_nHeight = 1600;
    } // #1 size
    else if (m_lBufSize == 5529600)
    {
        m_nWidth = 1440;
        m_nHeight = 1920;
    }
    else if (m_lBufSize == 960000)
    {
        m_nWidth = 600;
        m_nHeight = 800;
    } // #2 size
    else if (m_lBufSize == 1382400)
    {
        m_nWidth = 720;
        m_nHeight = 960;
    } // #2 size
    else if (m_lBufSize == 5293952)
    {
        m_nWidth = 1402;
        m_nHeight = 1888;
    }
    else if (m_lBufSize == 18374720)
    {
        m_nWidth = 2524;
        m_nHeight = 3640;
    }  // size4
    else if (m_lBufSize == 4593024)
    {
        m_nWidth = 1296;
        m_nHeight = 1772;
    }   // size1.5
    else if (m_lBufSize == 3211264)
    {
        m_nWidth = 1024;
        m_nHeight = 1568;
    }

    

 

    
    cv::Mat image(cv::Size(m_nWidth, m_nHeight), CV_16UC1, pInRawBuf);

    imshow("image", image);
    waitKey(0);
    //Mat image1 = image;

    
    /*MedianFilter(image1,10);


    namedWindow("image", WINDOW_NORMAL);
    namedWindow("image1", WINDOW_NORMAL);
    imshow("image", image);
    imshow("image1", image1);
    waitKey(0);*/
  

    



    // 创建待滤波数组
    //std::vector<double> data = image;

    // 调用快速中值滤波器
    //std::vector<double> result = fast_median_filter(data, 3);


    // 输出滤波后的数组
    /*for (auto x : result)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;*/
    //namedWindow("result", WINDOW_NORMAL);
    //imshow("result", result);
    //waitKey(0);



   // Mat dst;
    //LogTransform1(image, dst,200);

   
    //MyGammaCorrection(image, dst, 15);

    //normalize(image, image, 0.0, 65535.0, cv::NORM_MINMAX, CV_16UC1);
    //normalize(dst, dst, 0.0, 65535.0, cv::NORM_MINMAX, CV_16UC1);
    /*namedWindow("image", WINDOW_NORMAL);
    namedWindow("dst", WINDOW_NORMAL);
    imshow("image", image);
    imshow("dst", dst);
    waitKey(0);*/

    //Mat dst;
    //normalize(image, dst, 0.0, 16383.0, cv::NORM_MINMAX, CV_16UC1);
    ////cout << dst << endl;
    //cv::imwrite("C:\\1111\\9\\2\\test.tif", image);

    //unsigned char* pRawBuf2 = image.data;
    //cout << "1" << endl;
    //const char* filePath2 = "C:\\ncnn\\image16\\x64\\Release\\2.raw";
    //cout << "1" << endl;
    //SaveRawFile(pInRawBuf, m_nWidth, m_nHeight, filePath2);
    //cout << "1" << endl;
    //return 0;


    //saveRawFile("C:\\ncnn\\image16\\x64\\Release\\out.raw", image);

    /*Mat& MyGammaCorrection(Mat & src, float fGamma);
    if (!dst.data)
        return -1;*/
    //float fGamma = 3.5;
    //MyGammaCorrection(dst, fGamma);

    /*SSIF(image, dst, 25, 0.0005, 5, 1);*/
    //LogTransform1(image, dst,50);
    /*namedWindow("image", WINDOW_NORMAL);
    namedWindow("dst", WINDOW_NORMAL);
    imshow("image", image);
    imshow("dst", dst);
    waitKey(0);*/
     
    //dst = 65535 - image;
    /*Image_inversion(image,dst);
    namedWindow("dst", WINDOW_NORMAL);
    imshow("dst", dst);
    waitKey(0);*/
   /* Mat meanMat, meanStdMat;
    cv::meanStdDev(image, meanMat, meanStdMat);
    cout << meanMat << ", " << meanStdMat << endl;*/
    //Mat dst;
    //cv::fastNlMeansDenoising(image, dst);
    //imshow("image", image);
    //waitKey(0);
    
   /* double minVal = 0.0;
    double maxVal = 0.0;
    minMaxLoc(image, &minVal, &maxVal);
    cout << minVal << ", " << maxVal << endl;
    Mat image1=image;
    image1 = (image1 - minVal) * 65535 / (maxVal - minVal);*/


    /*cv::Mat tmp;
    cv::Ptr<cv::CLAHE> clahe = createCLAHE(1, cv::Size(4,3));
    clahe->apply(image, tmp);*/
    /*imshow("tmp", tmp);
    waitKey(0);*/

    //cv::imwrite("C:\\ncnn\\image16\\x64\\Release\\14\\20221130170fixpic\\14\\test.tif", image);

    
    //image = 65535 - image;

    //############################################################################################

    Mat gray, binary, binary1;
    image.convertTo(gray, CV_8U);
    //medianBlur(gray, gray,15);
    GaussianBlur(gray, gray, Size(15, 15), 0, 0);    
    namedWindow("a", WINDOW_NORMAL);
    imshow("a", gray);
    waitKey(0);
    //GaussianBlur(gray, gray, Size(9, 9), 0, 0);
    adaptiveThreshold(gray, binary,255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,3,0);
    //threshold(gray, binary, 40, 255, THRESH_BINARY | THRESH_OTSU);
    
 
    namedWindow("二值化", WINDOW_NORMAL);
    imshow("二值化", binary);
    // 定义结构元素
    Mat se1 = getStructuringElement(MORPH_RECT, Size(3 ,3));
    //Mat se2 = getStructuringElement(MORPH_RECT, Size(1, 1));

    morphologyEx(binary, binary1, MORPH_OPEN, se1);

    //namedWindow("f", WINDOW_NORMAL);
    //imshow("f", binary1);

    //morphologyEx(binary, binary, MORPH_DILATE, se2);
 
    //namedWindow("开运算", WINDOW_NORMAL);
    //imshow("开运算", binary1);
    //waitKey(0);
    vector<vector<Point>> contours;
    findContours(binary1, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    int index = -1;
    int max = 0;

    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area > max  )
        {
            max = area;
            index = i;
            cout << max << endl;
        }
    }
    RotatedRect rect = minAreaRect(contours[index]);
    Point2f srcpoint[4];//存放变换前四顶点
    Point2f dstpoint[4];//存放变换后四顶点
    rect.points(srcpoint);//获取最小外接矩形四顶点坐标

    cout << srcpoint[0] << endl;
    cout << srcpoint[1] << endl;
    cout << srcpoint[2] << endl;
    cout << srcpoint[3] << endl;
    //显示顶点
    for (size_t i = 0; i < 4; i++)
    {
        circle(image, srcpoint[i], 5, Scalar(65535, 65535, 65535), -1);//-1表示填充
    }
    namedWindow("顶点坐标", WINDOW_NORMAL);
    imshow("顶点坐标", image);
 
    //获取外接矩形宽高
    float width = rect.size.width;
    float height = rect.size.height;
    //定义矫正后四顶点
    dstpoint[0] = Point2f(0, height);
    dstpoint[1] = Point2f(0, 0);
    dstpoint[2] = Point2f(width, 0);
    dstpoint[3] = Point2f(width, height);
   
    // 透视变换
    Mat M = getPerspectiveTransform(srcpoint, dstpoint);
    Mat result = Mat::zeros(Size(width, height), CV_16UC1);
    warpPerspective(image, result, M, result.size());
    namedWindow("矫正结果", WINDOW_NORMAL);
    imshow("矫正结果", result);
    waitKey(0);
    cv::imwrite("C:\\ncnn\\image16\\x64\\Release\\test.tif", result);
    

    return 0;


}
