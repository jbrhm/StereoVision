#include "Cost.hpp"
#include <cstdint>
#include <cstdlib>
#include <opencv4/opencv2/core.hpp>
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>
#include "MatHelper.hpp"

Cost::Cost(Gradient left, Gradient right, int maxDisparity) : mMaxDisparity(maxDisparity){
	mNumRows = left.getGradient().rows;
	mNumCols = left.getGradient().cols;
	std::cout << "Making Disparity Matrix" << std::endl;
	for(int d = 0; d <= mMaxDisparity; d++){
		cv::Mat tempCost{mNumRows, mNumCols, 6, cv::Scalar{0,0,0}};
		for(int col = 0; col < mNumCols; col++){
			for(int row = 0; row < mNumRows; row++){
				if((col - d ) >= 0){
					tempCost.at<double>(row, col) = abs(left.at(row, col) - right.at(row, col-d));
				}else{
					tempCost.at<double>(row, col) = 2000000;
				}
			}
		}
		mCostMatrices.push_back(tempCost);
	}
	
	std::cout << "Top Left Pass" << std::endl;
	for(int row = 1; row < mNumRows; row++){
		for(int col = 1; col < mNumCols; col++){	
			for(int d = 0; d <= mMaxDisparity; d++){
				mCostMatrices.at(d).at<double>(row, col) += findBestOption(row, col, d, 0, -1);
				mCostMatrices.at(d).at<double>(row, col) += findBestOption(row, col, d, -1, -1);
				mCostMatrices.at(d).at<double>(row, col) += findBestOption(row, col, d, -1, 0);
				mCostMatrices.at(d).at<double>(row, col) += findBestOption(row, col, d, -1, 1);
			}
		}
		std::cout << row << std::endl;
	}

	std::cout << "Bottom Right Pass" << std::endl;
	for(int row = mNumRows-2; row >= 0; row--){
		for(int col = mNumCols-2; col >= 0; col--){	
			for(int d = 0; d <= mMaxDisparity; d++){
				mCostMatrices.at(d).at<double>(row, col) += findBestOption(row, col, d, 1, 0);
				mCostMatrices.at(d).at<double>(row, col) += findBestOption(row, col, d, 1, -1);
				mCostMatrices.at(d).at<double>(row, col) += findBestOption(row, col, d, 1, 1);
				mCostMatrices.at(d).at<double>(row, col) += findBestOption(row, col, d, 0, 1);
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
				if(mCostMatrices.at(minD).at<double>(row, col) > mCostMatrices.at(d).at<double>(row, col)){
					minD = static_cast<uint16_t>(d);
				}
			}
			mDepthMatrix16.at<uint16_t>(row, col) = minD;
			std::cout << "min: " << minD << " value: " << mCostMatrices.at(minD).at<double>(row, col) << std::endl;
		}
	}
	mDepthMatrix16.convertTo(mDepthMatrix, 0);
	MatHelper::normalizeImage(mDepthMatrix, 0, 255);
}

double Cost::getMinInDir(int row, int col, int i, int j){
	double minValue = 40000;
	for(int d = 0; d <= mMaxDisparity; d++){
		if(minValue > mCostMatrices.at(d).at<double>(row + j, col + i)){
			std::cout << "min, mCost: " << minValue << ", " << mCostMatrices.at(d).at<double>(row + j, col + i) << std::endl;
			minValue = mCostMatrices.at(d).at<double>(row + j, col + i);
		}
			//std::cout << "min, mCost: " << minValue << ", " << mCostMatrices.at(d).at<double>(row + j, col + i) << std::endl;
	}
	return minValue;
}

double Cost::findBestOption(int row, int col, int dis, int i, int j){
	double all = getMinInDir(row, col, i, j) + mP2;
	std::cout << "all " << all << std::endl;
	double minOne;
	if(dis > 0){
		minOne = mCostMatrices.at(dis - 1).at<double>(row + j, col + i) + mP1;
	}else{
		minOne = 40000;
	}
	std::cout << "minOne " << minOne << std::endl;

	double plusOne;
	if(dis < mMaxDisparity){
		plusOne = mCostMatrices.at(dis + 1).at<double>(row + j, col + i) + mP1;
	}else{
		plusOne = 40000;
	}
	std::cout << "plusOne " << plusOne << std::endl;

	double reg = mCostMatrices.at(dis).at<double>(row + j, col + i);
	std::cout << "reg " << reg << std::endl;
	
	int prev;
	if(j < 0 || (i < 0 && j == 0)){
		prev = getMinInDir(row, col, -1, 0);
	}else{
		prev = getMinInDir(row, col, 1, 0);
	}

	if(prev == 40000){
		std::cout << "row col " << row << " " << col << std::endl;
		throw std::runtime_error("2000");
	}

	std::cout << "prev: " << prev << std::endl;
	double best = std::min(std::min(all, reg), std::min(plusOne, minOne)) - getMinInDir(row, col, i, j);
	if(abs(best) > 1000){
		std::cout << "row col " << row << " " << col << ", best " << best << std::endl;
		throw std::runtime_error("2000");
	}
	return best;
}

cv::Mat& Cost::getCost(){
	return mDepthMatrix;
}
