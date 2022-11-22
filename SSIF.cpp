//***************************************************************************
//
// FileName:      SSIF.cpp  
// Author:        gaopc
// Date:      2022/11/22
// Description:  论文 <A guided edge-aware smoothing-sharpening filter based on patch interpolation model and generalized Gamma distribution> 的 C++ 实现.
// Function List:  
// History:              
//          <author>      <time>       <desc>        
//            gaopc          2022/11/22    Build
//
//***************************************************************************

void SSIF(cv::Mat& src, cv::Mat& dst, float radius, float epsilon, float kappa, float scale) {
	Mat  I,G, mu, nu, phi, a, c1, varsigma, beta, A, B, J, normalizeFactior, b, w, c3;
	src.convertTo(I, CV_32F);
	I = I / 255;
  //I = I / 65535;
	int patchsize = 2 * radius + 1;
	Mat h = Mat::ones(patchsize, patchsize, CV_32F);
	h = h / patchsize / patchsize;
	G = I;

	filter2D(I, mu, -1, h, cv::Point(-1, -1), (0, 0), 2);
	filter2D(G, nu, -1, h, cv::Point(-1, -1), (0, 0), 2);
	filter2D(I.mul(G), phi, -1, h, cv::Point(-1, -1), (0, 0), 2);

	Mat s1 = mu.mul(nu);
	phi = phi - s1;

	filter2D(G.mul(G), c1, -1, h, cv::Point(-1, -1), (0, 0), 2);
	c3 = c1 - nu.mul(nu);
	cv::max(0,c3, varsigma);
	
	Mat c4(varsigma.rows, varsigma.cols, CV_32FC3);
	for (int i = 0; i < c4.rows; i++)
	{
		for (int j = 0; j < c4.cols; j++)
		{
			for (int c = 0; c < c4.channels(); c++)
			{
				//给每一个元素赋值      
				c4.at<Vec3f>(i, j)[c] = varsigma.at<Vec3f>(i, j)[c] + epsilon;
			}
		}
	}
	
	a = phi.mul( 1/c4);
	//b = phi / (varsigma + epsilon);
	Mat c2(phi.rows, phi.cols, CV_32FC3);
	for (int i = 0; i < c2.rows; i++)
	{
		for (int j = 0; j < c2.cols; j++)
		{
			for (int c = 0; c < c2.channels(); c++)
			{
				//给每一个元素赋值
				if (phi.at<Vec3f>(i, j)[c] < 0)
				{
					c2.at<Vec3f>(i, j)[c] = -1;
				}	
				else if (phi.at<Vec3f>(i, j)[c] > 0)
				{
					c2.at<Vec3f>(i, j)[c] = 1;
				}
				else if (phi.at<Vec3f>(i, j)[c] == 0)
				{
					c2.at<Vec3f>(i, j)[c] = 0;
				}

					
			}
		}
	}
	sqrt(a.mul(a) + 4 * (kappa) * (epsilon) /c4, b);
	beta = (a + c2.mul(b)) / 2;
	cv:Scalar tempVal = cv::mean(varsigma);
	float matMean = (tempVal.val[0]+ tempVal.val[1]+ tempVal.val[2])/3;
	w = varsigma / (scale * matMean);
	Mat w1 = w.mul(w);
	for (int i = 0; i < w1.rows; i++)
	{
		for (int j = 0; j < w1.cols; j++)
		{
			for (int c = 0; c < w1.channels(); c++)
			{
				//给每一个元素赋值      
				w1.at<Vec3f>(i, j)[c] = w1.at<Vec3f>(i, j)[c] + 1;
			}
		}
	}

	w = 1 / w1;
	filter2D(w, normalizeFactior, -1, h, cv::Point(-1, -1), (0, 0), 2);
	filter2D(beta.mul(w), A, -1, h, cv::Point(-1, -1), (0, 0), 2);
	filter2D(((mu - beta.mul(nu)).mul(w)), B, -1, h, cv::Point(-1, -1), (0, 0), 2);
	J = (G.mul(A) + B) / normalizeFactior;

	J.convertTo(dst, CV_8UC3,255);  //转回无符号8位图像
  //J.convertTo(dst, CV_8UC1,255);  //转回无符号8位1单通道图像
  //J.convertTo(dst, CV_16UC1,65535);  //转回无符号16位图像
}
