//***************************************************************************
//
// FileName:      U16toU8.cpp  
// Author:        gaopc
// Date:      2022/11/22
// Description:   图像16位转8位
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/11/22    Build
//
//***************************************************************************


//***********
//opencv
//***********

//1.converTo
double minVal,maxVal;
minMaxIdx(image,&minVal,&maxVal);
double scaleVal = (maxVal - minVal) > 1.0 ? (maxVal - minVal) : 1.0; //避免除0
image -= minVal; //归一化到[0-255]
image.convertTo(dst,CV_8UC1,255.0/scaleVal,0.0);//归一化到[0-255]

//2.normalize
normalize(image, dst, 0.0, 255.0, cv::NORM_MINMAX, CV_8UC1);
