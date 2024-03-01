#include "Cost.hpp"
#include <cstdint>
#include <cstdlib>
#include <opencv4/opencv2/core.hpp>
#include <sys/types.h>

Cost::Cost(Gradient left, Gradient right, int maxDisparity) : mMaxDisparity(maxDisparity){
	mNumRows = left.getGradient().rows;
	mNumCols = left.getGradient().cols;

	for(int d = 0; d <= mMaxDisparity; d++){
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
	
	for(int row = 1; row < mNumRows; row++){
		for(int col = 1; col < mNumCols; col++){	
			for(int d = 0; d <= mMaxDisparity; d++){
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, 0);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, -1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, 1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 0, 1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 0, -1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, -1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, 0);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, 1);
			}
		}
	}

	for(int row = mNumRows-1; row >= 0; row--){
		for(int col = mNumCols-1; col >= 0; col--){	
			for(int d = 0; d <= mMaxDisparity; d++){
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, 0);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, -1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, 1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 0, 1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 0, -1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, -1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, 0);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, 1);
			}
		}
	}
}

uint16_t Cost::getMinInDir(int row, int col, int i, int j){
	uint16_t minValue = 65535;
	for(int d = 0; d <= mMaxDisparity; d++){
		if(minValue > mCostMatrices.at(d).at<uint16_t>(row + j, col + i)){
			minValue = mCostMatrices.at(d).at<uint16_t>(row + j, col + 1);
		}
	}
	return minValue;
}

uint16_t Cost::findBestOption(int row, int col, int dis, int i, int j){
	uint16_t all = getMinInDir(row, col, i, j) + mP2;
	uint16_t minOne;
	if(dis > 0){
		minOne = mCostMatrices.at(dis - 1).at<uint16_t>(row + j, col + i) + mP1;
	}else{
		minOne = 65535;
	}

	uint16_t plusOne;
	if(dis < mMaxDisparity){
		plusOne = mCostMatrices.at(dis + 1).at<uint16_t>(row + j, col + i) + mP1;
	}else{
		plusOne = 65535;
	}

	uint16_t reg = mCostMatrices.at(dis).at<uint16_t>(row + j, col + i);
	
	int prev;
	if(j < 0 || (i < 0 && j == 0)){
		prev = mCostMatrices.at(dis).at<uint16_t>(row, col - 1);
	}else{
		prev = mCostMatrices.at(dis).at<uint16_t>(row, col + 1);
	}

	return std::min(std::min(all, reg), std::min(plusOne, minOne)) - prev;
}

cv::Mat& Cost::getCost(){
	
}
