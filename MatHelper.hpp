#include "OpencvIncludes.hpp"
#include "ZEDIncludes.hpp"
#include <exception>
#include <opencv2/core/matx.hpp>
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
				mat.at<uchar>(row, col) = static_cast<double>(max-min) * (mat.at<uchar>(row, col) - minSeen) / (static_cast<double>(maxSeen) - minSeen);
			}
		}
	}

	// Mapping between MAT_TYPE and CV_TYPE
	inline static int getOCVtype(sl::MAT_TYPE type) {
		int cv_type = -1;
		switch (type) {
			case sl::MAT_TYPE::F32_C1: cv_type = CV_32FC1; break;
			case sl::MAT_TYPE::F32_C2: cv_type = CV_32FC2; break;
			case sl::MAT_TYPE::F32_C3: cv_type = CV_32FC3; break;
			case sl::MAT_TYPE::F32_C4: cv_type = CV_32FC4; break;
			case sl::MAT_TYPE::U8_C1: cv_type = CV_8UC1; break;
			case sl::MAT_TYPE::U8_C2: cv_type = CV_8UC2; break;
			case sl::MAT_TYPE::U8_C3: cv_type = CV_8UC3; break;
			case sl::MAT_TYPE::U8_C4: cv_type = CV_8UC4; break;
			default: break;
		}
		return cv_type;
	}

	inline static std::vector<cv::Mat> getRGBMatrices(sl::Mat& input){
		cv::Mat zedInput = slMat2cvMat(input);
		std::vector<cv::Mat> channels;
		if(input.getChannels() != 3) throw std::runtime_error("Matrix is not size 3");
		for(int i = 0; i < 3; i++){
			cv::Mat temp(input.getHeight(), input.getWidth(), 0, cv::Scalar{0,0,0});
			for(int y = 0; y < input.getHeight(); y++){
				for(int x = 0; x < input.getWidth(); x++){
					temp.at<uchar>(y,x) = zedInput.at<cv::Vec3b>(y,x)[i];
				}
			}
			channels.push_back(temp);
		}
		return channels;
	}

	inline static cv::Mat slMat2cvMat(sl::Mat& input) {
		return cv::Mat(input.getHeight(), input.getWidth(), getOCVtype(input.getDataType()), input.getPtr<sl::uchar1>(sl::MEM::CPU), input.getStepBytes(sl::MEM::CPU));
	}
};
