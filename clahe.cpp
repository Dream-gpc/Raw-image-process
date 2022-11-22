//***************************************************************************
//
// FileName:      clahe.cpp  
// Author:        gaopc
// Date:      2022/11/22
// Description:   限制对比度的自适应直方图均衡化实现
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/11/22    Build
//
//***************************************************************************


//***********
//opencv
//***********

//raw image to mat
cv::Mat src(HEIGHT,WIDTH,CV_16UC1);
cv::Mat tmp;
cv::Ptr<cv::CLAHE> clahe = createCLAHE(20.0,cv::Size(8,8));
clahe->apply(src,tmp);
imshow("src",src);
imshow("tmp",tmp);
waitKey(0);




//***********
//C++ 挖坑
//***********
