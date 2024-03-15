#include <cstdlib>
#include <iostream>
#include <opencv4/opencv2/core.hpp>
#include <stdexcept>
#include <unistd.h>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include "Gradient.hpp"
#include "Cost.hpp"
#include "MatHelper.hpp"
#include "ZEDIncludes.hpp"
#include "EigenIncludes.hpp"


using namespace sl;

typedef Eigen::Matrix<double, 9, 1> Vector9d;
typedef Eigen::Matrix<double, 9, 8> Essential;

Eigen::MatrixXd calcYMatrix(std::vector<Eigen::Vector2d> const& src, std::vector<Eigen::Vector2d> const& dst){
	if(src.size() != dst.size()) throw std::runtime_error("Cannot Computer Least Squares on Different sized Vectors");
	Eigen::MatrixXd yMatrix;
	yMatrix.resize(2 * src.size(), 4);
	for(int i = 0; i < src.size(); i++){
		yMatrix(2 * i, 0) = src.at(i).x();
		yMatrix(2 * i, 1) = 1;
		yMatrix(2 * i, 2) = 0;
		yMatrix(2 * i, 3) = 0;
		yMatrix(2 * i + 1, 0) = 0;
		yMatrix(2 * i + 1, 1) = 0;
		yMatrix(2 * i + 1, 2) = src.at(i).y();
		yMatrix(2 * i + 1, 3) = 1;
	}
	return yMatrix;
}

Eigen::MatrixXd calcDestVector(std::vector<Eigen::Vector2d> const& src, std::vector<Eigen::Vector2d> const& dst){
	if(src.size() != dst.size()) throw std::runtime_error("Cannot Computer Least Squares on Different sized Vectors");
	Eigen::MatrixXd yMatrix;
	yMatrix.resize(2 * src.size(), 1);
	for(int i = 0; i < src.size(); i++){
		yMatrix(2 * i, 0) = dst.at(i).x();
		yMatrix(2 * i + 1, 0) = dst.at(i).y();
	}
	return yMatrix;
}

int main() {
	std::vector<Eigen::Vector2d> src	{
											{2,1},
											{1,2},
											{1,3},
											{1,4},
											{1,5},
											{1,6},
											{1,7},
											{1,8}
										};	

	std::vector<Eigen::Vector2d> dest	{
											{3,1},
											{2,2},
											{2,3},
											{2,4},
											{2,5},
											{2,6},
											{2,7},
											{2,8}
										};	

	Eigen::MatrixXd A = calcYMatrix(src, dest);
	Eigen::MatrixXd b = calcDestVector(src, dest);

	std::cout << "A: " << std::endl << A << std::endl;
	std::cout << "b: " << std::endl << b << std::endl;

	std::cout << "The solution using normal equations is:\n"
		 << (A.transpose() * A).ldlt().solve(A.transpose() * b) << std::endl;
	return 0;
}




