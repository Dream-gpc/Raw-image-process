//***************************************************************************
//
// FileName:      CalMean.cpp  
// Author:        gaopc
// Date:      2022/11/21
// Description:   计算raw图像均值
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/11/22    Build
//
//***************************************************************************


//***********
//C++
//***********


// 获取Raw图像的均值
// .h
template<typename T>
double CalculateMean(T* raw, int width, int left, int top, int right, int bottom)

// .cpp
template<typename T>
double CalculateMean(T* raw, int width, int left, int top, int right, int bottom)
{
  double mean = 0.0;

  for (int j = top; j < bottom; j++)
  {
    for (int i = left; i < right; i++)
    {
      mean += raw[j * width + i];
    }
  }

  mean = mean / ((right - left) * (bottom - top));

  return mean;
}

//调用.cpp文件
//left top 从第几个元素开始读 默认left=0 top=0
double left =0;
double top =0;
double meanval =0;
meanval = CalculateMean(raw,Channl,left,top,m_nWidth,m_nHeight)
cout << min << ", " << max << endl;


//***********
//opencv
//***********

//将读取的raw图像转换为Mat
cv::Mat image(cv::Size(m_nWidth, m_nHeight), CV_16UC1, pInRawBuf);
//mean每通道均值 meanStdMat每通道标准差
Mat meanMat, meanStdMat;
cv::meanStdDev(image, meanMat, meanStdMat);
cout << meanMat << ", " << meanStdMat << endl;
