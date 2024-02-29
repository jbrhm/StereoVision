#include "Gradient.hpp"
#include <cmath>

std::vector<double> const Gradient::sobelFilterX = 	{
													1,0,-1,
													2,0,-2,
													1,0,-1
												};

std::vector<double> const Gradient::sobelFilterY = 	{
													1,2,1,
													0,0,0,
													-1,-2,-1
												};
double Gradient::runFilterX(cv::Mat const& regularImage, int row, int col){
	return sobelFilterX[0] * regularImage.at<uchar>(row-1, col-1) + sobelFilterX[1] * regularImage.at<uchar>(row-1, col) + sobelFilterX[2] * regularImage.at<uchar>(row-1, col+1) + sobelFilterX[3] * regularImage.at<uchar>(row, col-1) + sobelFilterX[4] * regularImage.at<uchar>(row, col) + sobelFilterX[5] * regularImage.at<uchar>(row, col+1) + sobelFilterX[6] * regularImage.at<uchar>(row+1, col-1) + sobelFilterX[7] * regularImage.at<uchar>(row+1, col) + sobelFilterX[8] * regularImage.at<uchar>(row+1, col+1);
}

double Gradient::runFilterY(cv::Mat const& regularImage, int row, int col){
	return sobelFilterY[0] * regularImage.at<uchar>(row-1, col-1) + sobelFilterY[1] * regularImage.at<uchar>(row-1, col) + sobelFilterY[2] * regularImage.at<uchar>(row-1, col+1) + sobelFilterY[3] * regularImage.at<uchar>(row, col-1) + sobelFilterY[4] * regularImage.at<uchar>(row, col) + sobelFilterY[5] * regularImage.at<uchar>(row, col+1) + sobelFilterY[6] * regularImage.at<uchar>(row+1, col-1) + sobelFilterY[7] * regularImage.at<uchar>(row+1, col) + sobelFilterY[8] * regularImage.at<uchar>(row+1, col+1);
}

Gradient::Gradient(cv::Mat const& regularImage){
	mGradientMat = cv::Mat{regularImage.rows-1, regularImage.cols-1, 0, cv::Scalar{0,0,0}};	
	
	std::cout << "regularImage (row, col): " << regularImage.rows << ", " << regularImage.cols << std::endl;
	std::cout << "mGradientMat (row, col): " << mGradientMat.rows << ", " << mGradientMat.cols << std::endl;

	for(int row = 0; row < mGradientMat.rows; ++row){
		for(int col = 0; col < mGradientMat.cols; ++col){
			mGradientMat.at<uchar>(row, col) = static_cast<uchar>(std::sqrt(std::pow(runFilterX(regularImage, row + 1, col + 1), 2) + std::pow(runFilterY(regularImage, row + 1, col + 1), 2)));

		}
	}
}

cv::Mat& Gradient::getGradient(){
	return mGradientMat;
}
