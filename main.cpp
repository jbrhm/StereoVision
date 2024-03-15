#include <cstdlib>
#include <iostream>
#include <opencv4/opencv2/core.hpp>
#include <unistd.h>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include "Gradient.hpp"
#include "Cost.hpp"
#include "MatHelper.hpp"
#include "ZEDIncludes.hpp"
#include "EigenIncludes.hpp"


using namespace sl;

int main() {
	Eigen::Vector3d ones{1,1,1};
	double dot = ones.dot(ones);
	std::cout << dot << std::endl;

	return 0;
}



