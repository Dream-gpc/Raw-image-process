//***************************************************************************
//
// FileName:      Imginverst.cpp  
// Author:        gaopc
// Date:      2022/11/25
// Description:   图像反转
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/11/25    Build
//
//***************************************************************************

// opencv Mat作为输入

void Image_inversion(cv::Mat& src, cv::Mat& dst) {
    int nr = src.rows;
    int nc = src.cols * src.channels();

    src.copyTo(dst);
    if (src.isContinuous() && dst.isContinuous()) {  //判断图像连续性
        nr = 1;
        nc = src.rows * src.cols * src.channels(); //行数*列数 * 通道数=一维数组的个数
    }
    for (int i = 0; i < nr; i++) {
        const ushort* srcdata = src.ptr <ushort>(i);  //采用指针访问像素，获取第i行的首地址
        ushort* dstdata = dst.ptr <ushort>(i);
        
        for (int j = 0; j < nc; j++) {
            
            dstdata[j] = 65535 - srcdata[j]; //开始处理每个像素

        }
    }
}
