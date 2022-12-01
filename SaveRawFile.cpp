//***************************************************************************
//
// FileName:      saveRawFile.cpp  
// Author:        gaopc
// Date:      2022/12/01
// Description:   保存raw图像
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/12/01    Build
//
//***************************************************************************



int saveRawFile(const char fileName[], cv::Mat src)
{
    if (fileName == nullptr) return 0;

    FILE* fw = fopen(fileName, "wb");

    fwrite(src.data, src.elemSize(), src.cols * src.rows, fw);

    fclose(fw);

    return 1;
}


saveRawFile(".\out.raw", image);
