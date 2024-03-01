#pragma once

#include <vector>
#include <iostream>
#include <cstddef>
#include <cstddef>
#include <cuchar>
#include <cmath>
#include "Gradient.hpp"
#include "OpencvIncludes.hpp"

class Cost {
private:
	cv::Mat mCostMat;
	int mNumRows;
	int mNumCols;

	int mMaxDisparity;

	std::vector<cv::Mat> mCostMatrices; 

public:
	Cost(Gradient left, Gradient right, int maxDisparity);

	cv::Mat& getCost();
};
