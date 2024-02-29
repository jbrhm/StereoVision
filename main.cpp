#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include "Gradient.hpp"
#include "MatHelper.hpp"
#include "ZEDIncludes.hpp"


using namespace sl;
using namespace std;

int main() {
    Camera zed;

    // Set configuration parameters
    InitParameters init_parameters;
    init_parameters.camera_resolution = RESOLUTION::AUTO; // Use HD720 opr HD1200 video mode, depending on camera type.
    init_parameters.camera_fps = 30; // Set fps at 30

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != ERROR_CODE::SUCCESS) {
        cout << "Error " << returned_state << ", exit program." << endl;
        return EXIT_FAILURE;
    }

    // Capture 50 frames and stop
    int i = 0;
    sl::Mat image;
    while (i < 50) {
        // Grab an image
        returned_state = zed.grab();
        // A new image is available if grab() returns ERROR_CODE::SUCCESS
        if (returned_state == ERROR_CODE::SUCCESS) {

            // Get the left image
            zed.retrieveImage(image, VIEW::LEFT);
			cv::Mat cvMat = MatHelper::slMat2cvMat(image);
			cv::imshow("ZED", cvMat);
			cv::waitKey();
            
            // Display the image resolution and its acquisition timestamp
            cout<<"Image resolution: "<< image.getWidth()<<"x"<<image.getHeight() <<" || Image timestamp: "<<image.timestamp.data_ns<<endl;
            i++;
        }
    }

    // Close the camera
    zed.close();
    return EXIT_SUCCESS;;


	//cv::Mat image = cv::imread("../dog.jpeg")// Create a ZED camera object
	//cv::imshow("dawg", image);
	//cv::waitKey();

	//cv::Mat grayScale{image.rows, image.cols, 0, cv::Scalar{0,0,0}};
	//cv::cvtColor(image, grayScale, 6);

	//cv::imshow("gray", grayScale);
	//cv::waitKey();

	//cv::Mat grad = Gradient{grayScale}.getGradient();
	//std::cout << grad.depth() << std::endl;
	//MatHelper::normalizeImage(grad, static_cast<uchar>(0), static_cast<uchar>(255));

	//cv::imshow("DOG IMAGE", grad);
	//cv::waitKey();
	//std::cout << "Hello, World" << std::endl;
	//return 0;
}



