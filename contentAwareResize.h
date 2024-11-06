/* Header file for Content Aware Resizing
 *
 *  \file contentAwareResize.h
 *  \author David Oluwagbemi
 *  \date 27 March 2023
 *
 *  \description
 *    This header will declare the functions for content aware resizing of an image and object removal
 */

#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

namespace ipcv {
	bool caResize(const cv::Mat& src, cv::Mat& dst, uint32_t& w, uint32_t& h);

	bool objectRemoval(const cv::Mat& src, cv::Mat& dst);
}
