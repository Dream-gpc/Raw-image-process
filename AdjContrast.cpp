//***************************************************************************
//
// FileName:      CalMaxMin.cpp  
// Author:        gaopc
// Date:      2022/11/22
// Description:   调整图像对比度
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/11/22    Build
//
//***************************************************************************


//***********
//opencv
//***********


//***********
//通过最大最小值调整对比度 （仿ImageJ）

//将读取的raw图像转换为Mat
cv::Mat image(cv::Size(m_nWidth, m_nHeight), CV_16UC1, pInRawBuf);
double minVal = 0.0;
double maxVal = 0.0;
minMaxLoc(image, &minVal, &maxVal);
cout << minVal << ", " << maxVal << endl;
Mat image1=image;
image1 = (image1 - minVal) * 65535 / (maxVal - minVal);

imshow("image", image);
imshow("image1", image1);
waitKey(0);


//***********
