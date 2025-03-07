#include "../../Common/stdafx.h"
#include <stdio.h>

void do_video();

int main()
{
    do_video();
    std::cout << "Program exiting... " << std::endl;
}

void do_video()
{
    cv::VideoCapture vid;
    vid.open(0, cv::CAP_V4L2);
    if (vid.isOpened() == true)
    {
	std::cout << "Camera starting..." << std::endl;
        do
        {
            cv::Mat frame, edges;
            vid >> frame;
            if (frame.empty() == false)
            {
                //cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
                //cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
                //cv::Canny(edges, edges, 0, 30, 3);
                //cv::imshow("edges", edges);
                cv::imshow("cameratest", frame);
            }
        }
        while (cv::waitKey(10) != 'q');
    }
    else std::cout << "Camera not opened..." << std::endl;
}
