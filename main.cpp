#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include "Gradient.hpp"
#include "MatHelper.hpp"
#include "ZEDIncludes.hpp"


using namespace sl;

int main() {

	
    // Create a ZED camera object
    Camera zed;

    // Open the camera
    ERROR_CODE returned_state = zed.open();
    if (returned_state != ERROR_CODE::SUCCESS) {
        std::cout << "Error " << returned_state << ", exit program.\n";
        return EXIT_FAILURE;
    }

    // Get camera information (ZED serial number)
    auto camera_infos = zed.getCameraInformation();
    printf("Hello! This is my serial number: %d\n", camera_infos.serial_number);

    // Close the camera
    zed.close();
    return EXIT_SUCCESS;

	cv::Mat image = cv::imread("../dog.jpeg");

	cv::imshow("dawg", image);
	cv::waitKey();

	cv::Mat grayScale{image.rows, image.cols, 0, cv::Scalar{0,0,0}};
	cv::cvtColor(image, grayScale, 6);

	cv::imshow("gray", grayScale);
	cv::waitKey();

	cv::Mat grad = Gradient{grayScale}.getGradient();
	std::cout << grad.depth() << std::endl;
	MatHelper::normalizeImage(grad, static_cast<uchar>(0), static_cast<uchar>(255));

	cv::imshow("DOG IMAGE", grad);
	cv::waitKey();
	std::cout << "Hello, World" << std::endl;
	return 0;
}



