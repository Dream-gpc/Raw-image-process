//***************************************************************************
//
// FileName:      CalMaxMin.cpp  
// Author:        gaopc
// Date:      2022/11/21
// Description:   计算图像最大最小值
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/11/22    Build
//
//***************************************************************************


//***********
//C++
//***********


// 获取Raw图像的最大值和最小值
// .h
template<typename T>
void GetMaxAndMin(T* raw, int width, int height, T& min, T& max)
// .cpp
template<typename T>
void GetMaxAndMin(T* raw, int width, int height, T& min, T& max)
{
  unsigned long bufSize = width * height;
  min = max = raw[0];

  for (unsigned long i = 0; i < bufSize; i++)
  {
    T sValue = *raw++;
    if (sValue > max)
      max = sValue;
    if (sValue < min)
      min = sValue;
  }
}


//调用.cpp文件
double min = 0.0;
double max = 0.0;
GetMaxAndMin(raw,m_nWidth,m_nHeight,min,max)
cout << min << ", " << max << endl;

//***********
//opencv
//***********


//将读取的raw图像转换为Mat
//cv::Mat image(cv::Size(m_nWidth, m_nHeight), CV_16UC1, pInRawBuf);
double minVal = 0.0;
double maxVal = 0.0;
minMaxLoc(image, &minVal, &maxVal);
cout << minVal << ", " << maxVal << endl;


