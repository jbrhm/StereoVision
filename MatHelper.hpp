#include "OpencvIncludes.hpp"
#include <exception>
#include <stdexcept>
#include <cuchar>

class MatHelper{
public:
	inline static void normalizeImage(cv::Mat & mat, uchar min, uchar max){
		if(mat.depth() != 0) throw std::runtime_error("Cannot use this function with > 1 channels");
		uchar minSeen;
		uchar maxSeen;
		for(int row = 0; row < mat.rows; row++){
			for(int col = 0; col < mat.cols; col++){
				if(minSeen > mat.at<uchar>(row, col)){
					minSeen = mat.at<uchar>(row, col);
				}
				if(maxSeen < mat.at<uchar>(row, col)){
					maxSeen = mat.at<uchar>(row, col);
				}
			}
		}

		for(int row = 0; row < mat.rows; row++){
			for(int col = 0; col < mat.cols; col++){
				mat.at<uchar>(row, col) = static_cast<double>(max) * (mat.at<uchar>(row, col) - minSeen) / (static_cast<double>(max) - min);
			}
		}
	}
};
