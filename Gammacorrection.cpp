//***************************************************************************
//
// FileName:      Gammacorrection.cpp 
// Author:        gaopc
// Date:      2022/12/06
// Description:   伽马矫正
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc       2022/12/06    Build
//
//***************************************************************************


//************
//opencv
//************

void MyGammaCorrection(Mat& src, Mat& dst, float fGamma)
{

    // build look up table  
    unsigned short lut[65536];
    for (int i = 0; i < 65536; i++)
    {
        lut[i] = saturate_cast<ushort>(pow((float)(i / 65535.0), fGamma) * 65535.0f);
    }

    dst = src.clone();
    const int channels = dst.channels();
    switch (channels)
    {
    case 1:   //灰度图的情况
    {

        MatIterator_<ushort> it, end;
        for (it = dst.begin<ushort>(), end = dst.end<ushort>(); it != end; it++)
            //*it = pow((float)(((*it))/255.0), fGamma) * 255.0;  
            *it = lut[(*it)];

        break;
    }
    case 3:  //彩色图的情况
    {

        MatIterator_<Vec3s> it, end;
        for (it = dst.begin<Vec3s>(), end = dst.end<Vec3s>(); it != end; it++)
        {
            //(*it)[0] = pow((float)(((*it)[0])/255.0), fGamma) * 255.0;  
            //(*it)[1] = pow((float)(((*it)[1])/255.0), fGamma) * 255.0;  
            //(*it)[2] = pow((float)(((*it)[2])/255.0), fGamma) * 255.0;  
            (*it)[0] = lut[((*it)[0])];
            (*it)[1] = lut[((*it)[1])];
            (*it)[2] = lut[((*it)[2])];
        }

        break;

    }
    }
}


