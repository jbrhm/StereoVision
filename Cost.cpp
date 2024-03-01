#include "Cost.hpp"
#include <cstdint>
#include <cstdlib>
#include <opencv4/opencv2/core.hpp>

Cost::Cost(Gradient left, Gradient right, int maxDisparity) : mMaxDisparity(maxDisparity){
	mNumRows = left.getGradient().rows;
	mNumCols = left.getGradient().cols;

	for(int d = 0; d <= maxDisparity; d++){
		cv::Mat tempCost{mNumRows, mNumCols, 2, cv::Scalar{0,0,0}};
		for(int col = 0; col < mNumCols; col++){
			for(int row = 0; row < mNumRows; row++){
				if(col - d >= 0){
					tempCost.at<uint16_t>(row, col) = abs(left.at(row, col) - right.at(row, col - d));
				}else{
					tempCost.at<uint16_t>(row, col) = 65535;
				}
			}
		}
		mCostMatrices.push_back(tempCost);
	}
}

cv::Mat& Cost::getCost(){
	
}
