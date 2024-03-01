#pragma once

#include <cstdint>
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

	int mP1 = 1000;
	int mP2 = 10000;

	std::vector<cv::Mat> mCostMatrices; 

	uint16_t getMinInDir(int row, int col, int i, int j);

	uint16_t findBestOption(int row, int col, int dis, int i, int j);

public:
	Cost(Gradient left, Gradient right, int maxDisparity);

	cv::Mat& getCost();
};
