#pragma once

#include <vector>
#include <iostream>
#include <cstddef>
#include <cuchar>
#include <cmath>
#include "OpencvIncludes.hpp"

class Gradient {
private:
	static std::vector<double> const sobelFilterX;

	static std::vector<double> const sobelFilterY;

	cv::Mat mGradientMat;

	double runFilterX(cv::Mat const& regularImage, int row, int col);

	double runFilterY(cv::Mat const& regularImage, int row, int col);
public:
	Gradient(cv::Mat const& regularImage);

	double at(int row, int col);

	cv::Mat& getGradient();
};
