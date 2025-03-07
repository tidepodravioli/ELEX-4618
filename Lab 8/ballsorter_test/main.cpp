#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Function to detect and label colored balls
void detectColor(Mat& image, const string& colorName, Scalar lower, Scalar upper, Scalar textColor) {
    Mat hsv, mask;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    inRange(hsv, lower, upper, mask);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        if (contourArea(contour) > 500) { // Ignore small noise
            Point2f center;
            float radius;
            minEnclosingCircle(contour, center, radius);

            circle(image, center, (int)radius, Scalar(255, 255, 255), 2); // Draw circle
            putText(image, colorName, Point(center.x - 20, center.y - 20),
                    FONT_HERSHEY_SIMPLEX, 0.6, textColor, 2); // Label color
        }
    }
}

int main() {
    // Load image
    cv::VideoCapture vid;
    vid.open(0, cv::CAP_V4L2);
    if (vid.isOpened() == true)
    {
        do
        {
            Mat image;
            vid >> image;
            if (image.empty()) {
                cerr << "Error: Could not load image!" << endl;
                return -1;
            }

            // Detect colors
            detectColor(image, "Blue", Scalar(90, 100, 100), Scalar(140, 255, 255), Scalar(0, 0, 0));
            detectColor(image, "Lime", Scalar(40, 100, 100), Scalar(70, 255, 255), Scalar(0, 0, 0));
            detectColor(image, "Yellow", Scalar(20, 100, 100), Scalar(30, 255, 255), Scalar(0, 0, 0));
            detectColor(image, "Orange", Scalar(10, 100, 100), Scalar(20, 255, 255), Scalar(0, 0, 0));

            // Show result
            imshow("Detected Colors", image);

        }
        while(waitKey(1) != 'q');
    }
}
