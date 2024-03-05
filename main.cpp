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


using namespace sl;

int main() {
    Camera zed;

    // Set configuration parameters
    InitParameters init_parameters;
    init_parameters.camera_resolution = RESOLUTION::VGA; // iUse HD720 opr HD1200 video mode, depending on camera type.
    init_parameters.camera_fps = 30; // Set fps at 30

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != ERROR_CODE::SUCCESS) {
		std::cout << "Error " << returned_state << ", exit program." << std::endl;
        return EXIT_FAILURE;
    }

    // Capture 50 frames and stop
    int i = 0;
    sl::Mat rightImage;
	sl::Mat leftImage;
    while (true) {
        // Grab an image
        returned_state = zed.grab();
        // A new image is available if grab() returns ERROR_CODE::SUCCESS
        if (returned_state == ERROR_CODE::SUCCESS) {

            // Get the left image
			zed.retrieveImage(leftImage, VIEW::LEFT);
			zed.retrieveImage(rightImage, VIEW::RIGHT);
			
			cv::Mat cvImageLeftPre = MatHelper::slMat2cvMat(leftImage);
			cv::Mat cvImageRightPre = MatHelper::slMat2cvMat(rightImage);

			cv::Mat cvImageLeft;
			cv::Mat cvImageRight;

			cv::resize(cvImageLeftPre, cvImageLeft, cv::Size(), 0.1, 0.1);
			cv::resize(cvImageRightPre, cvImageRight, cv::Size(), 0.1, 0.1);

			cv::Mat grayLeft;
			cv::Mat grayRight;
			cv::cvtColor(cvImageLeft, grayLeft, 6);
			cv::cvtColor(cvImageRight, grayRight, 6);

			//Show Images
			cv::Mat grayScaleLeft = Gradient(grayLeft).getGradient();
			cv::Mat grayScaleRight = Gradient(grayRight).getGradient();
			cv::imshow("ZED LEFT", grayScaleLeft);
			cv::waitKey();
			cv::imshow("ZED RIGHT", grayScaleRight);
			cv::waitKey();

			Cost cost{grayScaleLeft, grayScaleRight, 10};
			cv::Mat depthMatrix = cost.getCost();
			cv::imshow("Cost Matrix", depthMatrix);
			cv::waitKey();
			
			i++;
        	std::cout << "NUM INTERATIONS: " << i << std::endl;
		}
		else{
			return EXIT_FAILURE;
		}
    }

    // Close the camera
    zed.close();
    return EXIT_SUCCESS;;

//	cv::Mat image = cv::imread("../dog.jpeg")// Create a ZED camera object
//	cv::imshow("dawg", image);
//	cv::waitKey();
//
//	cv::Mat grayScale{image.rows, image.cols, 0, cv::Scalar{0,0,0}};
//	cv::cvtColor(image, grayScale, 6);
//
//	cv::imshow("gray", grayScale);
//	cv::waitKey();
//
//	cv::Mat grad = Gradient{grayScale}.getGradient();
//	std::cout << grad.depth() << std::endl;
//	MatHelper::normalizeImage(grad, static_cast<uchar>(0), static_cast<uchar>(255));
//
//	cv::imshow("DOG IMAGE", grad);
//	cv::waitKey();
//	std::cout << "Hello, World" << std::endl;
//	return 0;
}



