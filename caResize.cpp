// caResize.cpp : This file contains the 'main' function. Program execution begins and ends there.
/* Main file for Content Aware Resizing
 *
 *  \file caResize.cpp
 *  \author David Oluwagbemi
 *  \date 27 March 2023
 *
 *  \description
 *    This file contains the 'main' function for the content aware resizing application.
 */
#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <contentAwareResize.h>

using namespace std;

namespace po = boost::program_options;

cv::Point prev_pos(-1, -1);
cv::Mat roi;

//CALLBACK FUNCTIONS FOR MOUSE INPUTS
//Remove Area Highlighter
void onMouseR(int event, int x, int y, int flags, void* userdata){
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

//Keep Area Highlighter
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

int main(int argc, char* argv[]) {
    bool verbose = false;
    bool keep = false;
    bool remove = false;
    bool R = false;
    string src_filename = "";
    string dst_filename = "";
    uint32_t tgt_width =  0;
    uint32_t tgt_height = 0;

    po::options_description options("Options");
    options.add_options()("help,h", "display this message")(
    "verbose,v", po::bool_switch(&verbose), "verbose [default is silent. can be used alone or with other options for picture info]")(
    "keep,k", po::bool_switch(&keep), "keep area unchanged when resizing [use with other options to influence outputs. \nEx.\"caResize input.jpg -r -k\"]")(
    "remove and resize,r", po::bool_switch(&remove), "removes selection then returns image to original size [Use -R instead for no resizing after removal]")(
    "remove,R", po::bool_switch(&R), "remove selection without maintaining image size")(
    "source-filename,i", po::value<string>(&src_filename), "source filename")(
    "destination-filename,o", po::value<string>(&dst_filename), "destination filename")(
    "tgt-width,x", po::value<uint32_t>(&tgt_width), "target width, columns [can be used with k to change results]")(
    "tgt-height,y", po::value<uint32_t>(&tgt_height), "target height [can be used with k to change results]");

    po::positional_options_description positional_options;
    positional_options.add("source-filename", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
        .options(options)
        .positional(positional_options)
        .run(),
        vm);
    po::notify(vm);
   
    if (vm.count("help")) {
        cout << "Usage: " << argv[0] << " [options] source-filename" << endl;
        cout << options << endl;
        return EXIT_SUCCESS;
    }

    if (!boost::filesystem::exists(src_filename)) {
        cerr << "Provided source file does not exists" << endl;
        return EXIT_FAILURE;
    }

    cv::Mat src = cv::imread(src_filename, cv::IMREAD_COLOR);

    if (tgt_width == 0) {
        tgt_width = src.cols;
    }

    if (tgt_height == 0) {
        tgt_height = src.rows;
    }

    if (verbose) {
        cout << "Source filename: " << src_filename << endl;
        cout << "Size: " << src.size() << endl;
        cout << "Channels: " << src.channels() << endl;
        cout << "Target Height: " << tgt_height << endl;
        cout << "Target Width: " << tgt_width << endl;
        cout << "Destination filename: " << dst_filename << endl;
    }

    

    cv::cvtColor(src, src, cv::COLOR_BGR2BGRA);

    for (int r = 0; r < src.rows; r++) {
        for (int c = 0; c < src.cols; c++) {
            src.at<cv::Vec4b>(r, c)[3] = 1;
        }
    }
    src.copyTo(roi);
    if (keep) {
        cv::namedWindow("Draw on Subject to Keep");
        cv::setMouseCallback("Draw on Subject to Keep", onMouseK);
        cv::imshow("Draw on Subject to Keep", roi);
        cv::waitKey(0);

        for (int r = 0; r < src.rows; r++) {
            for (int c = 0; c < src.cols; c++) {
                src.at<cv::Vec4b>(r, c)[3] = roi.at<cv::Vec4b>(r, c)[3];
            }
        }
    }
    if (remove || R) {
        cv::namedWindow("Draw on Subject to Remove");
        cv::setMouseCallback("Draw on Subject to Remove", onMouseR);
        cv::imshow("Draw on Subject to Remove", roi);
        cv::waitKey(0);

        for (int r = 0; r < src.rows; r++) {
            for (int c = 0; c < src.cols; c++) {
                src.at<cv::Vec4b>(r, c)[3] = roi.at<cv::Vec4b>(r, c)[3];
            }
        }

    }

    clock_t startTime = clock();

    cv::Mat dst;
    src.copyTo(dst);

    if (remove && !R) {
        cv::Mat tempR;
        ipcv::objectRemoval(src, tempR);
        tempR.copyTo(dst);
        ipcv::caResize(tempR, dst, tgt_width, tgt_height);
    }
    else if(R || (R && remove)){
        ipcv::objectRemoval(src, dst);
    }
    else {
        ipcv::caResize(src, dst, tgt_width, tgt_height);
    }
    clock_t endTime = clock();

    if (verbose) {
        cout << "Elapsed time: "
            << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
            << " [s]" << endl;
        cout << "Size: " << dst.size() << endl;
    }

    //100% ALPHA CHANNEL for image formats with transparency
    for (int r = 0; r < dst.rows; r++) {
        for (int c = 0; c < dst.cols; c++) {
            dst.at<cv::Vec4b>(r, c)[3] = 255;
        }
    }

    if (dst_filename.empty()) {
        cv::imshow(src_filename, src);
        cv::imshow(src_filename + " [caResize]", dst);
        cv::waitKey(0);
    }

    else {
        cv::imwrite(dst_filename, dst);
    }

    return EXIT_SUCCESS;
}