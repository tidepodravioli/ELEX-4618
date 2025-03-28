#include <opencv2/opencv.hpp>
#include <iostream>

#define CVUI_IMPLEMENTATION
#include <cvui.h>

using namespace cv;
using namespace std;

void detectColor(Mat &image, const string& colorName, Scalar lower, Scalar upper, Scalar textColor, vector<Point2f> &centers) {
    Mat hsv, mask;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    inRange(hsv, lower, upper, mask);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        if (contourArea(contour) > 500) {
            Point2f center;
            float radius;
            minEnclosingCircle(contour, center, radius);
            centers.push_back(center);

            circle(image, center, (int)radius, Scalar(0, 0, 0), 2);
            putText(image, colorName, Point(center.x - 20, center.y - 20), FONT_HERSHEY_SIMPLEX, 0.6, textColor, 2);
        }
    }
}


int main() {
    cvui::init("Detected Colors");

    int bin1low = 90, 
        bin1high = 140, 
        bin2low = 40, 
        bin2high = 70, 
        bin3low = 20, 
        bin3high = 30, 
        bin4low = 0, 
        bin4high = 19;
        
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

            Mat frame = image.clone();

            vector<Point2f> bin1centers, bin2centers, bin3centers, bin4centers;

            detectColor(frame, "BIN1", Scalar(bin1low, 100, 100), Scalar(bin1high, 255, 255), Scalar(0, 0, 0), bin1centers); //BLUE
            detectColor(frame, "BIN2", Scalar(bin2low, 100, 100), Scalar(bin2high, 255, 255), Scalar(0, 0, 0), bin2centers); //LIME
            detectColor(frame, "BIN3", Scalar(bin3low, 100, 100), Scalar(bin3high, 255, 255), Scalar(0, 0, 0), bin3centers); //YELLOW
            detectColor(frame, "BIN4", Scalar(bin4low, 100, 100), Scalar(bin4high, 255, 255), Scalar(0, 0, 0), bin4centers); //ORANGE

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

            rectangle(frame, Point(225,50), Point (425, 250), Scalar(0,0,0), 1);
            Rect pos2 = Rect(255, 50, 200, 200);

            bool emptyBin = true;

            if(emptyBin)
            for(const Point2f &bluePoint : bin1centers)
            {
                if(pos2.contains(bluePoint))
                {
                    cout << "BIN1" << endl;
                    emptyBin = false;
                    break;
                }
            }

            if(emptyBin)
            for(const Point2f &limePoint : bin2centers)
            {
                if(pos2.contains(limePoint))
                {
                    cout << "BIN2" << endl;
                    emptyBin = false;
                    break;
                }
            }

            if(emptyBin)
            for(const Point2f &yellowPoint : bin3centers)
            {
                if(pos2.contains(yellowPoint))
                {
                    cout << "BIN3" << endl;
                    emptyBin = false;
                    break;
                }
            }

            if(emptyBin)
            for(const Point2f &orangePoint : bin4centers)
            {
                if(pos2.contains(orangePoint))
                {
                    cout << "BIN4" << endl;
                    emptyBin = false;
                    break;
                }
            }

            if(emptyBin)
                cout << "EMPTY" << endl;

            cvui::imshow("Detected Colors", frame);

        }
        while(waitKey(1) != 'q');
    }
}
