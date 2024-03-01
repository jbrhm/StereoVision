#include "Cost.hpp"
#include <cstdint>
#include <cstdlib>
#include <opencv4/opencv2/core.hpp>
#include <sys/types.h>
#include "MatHelper.hpp"

Cost::Cost(Gradient left, Gradient right, int maxDisparity) : mMaxDisparity(maxDisparity){
	mNumRows = left.getGradient().rows;
	mNumCols = left.getGradient().cols;
	std::cout << "Making Disparity Matrix" << std::endl;
	for(int d = 0; d <= mMaxDisparity; d++){
		cv::Mat tempCost{mNumRows, mNumCols, 2, cv::Scalar{0,0,0}};
		for(int col = 0; col < mNumCols; col++){
			for(int row = 0; row < mNumRows; row++){
				if((col - d )>= 0){
					tempCost.at<uint16_t>(row, col) = abs(left.at(row, col-d) - right.at(row, col));
				}else{
					tempCost.at<uint16_t>(row, col) = 65535;
				}
			}
		}
		mCostMatrices.push_back(tempCost);
	}
	
	std::cout << "Top Left Pass" << std::endl;
	for(int row = 1; row < mNumRows; row++){
		for(int col = 1; col < mNumCols; col++){	
			for(int d = 0; d <= mMaxDisparity; d++){
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 0, -1);
				//std::cout << findBestOption(row, col, d, 0, -1) << std::endl;
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, -1);
				//std::cout << findBestOption(row, col, d, -1, -1) << std::endl;
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, 0);
				//std::cout << findBestOption(row, col, d, -1, 0) << std::endl;
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, -1, 1);
				//std::cout << findBestOption(row, col, d, -1, 1) << std::endl;
			}
		}
		std::cout << row << std::endl;
	}

	std::cout << "Bottom Right Pass" << std::endl;
	for(int row = mNumRows-2; row >= 0; row--){
		for(int col = mNumCols-2; col >= 0; col--){	
			for(int d = 0; d <= mMaxDisparity; d++){
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, 0);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, -1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 1, 1);
				mCostMatrices.at(d).at<uint16_t>(row, col) += findBestOption(row, col, d, 0, 1);
			}
		}
		std::cout << row << std::endl;
	}
	
	std::cout << "Generating Final Depth Map" << std::endl;
	mDepthMatrix = cv::Mat{mNumRows, mNumCols, 0, cv::Scalar{0,0,0}};
	cv::Mat mDepthMatrix16 = cv::Mat{mNumRows, mNumCols, 2, cv::Scalar{0,0,0}};
	for(int row = mNumRows-1; row >= 0; row--){
		for(int col = mNumCols-1; col >= 0; col--){	
			uint16_t minD = 0;
			for(int d = 0; d <= mMaxDisparity; d++){
				if(mCostMatrices.at(minD).at<uint16_t>(row, col) > mCostMatrices.at(d).at<uint16_t>(row, col)){
					minD = d;
				}
			}
			mDepthMatrix16.at<uint16_t>(row, col) = minD;
			std::cout << "min: " << minD << std::endl;
		}
	}
	mDepthMatrix16.convertTo(mDepthMatrix, 0);
	MatHelper::normalizeImage(mDepthMatrix, 0, 255);
}

uint16_t Cost::getMinInDir(int row, int col, int i, int j){
	uint16_t minValue = 65535;
	for(int d = 0; d <= mMaxDisparity; d++){
		if(minValue > mCostMatrices.at(d).at<uint16_t>(row + j, col + i)){
			minValue = mCostMatrices.at(d).at<uint16_t>(row + j, col + i);
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
		prev = mCostMatrices.at(0).at<uint16_t>(row, col - 1);
	}else{
		prev = mCostMatrices.at(0).at<uint16_t>(row, col + 1);
	}

	return std::min(std::min(all, reg), std::min(plusOne, minOne)) - prev;
}

cv::Mat& Cost::getCost(){
	return mDepthMatrix;
}
