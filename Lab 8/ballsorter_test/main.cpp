#include <opencv2/opencv.hpp>
#include <iostream>

#define CVUI_IMPLEMENTATION
#include <cvui.h>

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

            circle(image, center, (int)radius, Scalar(0, 0, 0), 2); // Draw circle
            putText(image, colorName, Point(center.x - 20, center.y - 20),
                    FONT_HERSHEY_SIMPLEX, 0.6, textColor, 2); // Label color
        }
    }
}

int main() {
    // Load image
    Mat image = imread("C:\\Users\\alecb\\repo\\OOP\\Lab 8\\ballsorter_test\\testimage.jpg");
    if (image.empty()) {
        cerr << "Error: Could not load image!" << endl;
        return -1;
    }

    cvui::init("Detected Colors");

    int bin1low = 90, 
        bin1high = 140, 
        bin2low = 40, 
        bin2high = 70, 
        bin3low = 20, 
        bin3high = 30, 
        bin4low = 10, 
        bin4high = 20;

    do
    {
        Mat frame = image.clone();

        detectColor(frame, "BIN1", Scalar(bin1low, 100, 100), Scalar(bin1high, 255, 255), Scalar(0, 0, 0)); //BLUE
        detectColor(frame, "BIN2", Scalar(bin2low, 100, 100), Scalar(bin2high, 255, 255), Scalar(0, 0, 0)); //LIME
        detectColor(frame, "BIN3", Scalar(bin3low, 100, 100), Scalar(bin3high, 255, 255), Scalar(0, 0, 0)); //YELLOW
        detectColor(frame, "BIN4", Scalar(bin4low, 100, 100), Scalar(bin4high, 255, 255), Scalar(0, 0, 0)); //ORANGE

        cvui::beginRow(frame, 5, 5, 200, 500);
        cvui::window(200, 400, "Hue Adjustments");
        cvui::trackbar(frame, 16, 10, 180, &bin1low, 0, 180);
        cvui::trackbar(frame, 16, 60, 180, &bin1high, 0, 180);

        cvui::trackbar(frame, 16, 110, 180, &bin2low, 0, 180);
        cvui::trackbar(frame, 16, 160, 180, &bin2high, 0, 180);

        cvui::trackbar(frame, 16, 210, 180, &bin3low, 0, 180);
        cvui::trackbar(frame, 16, 260, 180, &bin3high, 0, 180);

        cvui::trackbar(frame, 16, 310, 180, &bin4low, 0, 180);
        cvui::trackbar(frame, 16, 360, 180, &bin4high, 0, 180);
        cvui::endRow();

        cvui::imshow("Detected Colors", frame);
    }
    while(waitKey(1) != 'q');

    

    return 0;
}
