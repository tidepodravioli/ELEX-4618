#include "CBallCamera.hpp"

bool CBallCamera::init_camera(int index, int videopref)
{
    m_camera = new VideoCapture;
    m_camera->open(0, CAP_V4L2);

    return m_camera->isOpened();
}

bool CBallCamera::get_frame(Mat &frame)
{
    Mat image;
    *m_camera >> image;

    return !image.empty();
}

bool CBallCamera::isTimeout()
{
    return flag_cameraTimeOut;
}

void CBallCamera::add_ball_target(Scalar lowerRange, Scalar upperRange)
{
    m_lowerBounds.push_back(lowerRange);
    m_upperBounds.push_back(upperRange);

    m_balltargetcount++;
}

void CBallCamera::remove_ball_target(int index)
{
    try
    {
        m_lowerBounds.erase(m_lowerBounds.begin() + index);
        m_upperBounds.erase(m_upperBounds.begin() + index);

        m_balltargetcount--;
    }
    catch(const exception &e)
    {
        cout << "CBallCamera : Index is invalid! No elements erased" << endl;
    }
}

bool CBallCamera::get_centers(vector<vector<Point2f>> centers)
{
    Mat image, hsv;
    vector<vector<Point2f>> result;
    
    for(int attempt = 1; attempt <= CAMERA_MAXBADFRAMES; attempt++)
    {
        if(get_frame(image))
            break;
        else continue;
    }

    cvtColor(image, hsv, COLOR_BGR2HSV);

    for(int target = 0; target < m_balltargetcount; target++)
    {
        Mat mask;
        inRange(hsv, m_lowerBounds[target], m_upperBounds[target], mask);

        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        vector<Point2f> centers;
        for(const auto & contour : contours)
        {
            if(contourArea(contour) > CAMERA_MINCONTOUR)
            {
                Point2f center;
                float radius;

                minEnclosingCircle(contour, center, radius);
                centers.push_back(center);
            }
        }

        result.push_back(centers);
    }
}


