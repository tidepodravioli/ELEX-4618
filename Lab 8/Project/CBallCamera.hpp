
#include <opencv2/opencv.hpp>
#include <vector>

#define CAMERA_MAXBADFRAMES 32
#define CAMERA_MINCONTOUR 500

using namespace cv;
using namespace std;


/**
 * @brief Handles ball detection based on contours
 *  
 */
class CBallCamera
{
    private:
    VideoCapture * m_camera;

    int m_balltargetcount = 0;
    vector<Scalar> m_lowerBounds;
    vector<Scalar> m_upperBounds;

    bool flag_cameraTimeOut = false;
    
    bool get_frame(Mat &frame);

    public:
    bool init_camera(int index = 0, int videopref = CAP_V4L2);
    bool isTimeout();

    void add_ball_target(Scalar lowerRange, Scalar upperRange);
    void remove_ball_target(int index);

    bool get_centers(vector<vector<Point2f>> centers);
};