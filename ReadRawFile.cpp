//***************************************************************************
//
// FileName:      ReadRawFile.cpp  
// Author:        gaopc
// Date:      2022/11/21
// Description:   16位raw图像读取
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/11/21    Build
//
//***************************************************************************


//使用opencv显示raw图像

#include<iostream>
#include<opencv2/opencv.hpp>
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include <opencv2\imgproc\types_c.h>
#include<opencv2/opencv.hpp>
#include"math.h"
#include "chrono"
#include<fstream>

using namespace std;
using namespace chrono;
using namespace cv;

// 获取图像大小
long filelen; 
unsigned char* ReadRawFromFile(char* filePath, long* filesize)
{
    FILE* fp;
    fp = fopen(filePath, "rb");
    if (fp == NULL)
    {
        std::cout << "failed to read file";
        return NULL;
    }
    fpos_t startpos, endpos;
    fseek(fp, 0, SEEK_END);
    fgetpos(fp, &endpos);
    fseek(fp, 0, SEEK_SET);
    fgetpos(fp, &startpos);
   
    filelen = (long)(endpos - startpos);
    unsigned char* bTemp = NULL;
    bTemp = (unsigned char*)malloc(filelen);
    if (bTemp == NULL)
    {
        fclose(fp);
        return NULL;
    }
    memset(bTemp, 0, filelen);
    fread(bTemp, filelen, 1, fp);
    fclose(fp);
    *filesize = filelen;
    return bTemp;
}
//高低字节转换 （假如你需要
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
    //高低字节转换 （假如你需要
    unsigned short *pInRawBuf1 = (unsigned short*)pInRawBuf;
    for (unsigned long i = 0; i < filelen/2 ; i++)
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


    cv::Mat image(cv::Size(m_nWidth, m_nHeight), CV_16UC1, pInRawBuf);
    imshow("image", image);
    waitKey(0);

    return 0;
}
