#include <..\Common\stdafx.h>
#include <..\Common\CControl.h>
#include <..\Common\CJoystickPosition.hpp>

#include <stdio.h>
#include <conio.h>
#include <sstream>

#define CVUI_IMPLEMENTATION
#include <..\Common\cvui\cvui.h>

using namespace cv;
int main()
{
    CControl port;
    port.init_com(6);

    Mat canvas;
    Size canvasSize(800, 600);
    const int circleRadius = 100;
    
    bool exit = false;
    while(!exit)
    {
        bool pass = false;
        CJoystickPosition joystick = port.get_analog(pass);

        if(pass)
        {
            canvas = Mat::zeros(canvasSize, CV_8UC3);

            const int percentX = joystick.percentX();
            const int percentY = joystick.percentY();

            JOYSTICK_DIRECTION direction = joystick.get_simple_direction();

            cvui::beginRow(canvas, 10, 10);
            cvui::text("Joystick : ");
            stringstream joystickstats;
            joystickstats << "X : " << percentX << ", Y : " << percentY << " (DIRECTION : " << direction_op::get_direction_string(direction) << ")";
            cvui::text(joystickstats.str());
            cvui::endRow();
            
            const int centerX = canvasSize.width/2;
            const int centerY = canvasSize.height/2;

            const int indX = 2 * percentX + centerX - circleRadius;
            const int indY = -2 * (percentY - 100) + centerY - circleRadius;

            circle(canvas, Point(centerX, centerY), circleRadius, Scalar(255,255,255));
            circle(canvas, Point(indX, indY), 30, Scalar(255,255,255), -1);
            
            imshow("JOYSTICKTEST", canvas);
            exit = waitKey(1) == 'q';
        }        
    }
}