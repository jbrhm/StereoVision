#pragma once

#include <cstdint>
#include <opencv4/opencv2/core.hpp>
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

	int mP1 = 5;
	int mP2 = 25;

	std::vector<cv::Mat> mCostMatrices; 


	cv::Mat mDepthMatrix;

	double getMinInDir(int row, int col, int i, int j);

	double findBestOption(int row, int col, int dis, int i, int j);

public:
	Cost(Gradient left, Gradient right, int maxDisparity);

	cv::Mat& getCost();
};
