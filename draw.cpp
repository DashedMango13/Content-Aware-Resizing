/** Interface file for Content Aware Resizing
 *
 *  \file draw.h
 *  \author David Oluwagbemi
 *  \date 27 March 2023
 *
 *  \description
 *    This function enables the user to highlight/draw images using the mouse cursor
 */

#include "draw.h"

cv::Point prev_pos(-1, -1);
cv::Mat roi;

namespace ipcv2{
    // Callback function for mouse events
    void onMouseR(int event, int x, int y, int flags, void* userdata) {
        if (event == cv::EVENT_LBUTTONDOWN) {
            // Record the current mouse position as the previous position
            prev_pos = cv::Point(x, y);
        }
        else if (event == cv::EVENT_LBUTTONUP) {
            // Reset the previous mouse position
            prev_pos = cv::Point(-1, -1);
        }
        else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON)) {
            // If the left mouse button is down and the mouse is moving, draw a line
            if (prev_pos.x >= 0 && prev_pos.y >= 0) {
                line(roi, prev_pos, cv::Point(x, y), cv::Scalar(0, 0, 255, 2), 8);
            }
            // Record the current mouse position as the previous position
            prev_pos = cv::Point(x, y);
        }
        cv::imshow("Draw on Subject to Remove", roi);
    }

    void onMouseK(int event, int x, int y, int flags, void* userdata) {
        if (event == cv::EVENT_LBUTTONDOWN) {
            // Record the current mouse position as the previous position
            prev_pos = cv::Point(x, y);
        }
        else if (event == cv::EVENT_LBUTTONUP) {
            // Reset the previous mouse position
            prev_pos = cv::Point(-1, -1);
        }
        else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON)) {
            // If the left mouse button is down and the mouse is moving, draw a line
            if (prev_pos.x >= 0 && prev_pos.y >= 0) {
                line(roi, prev_pos, cv::Point(x, y), cv::Scalar(0, 255, 0, 0), 8);
            }
            // Record the current mouse position as the previous position
            prev_pos = cv::Point(x, y);
        }
        cv::imshow("Draw on Subject to Keep", roi);
    }

}