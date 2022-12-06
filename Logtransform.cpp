//***************************************************************************
//
// FileName:      Logtransform.cpp  
// Author:        gaopc
// Date:      2022/12/06
// Description:   对数变换
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc      2022/12/06    Build
//
//***************************************************************************


//***********
//opencv
//***********


void LogTransform(cv::Mat& src, cv::Mat& dst, double c) {
    int nr = src.rows;
    int nc = src.cols * src.channels();
    src.copyTo(dst);
    dst.convertTo(dst, CV_64F);
    if (src.isContinuous() && dst.isContinuous()) {  //判断图像连续性
        nr = 1;
        nc = src.rows * src.cols * src.channels(); //行数*列数 * 通道数= 一维数组的个数
    }

    for (int i = 0; i < nr; i++) {
        const ushort* srcdata = src.ptr <ushort>(i);  //采用指针访问像素，获取第i行的首地址
        double* dstdata = dst.ptr <double>(i);
        for (int j = 0; j < nc; j++) {
            dstdata[j] = c * log(double(1.0 + srcdata[j])); //开始处理每个像素
        }
    }
    cv::normalize(dst, dst, 0, 65535, cv::NORM_MINMAX); //经过对比拉升（将像素值归一化到0-65535）得到最终的图像
    dst.convertTo(dst, CV_16U);  //转回无符号16位图像
}
