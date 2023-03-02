//***************************************************************************
//
// FileName:      Defect_Point_Correction.cpp  
// Author:        gaopc
// Date:      2023/03/01
// Description:   坏点矫正（包括坏线）
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2023/03/01    Build
//
//***************************************************************************

//使用示例
// ./xxx.exe gain.raw offset.raw 0/1


// C++

#include <iostream>
#include<fstream>
#include <algorithm>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{



    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
        return -1;
    }

    std::vector<unsigned short> gainData1D;
    std::vector<unsigned short> offsetData1D;
    bool isReverse = true;
    FILE* rfid = fopen(argv[1], "rb");
    if (rfid == NULL)
        return false;

    struct stat statbuf;
    stat(argv[1], &statbuf);
    long long pixelCount = statbuf.st_size / sizeof(unsigned short);
    gainData1D.resize(pixelCount);
    fread(&gainData1D[0], sizeof(unsigned short), pixelCount, rfid);

    //字节反转
    if (isReverse) {
        for (int i = 0; i < pixelCount; i++) {
            gainData1D[i] = SW16(gainData1D[i]);
        }
    }
    fclose(rfid);


    FILE* rfid1 = fopen(argv[2], "rb");
    if (rfid1 == NULL)
        return false;

    struct stat statbuf1;
    stat(argv[2], &statbuf1);
    long long pixelCount1 = statbuf1.st_size / sizeof(unsigned short);
    offsetData1D.resize(pixelCount1);
    fread(&offsetData1D[0], sizeof(unsigned short), pixelCount, rfid);

    //字节反转
    if (isReverse) {
        for (int i = 0; i < pixelCount; i++) {
            offsetData1D[i] = SW16(offsetData1D[i]);
        }
    }
    fclose(rfid1);

    int m_lBufSize = 2 * offsetData1D.size();
    cout << m_lBufSize << endl;


    long m_lBufSize1 = filelen, m_nWidth, m_nHeight;
    int triangle_size = 0;
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
        triangle_size = 139;
    }
    else if (m_lBufSize == 18374720)
    {
        m_nWidth = 2524;
        m_nHeight = 3640;
        triangle_size = 139;
    }  // size4
    else if (m_lBufSize == 4593024)
    {
        m_nWidth = 1296;

        m_nHeight = 1772;
        triangle_size = 215;
    }   // size1.5
    else if (m_lBufSize == 3211264)
    {
        m_nWidth = 1024;
        m_nHeight = 1568;

    }
    std::vector<std::vector<unsigned short>> gainData2D(m_nHeight, std::vector<unsigned short>(m_nWidth));
    std::vector<std::vector<unsigned short>> offsetData2D(m_nHeight, std::vector<unsigned short>(m_nWidth));
    gainData2D.resize(m_nWidth);
    for (int i = 0; i < m_nWidth; i++) {
        gainData2D[i].resize(m_nHeight);
    }
    for (int j = 0; j < m_nHeight; j++) {
        for (int i = 0; i < m_nWidth; i++) {
            gainData2D[i][j] = gainData1D[i + j * m_nWidth];
        }
    }

    offsetData2D.resize(m_nWidth);
    for (int i = 0; i < m_nWidth; i++) {
        offsetData2D[i].resize(m_nHeight);
    }
    for (int j = 0; j < m_nHeight; j++) {
        for (int i = 0; i < m_nWidth; i++) {
            offsetData2D[i][j] = offsetData1D[i + j * m_nWidth];
        }
    }


    float model = atof(argv[3]);
    int offset;
    float yuzhi;

    if (model == 0)
    {
        offset = 800;
        yuzhi = 0.2;
    }
    else
    {
        offset = 0;
        yuzhi = 0.1;
    }
    for (int j = 0; j < m_nHeight; j++)
    {
        for (int i = 0; i < m_nWidth; i++)
        {
            gainData2D[i][j] = gainData2D[i][j] + offset;
            offsetData2D[i][j] = offsetData2D[i][j] + offset;
        }
    }



    for (int j = 0; j < m_nHeight; j++)
    {
        for (int i = 0; i < m_nWidth; i++)
        {
            if (gainData2D[i][j] > offsetData2D[i][j])
            {
                gainData2D[i][j] = gainData2D[i][j] - offsetData2D[i][j];
            }
            else
            {
                gainData2D[i][j] = 0;
            }


        }
    }


    double mean_gain = 0;
    double mean_offset = 0;


    for (int j = 0; j < (m_nHeight - triangle_size); j++)
    {
        for (int i = 0; i < m_nWidth; i++) {
            mean_gain += gainData2D[i][j];

            mean_offset += offsetData2D[i][j];
        }
    }

    cout << mean_gain << endl;
    cout << mean_offset << endl;
    int mianji = (m_nHeight - triangle_size) * (m_nWidth);
    double mean_val = mean_gain / mianji;
    double mean_val_offset = mean_offset / mianji;
    cout << mianji << endl;
    cout << mean_val << endl;
    cout << mean_val_offset << endl;

    int c = 0;
    for (int i = m_nHeight - triangle_size; i < m_nHeight; i++) {
        c++;
        for (int j = 0; j < c; j++) {

            gainData2D[j][i] = (int)mean_val;
            offsetData2D[j][i] = (int)mean_val_offset;
        }

        for (int j = m_nWidth - c; j < m_nWidth; j++) {
            gainData2D[j][i] = (int)mean_val;
            offsetData2D[j][i] = (int)mean_val_offset;
        }
    }



    vector<pair<int, int>> list_points_all;
    vector<pair<int, int>> bad_points_all;
    int  count1 = 0;

    for (int j = 0; j < m_nHeight; j++) {
        {for (int i = 0; i < m_nWidth; i++)
            // 如果该点像素的| p(x,y) – Avag | > 614则认为是坏点
            if (abs(static_cast<double>(gainData2D[i][j] - mean_val)) > mean_val * 0.15) {
                count1++;
                list_points_all.push_back(make_pair(i, j));
            }
            else if (static_cast<double>(offsetData2D[i][j]) > mean_val_offset * (yuzhi + 1)) {
                count1++;
                list_points_all.push_back(make_pair(i, j));
            }
        }
    }
    bad_points_all = list_points_all;
    vector<int> list_lines;
    vector<pair<int, int>> list_points_on_lines;
    int sum_lines = 0;
    // 扫描所有行
    cout << "1" << endl;
    for (int i = 0; i < m_nWidth; i++) {
        int count = 0;
        vector<pair<int, int>> list_points_on_line;
        for (int j = 0; j < m_nHeight; j++) {
            if (abs(static_cast<double>(gainData2D[i][j] - mean_val)) > mean_val * 0.15) {
                count++;
                list_points_on_line.push_back(make_pair(i, j));
            }

        }
        if (count > 300) {
            sum_lines++;
            list_lines.push_back(i);
            list_points_on_lines.insert(list_points_on_lines.end(), list_points_on_line.begin(), list_points_on_line.end());
        }
    }

    // 扫描所有列
    for (int j = 0; j < m_nHeight; j++) {
        int count = 0;
        vector<pair<int, int>> list_points_on_line;
        for (int i = 0; i < m_nWidth; i++) {
            if (abs(static_cast<double>(gainData2D[i][j] - mean_val)) > mean_val * 0.15) {
                count++;
                list_points_on_line.push_back(make_pair(i, j));
            }

        }
        if (count > 300) {
            sum_lines++;
            list_lines.push_back(-j);
            list_points_on_lines.insert(list_points_on_lines.end(), list_points_on_line.begin(), list_points_on_line.end());
        }
    }

    for (int i = 0; i < list_points_all.size(); i++) {
        for (int j = 0; j < list_points_on_lines.size(); j++) {
            if (list_points_all[i] == list_points_on_lines[j]) {
                list_points_all.erase(list_points_all.begin() + i);
                i--; // 因为删除了一个元素，所以要将 i 减 1
                break; // 找到一个相同的点就可以退出内层循环
            }
        }
    }

    // Open file for writing
    ofstream output_file("points.txt");

    output_file << "ShadoCam Pixel Map File\nGenerated by ShadoCam\n*** Do Not Edit ***\n\n$begin" << endl;


    // Write points to file
    for (const auto& point : list_points_all) {
        output_file << "P" << point.first + 1 << "," << point.second + 1 << endl;
    }
    output_file << "C512\nC513" << endl;

    for (const auto& point : list_lines) {
        if (point > 0) {
            output_file << "C" << abs(point) + 1 << " 1-" << m_nHeight << endl;

        }
        else if (point < 0) {
            output_file << "R" << abs(point) + 1 << " 1-" << m_nWidth << endl;

        }

    }


    // Close file
    output_file.close();
    // 输出所有坏点总个数sum_points_all
    cout << "Total number of bad points: " << count1 << endl;


    // 输出所有坏线总数sum_lines
    cout << "Total number of bad lines: " << sum_lines << endl;

    set<pair<int, int>> ppoints(list_points_all.begin(), list_points_all.end());
    set<pair<int, int>> badpoints(bad_points_all.begin(), bad_points_all.end());
    vector<pair<int, int>> error_points;
    for (auto it = ppoints.begin(); it != ppoints.end(); ++it) {
        //int JUJIcount = countNeighborBadPoints(list_points_all[i], bad_points_all);

        int x = it->first;
        int y = it->second;
        

        if (!badpoints.count({ x - 1, y - 1 }) && !badpoints.count({ x + 1, y + 1 })) {
            //image.at<ushort>(y, x) = (int)((image.at<ushort>(y - 1, x - 1) + image.at<ushort>(y + 1, x + 1)) / 2);
            if (x - 1 > 0 && y - 1 > 0) {
                gainData2D[x][y] = (int)((gainData2D[x - 1][y - 1] + gainData2D[x + 1][y + 1]) / 2);
            }
           

        }
        else if (!badpoints.count({ x, y - 1 }) && !badpoints.count({ x, y + 1 })) {
            //image.at<ushort>(y, x) = (int)((image.at<ushort>(y - 1, x) + image.at<ushort>(y + 1, x)) / 2);
            if (x - 1 > 0 && y - 1 > 0) {
                gainData2D[x][y] = (int)((gainData2D[x ][y - 1] + gainData2D[x][y + 1]) / 2);
            }
        }
        else if (!badpoints.count({ x - 1, y }) && !badpoints.count({ x + 1, y })) {
            //image.at<ushort>(y, x) = (int)((image.at<ushort>(y, x - 1) + image.at<ushort>(y, x + 1)) / 2);
            if (x - 1 > 0 && y - 1 > 0) {
            gainData2D[x][y] = (int)((gainData2D[x - 1][y] + gainData2D[x + 1][y]) / 2);
             }
        }
        else if (!badpoints.count({ x + 1, y - 1 }) && !badpoints.count({ x - 1, y + 1 })) {
            //image.at<ushort>(y, x) = (int)((image.at<ushort>(y - 1, x + 1) + image.at<ushort>(y + 1, x - 1)) / 2);
            if (x - 1 > 0 && y - 1 > 0) {
                gainData2D[x][y] = (int)((gainData2D[x + 1][y - 1] + gainData2D[x - 1][y + 1]) / 2);
            }
        }
        else {
            error_points.push_back(make_pair(x, y));
        }
    }

    set<int> lines(list_lines.begin(), list_lines.end());


   
    // Use a set instead of a vector
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        int x = *it;
        cout << x << endl;

        int x1 = x + 1, x2 = x + 2, x3 = x + 3, xm1 = x + 1, xm2 = x + 2, xm3 = x + 3; // Store the values in variables

        //3条坏线
        if (x > 0 && lines.count(x1) && lines.count(x2) && !lines.count(x3)) {
            // Avoid repeated computation by storing the value in a variable
            //int avg = (image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2;

            for (int n = 0; n < m_nWidth; n++) {
                //image.at<ushort>(n, x) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x3)) / 2);
                //image.at<ushort>(n, x1) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x3)) / 2);
                //image.at<ushort>(n, x2) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x3)) / 2);
                gainData2D[x][n] = (int)((gainData2D[x - 1][n] + gainData2D[x3][n]) / 2);
                gainData2D[x1][n] = (int)((gainData2D[x - 1][n] + gainData2D[x3][n]) / 2);
                gainData2D[x2][n] = (int)((gainData2D[x - 1][n] + gainData2D[x3][n]) / 2);
            }

            lines.erase(x2);
            lines.erase(x1);
            lines.erase(x);
        }
        //3条坏线
        else if (x < 0 && lines.count(xm1) && lines.count(xm2) && !lines.count(xm3)) {
            // Avoid repeated computation by storing the value in a variable
           // int avg = (image.at<ushort>(abs(x) - 1, n) + image.at<ushort>(abs(x) + 2, n)) / 2;
            int xx = abs(x);
            for (int n = 0; n < m_nHeight; n++) {
                //image.at<ushort>(xx, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx - 3, n)) / 2);
                //image.at<ushort>(xx - 1, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx - 3, n)) / 2);
                //image.at<ushort>(xx - 2, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx - 3, n)) / 2);
                gainData2D[n][xx] = (int)((gainData2D[n][xx+1] + gainData2D[n][xx - 3]) / 2);
                gainData2D[n][xx-1] = (int)((gainData2D[n][xx + 1] + gainData2D[n][xx - 3]) / 2);
                gainData2D[n][xx-2] = (int)((gainData2D[n][xx + 1] + gainData2D[n][xx - 3]) / 2);
            }
            lines.erase(xm2);
            lines.erase(xm1);
            lines.erase(x);
        }

        //2条坏线
        else if (x > 0 && lines.count(x1) && !lines.count(x2) && !lines.count(x3)) {
            // Avoid repeated computation by storing the value in a variable
            //int avg = (image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2;

            for (int n = 0; n < m_nHeight; n++) {
                //image.at<ushort>(n, x) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2);
                //image.at<ushort>(n, x1) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2);
                gainData2D[x][n] = (int)((gainData2D[x - 1][n] + gainData2D[x2][n]) / 2);
                gainData2D[x1][n] = (int)((gainData2D[x - 1][n] + gainData2D[x2][n]) / 2);
            }
          
            lines.erase(x1);
            lines.erase(x);
        }
        else if (x < 0 && lines.count(xm1) && !lines.count(xm2) && !lines.count(xm3)) {
            // Avoid repeated computation by storing the value in a variable
           // int avg = (image.at<ushort>(abs(x) - 1, n) + image.at<ushort>(abs(x) + 2, n)) / 2;
            int xx = abs(x);
            for (int n = 0; n < m_nWidth; n++) {
                //image.at<ushort>(xx, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx - 2, n)) / 2);
                //image.at<ushort>(xx - 1, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx - 2, n)) / 2);
                gainData2D[n][xx] = (int)((gainData2D[n][xx + 1] + gainData2D[n][xx - 2]) / 2);
                gainData2D[n][xx - 1] = (int)((gainData2D[n][xx + 1] + gainData2D[n][xx - 2]) / 2);
            }
          
            lines.erase(xm1);
            lines.erase(x);
        }
        else if (x > 0 && !lines.count(x1) && !lines.count(x2) && !lines.count(x3)) {
            // Avoid repeated computation by storing the value in a variable
            //int avg = (image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2;

            for (int n = 0; n < m_nHeight; n++) {

               // image.at<ushort>(n, x) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x + 1)) / 2);
                gainData2D[x][n] = (int)((gainData2D[x - 1][n] + gainData2D[x + 1][n]) / 2);
            }
          
            lines.erase(x);
        }
        else if (x < 0 && !lines.count(xm1) && !lines.count(xm2) && !lines.count(xm3)) {
            // Avoid repeated computation by storing the value in a variable
           // int avg = (image.at<ushort>(abs(x) - 1, n) + image.at<ushort>(abs(x) + 2, n)) / 2;
            int xx = abs(x);
            for (int n = 0; n < m_nWidth; n++) {
                //image.at<ushort>(xx, n) = (int)((image.at<ushort>(xx - 1, n) + image.at<ushort>(xx + 1, n)) / 2);
                gainData2D[n][xx] = (int)((gainData2D[n][xx - 1] + gainData2D[n][xx +1]) / 2);
            }
        
            lines.erase(x);
        }
    }
    ofstream output_file1("error_points.txt");

    output_file1 << "ShadoCam Pixel Map File\nGenerated by ShadoCam\n*** Do Not Edit ***\n\n$begin" << endl;


    // Write points to file
    for (const auto& point : error_points) {
        output_file1 << "P" << point.first << "," << point.second << endl;
    }
    output_file1.close();



    c = 0;
    for (int i = m_nHeight - triangle_size; i < m_nHeight; i++) {
        c++;
        for (int j = 0; j < c; j++) {

            gainData2D[j][i] =0;
            
        }

        for (int j = m_nWidth - c; j < m_nWidth; j++) {
            gainData2D[j][i] = 0;
         
        }
    }




    vector<unsigned short> saveG(m_nWidth* m_nHeight);
    for (int j = 0; j < m_nHeight; j++) {
        for (int i = 0; i < m_nWidth; i++) {
            unsigned short temp;
            if (round(gainData2D[i][j]) > 65535) {
                temp = 65535;
            }
            else {
                temp = round(gainData2D[i][j]);
            }
            saveG[i + j * m_nWidth] = SW16(temp);
        }
    }
    const char* wpath = "output.raw";

    FILE* wfid = fopen(wpath, "wb");
    fwrite(&saveG[0], sizeof(unsigned short), saveG.size(), wfid);
    fclose(wfid);




    return 0;
}







// Opencv   
#include <iostream>
#include<fstream>
#include <algorithm>
#include <vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

long filelen; 
unsigned char* ReadRawFromFile(const char* filePath, long* filesize)
{
    FILE* fp;
    fp = fopen(filePath, "rb");
    if (fp == NULL)
    {
        std::cout << "打开文件失败";
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
#define SW16(x) \
  ((unsigned short)( \
  (((unsigned short)(x) & (unsigned short)0x00ffU) << 8) | \
  (((unsigned short)(x) & (unsigned short)0xff00U) >> 8) ))
//int argc, char* argv[]
int main(int argc, char* argv[])
{

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
        return -1;
    }
    long rawDataSize = 0;
    unsigned char* pInRawBuf = NULL;
    unsigned char* pInRawBuf2 = NULL;
    //读取RAW图
    pInRawBuf = ReadRawFromFile(argv[1], &rawDataSize);

    pInRawBuf2 = ReadRawFromFile(argv[2], &rawDataSize);
    //高低字节转换
    unsigned short *pInRawBuf1 = (unsigned short*)pInRawBuf;
    for (unsigned long i = 0; i < filelen/2; i++)
        *pInRawBuf1++ = SW16(*pInRawBuf1);

    unsigned short* pInRawBuf22 = (unsigned short*)pInRawBuf2;
    for (unsigned long i = 0; i < filelen / 2; i++)
        *pInRawBuf22++ = SW16(*pInRawBuf22);
    
    long m_lBufSize= filelen, m_nWidth, m_nHeight;
    int triangle_size=0;
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
        triangle_size = 139;
    }
    else if (m_lBufSize == 18374720)
    {
        m_nWidth = 2524;
        m_nHeight = 3640;
        triangle_size = 139;
    }  // size4
    else if (m_lBufSize == 4593024)
    {
        m_nWidth = 1296;

        m_nHeight = 1772;
        triangle_size = 215;
    }   // size1.5
    else if (m_lBufSize == 3211264)
    {
        m_nWidth = 1024;
        m_nHeight = 1568;
      
    }


    cv::Mat image(cv::Size(m_nWidth, m_nHeight), CV_16UC1, pInRawBuf);
    cv::Mat image_offset(cv::Size(m_nWidth, m_nHeight), CV_16UC1, pInRawBuf2);

    float model = atof(argv[3]);
    int offset;
    float yuzhi;
    if (model == 0)
    {
        offset = 800;
        yuzhi = 0.2;
    }
    else
    {
        offset = 0;
        yuzhi = 0.1;
    }
    image = image + offset;
    image_offset = image_offset + offset;

    image = image - image_offset;
    

    int count1= 0 ;
    float mean_val; //static_cast<double>(mean1.at<double>(0));
    float mean_val_offset;
  

    Rect roi(0, 0, m_nWidth, m_nHeight- triangle_size);

    // 提取 ROI 区域
    Mat roi_img = image(roi);
    Mat roi_img_offset = image_offset(roi);



   

    // 计算均值
    Scalar mean = cv::mean(roi_img);
    Scalar mean_offset = cv::mean(roi_img_offset);

   

    mean_val = mean[0];
    mean_val_offset = mean_offset[0];
    (int)mean_val_offset;
    cout << roi.area() << endl;
    cout << mean_val << endl;
    cout << mean_val_offset << endl;
  
    int c=0;
    for (int i = m_nHeight- triangle_size; i < m_nHeight; i++) {
        c++;
        for (int j = 0; j < c; j++) {
            image.at<ushort>(i, j) = (int)mean_val;
            image_offset.at<ushort>(i, j) = (int)mean_val_offset;
        }

        for (int j = m_nWidth- c; j < m_nWidth; j++) {
            image.at<ushort>(i, j) = (int)mean_val;
            image_offset.at<ushort>(i, j) = (int)mean_val_offset;
        }
    }

    cv::imwrite("test1.tif", image);
    vector<pair<int, int>> list_points_all;
    vector<pair<int, int>> bad_points_all;
    for (int j = 0; j < m_nHeight; j++) {
         {for (int i = 0; i < m_nWidth; i++)
            // 如果该点像素的| p(x,y) – Avag | > 614则认为是坏点
            if (abs(static_cast<double>(image.at<ushort>(j,i) - mean_val)) > mean_val*0.15) {
                count1++;
                list_points_all.push_back(make_pair(i, j));
            }
            else if (static_cast<double>(image_offset.at<ushort>(j, i)) > mean_val_offset * (yuzhi+1)) {
                count1++;
                list_points_all.push_back(make_pair(i, j));
         }
        }
    }
    bad_points_all = list_points_all;
    vector<int> list_lines;
    vector<pair<int, int>> list_points_on_lines;
    int sum_lines = 0;
    // 扫描所有行
 
    for (int i = 0; i < m_nWidth; i++) {
        int count = 0;
        vector<pair<int, int>> list_points_on_line;
        for (int j = 0; j < m_nHeight; j++) {
            if (abs(static_cast<double>(image.at<ushort>(j, i) - mean_val)) > mean_val*0.15) {
                count++;
                list_points_on_line.push_back(make_pair(i, j));
            }
         
        }
        if (count > 300) {
            sum_lines++;
            list_lines.push_back(i);
            list_points_on_lines.insert(list_points_on_lines.end(), list_points_on_line.begin(), list_points_on_line.end());
        }
    }

    // 扫描所有列
    for (int j = 0; j < m_nHeight; j++) {
        int count = 0;
        vector<pair<int, int>> list_points_on_line;
        for (int i = 0; i < m_nWidth; i++) {
            if (abs(static_cast<double>(image.at<ushort>(j, i) - mean_val)) > mean_val * 0.15) {
                count++;
                list_points_on_line.push_back(make_pair(i, j));
            }

        }
        if (count > 300) {
            sum_lines++;
            list_lines.push_back(-j);
            list_points_on_lines.insert(list_points_on_lines.end(), list_points_on_line.begin(), list_points_on_line.end());
        }
    }


    for (int i = 0; i < list_points_all.size(); i++) {
        for (int j = 0; j < list_points_on_lines.size(); j++) {
            if (list_points_all[i] == list_points_on_lines[j]) {
                list_points_all.erase(list_points_all.begin() + i);
                i--; // 因为删除了一个元素，所以要将 i 减 1
                break; // 找到一个相同的点就可以退出内层循环
            }
        }
    }

    // Open file for writing
    ofstream output_file("points.txt");

    output_file << "ShadoCam Pixel Map File\nGenerated by ShadoCam\n*** Do Not Edit ***\n\n$begin" << endl;


    // Write points to file
    for (const auto& point : list_points_all) {
        output_file <<"P" << point.first+1 << "," << point.second+1 << endl;
    }
    output_file << "C512\nC513" << endl;

    for (const auto& point : list_lines) {
        if (point > 0) {
            output_file << "C" << abs(point)+1 << " 1-"<< image.rows << endl;
     
        }
        else if (point < 0) {
            output_file << "R" << abs(point)+1 <<" 1-" << image.cols << endl;
     
        }
     
    }


    // Close file
    output_file.close();



    // 输出所有坏点总个数sum_points_all
    cout << "Total number of bad points: " << count1 << endl;


    // 输出所有坏线总数sum_lines
    cout << "Total number of bad lines: " << sum_lines << endl;


    //修复坏点
    set<pair<int,int>> ppoints(list_points_all.begin(), list_points_all.end());
    set<pair<int, int>> badpoints(bad_points_all.begin(), bad_points_all.end());
    vector<pair<int, int>> error_points;
    for (auto it = ppoints.begin(); it != ppoints.end(); ++it) {
       

        int x = it->first;
        int y = it->second;
        
  
        if (!badpoints.count({ x - 1, y - 1 }) && !badpoints.count({ x + 1, y + 1 })) {
            image.at<ushort>(y, x) = (int)((image.at<ushort>(y - 1, x -1) + image.at<ushort>(y+1, x+1)) / 2);
        
        }
        else if (!badpoints.count({ x, y - 1 }) && !badpoints.count({ x, y + 1 })) {
            image.at<ushort>(y, x) = (int)((image.at<ushort>(y - 1, x) + image.at<ushort>(y + 1, x)) / 2);

        }
        else if (!badpoints.count({ x - 1, y }) && !badpoints.count({ x+ 1, y  })) {
            image.at<ushort>(y, x) = (int)((image.at<ushort>(y, x-1) + image.at<ushort>(y, x+1)) / 2);

        }
        else if (!badpoints.count({ x + 1, y - 1 }) && !badpoints.count({ x - 1, y + 1 })) {
            image.at<ushort>(y, x) = (int)((image.at<ushort>(y-1, x + 1) + image.at<ushort>(y+1, x - 1)) / 2);
        }
        else {
            error_points.push_back(make_pair(x, y));
        }
    }
    
    set<int> lines(list_lines.begin(), list_lines.end());
    // Use a set instead of a vector
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        int x = *it;
        cout << x << endl;
        
        int x1 = x + 1, x2 = x + 2, x3 = x + 3, xm1 = x + 1, xm2 = x + 2, xm3 = x + 3; // Store the values in variables
        
        //3条坏线
        if (x > 0 && lines.count(x1) && lines.count(x2) && !lines.count(x3)) {
        // Avoid repeated computation by storing the value in a variable
        //int avg = (image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2;

            for (int n = 0; n < m_nWidth; n++) {
                image.at<ushort>(n, x) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x3)) / 2);
                image.at<ushort>(n, x1) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x3)) / 2);
                image.at<ushort>(n, x2) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x3)) / 2);
            }

            lines.erase(x2);
            lines.erase(x1);
            lines.erase(x);
         }
        //3条坏线
        else if (x < 0 && lines.count(xm1) && lines.count(xm2) && !lines.count(xm3)) {
        // Avoid repeated computation by storing the value in a variable
       // int avg = (image.at<ushort>(abs(x) - 1, n) + image.at<ushort>(abs(x) + 2, n)) / 2;
           int xx = abs(x);
           for (int n = 0; n < m_nHeight; n++) {
               image.at<ushort>(xx, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx -3, n)) / 2);
               image.at<ushort>(xx - 1, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx -3, n)) / 2);
               image.at<ushort>(xx - 2, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx - 3, n)) / 2);
           }
           lines.erase(xm2);
           lines.erase(xm1);
           lines.erase(x);
         }

        //2条坏线
        else if (x > 0 && lines.count(x1) && !lines.count(x2) && !lines.count(x3)) {
        // Avoid repeated computation by storing the value in a variable
        //int avg = (image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2;

            for (int n = 0; n < m_nHeight; n++) {
                image.at<ushort>(n, x) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2);
                image.at<ushort>(n, x1) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2);
            }
            
            lines.erase(x1);
            lines.erase(x);
        }
        else if (x < 0 && lines.count(xm1) && !lines.count(xm2) && !lines.count(xm3)) {
        // Avoid repeated computation by storing the value in a variable
       // int avg = (image.at<ushort>(abs(x) - 1, n) + image.at<ushort>(abs(x) + 2, n)) / 2;
           int xx = abs(x);
           for (int n = 0; n < m_nWidth; n++) {
               image.at<ushort>(xx, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx - 2, n)) / 2);
               image.at<ushort>(xx - 1, n) = (int)((image.at<ushort>(xx + 1, n) + image.at<ushort>(xx - 2, n)) / 2);
           }
        
           lines.erase(xm1);
           lines.erase(x);
        }
        else if (x > 0 && !lines.count(x1) && !lines.count(x2) && !lines.count(x3)) {
            // Avoid repeated computation by storing the value in a variable
            //int avg = (image.at<ushort>(n, x - 1) + image.at<ushort>(n, x2)) / 2;

            for (int n = 0; n < m_nHeight; n++) {
                image.at<ushort>(n, x) = (int)((image.at<ushort>(n, x - 1) + image.at<ushort>(n, x+1)) / 2);
              
            }
         
            lines.erase(x);
        }
        else if (x < 0 && !lines.count(xm1) && !lines.count(xm2) && !lines.count(xm3)) {
            // Avoid repeated computation by storing the value in a variable
           // int avg = (image.at<ushort>(abs(x) - 1, n) + image.at<ushort>(abs(x) + 2, n)) / 2;
            int xx = abs(x);
            for (int n = 0; n < m_nWidth; n++) {
                image.at<ushort>(xx, n) = (int)((image.at<ushort>(xx - 1, n) + image.at<ushort>(xx + 1, n)) / 2);
             
            }
        
            lines.erase(x);
        }
    }
        ofstream output_file1("error_points.txt");

        output_file1 << "ShadoCam Pixel Map File\nGenerated by ShadoCam\n*** Do Not Edit ***\n\n$begin" << endl;


        // Write points to file
        for (const auto& point : error_points) {
            output_file1 << "P" << point.first << "," << point.second << endl;
        }
        output_file1.close();

      

        cv::imwrite("test.tif", image);




        return 0;
    }
