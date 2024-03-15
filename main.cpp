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

typedef Eigen::Matrix<double, 9, 1> Vector9d;
typedef Eigen::Matrix<double, 9, 8> Essential;

Vector9d calculateYVector(Eigen::Vector2d y, Eigen::Vector2d yPrime){
	Vector9d y9;
	y9 << yPrime.x() * y.x(),
	   	  yPrime.x() * y.y(),
		  yPrime.x(),
		  yPrime.y() * y.x(),
		  yPrime.y() * y.y(),
		  yPrime.y(),
		  y.x(),
		  y.y(),
		  1.0;
	return y9;
}

Essential calcYMatrix(std::vector<Eigen::Vector2d> const& src, std::vector<Eigen::Vector2d> const& dst){
	Essential yMatrix;
	assert(src.size() == 8);
	assert(dst.size() == 8);

	for(int i = 0; i < 8; i++){
		yMatrix.col(i) = calculateYVector(src.at(i), dst.at(i));
	}

	return yMatrix;
}

int main() {
	std::vector<Eigen::Vector2d> src	{
											{1,1},
											{1,2},
											{1,3},
											{1,4},
											{1,5},
											{1,6},
											{1,7},
											{1,8}
										};	

	std::vector<Eigen::Vector2d> dest	{
											{-1,1},
											{-2,1},
											{-3,1},
											{-4,1},
											{-5,1},
											{-6,1},
											{-7,1},
											{-8,1}
										};	

	Essential A = calcYMatrix(src, dest);
	Eigen::FullPivLU<Eigen::MatrixXd> lu(A.transpose());
	Eigen::MatrixXd A_null_space = lu.kernel();
	std::cout << A_null_space << std::endl;

	Vector9d b;
	b << 0.001,0.001,0.001,0.001,0.001,0.001,0.001,0.001,0.001;

	std::cout << "The solution using normal equations is:\n"
		 << (A.transpose() * A).ldlt().solve(A.transpose() * b) << std::endl;
	return 0;
}




