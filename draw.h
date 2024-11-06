/** Interface file for Content Aware Resizing
 *
 *  \file draw.h
 *  \author David Oluwagbemi
 *  \date 27 March 2023
 *
 *  \description
 *    This function enables the user to highlight/draw images using the mouse cursor
 */

#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

namespace ipcv2{
	void onMouseR(int event, int x, int y, int flags, void* userdata);

	void onMouseK(int event, int x, int y, int flags, void* userdata);
}
