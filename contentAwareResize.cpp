/*  Implementation file for Content Aware Resizing
 *
 *  \file contentAwareResize.cpp
 *  \author David Oluwagbemi
 *  \date 27 March 2023
 *
 *  \description
 *    This implementation will perform content aware resizing of an image and object removal
 */

#include <contentAwareResize.h>

using namespace std;

namespace ipcv {
	//Calculates the energy map of a provided image
	bool energyMap(const cv::Mat& src, cv::Mat& map) {
		cv::Mat srcGray;
		cv::cvtColor(src, srcGray, cv::COLOR_BGRA2GRAY);

		cv::Mat kernelX;
		cv::Mat kernelY;
		cv::Mat srcF;

		cv::Mat energyX;
		cv::Mat energyY;

		

		srcF = srcGray;
		srcF.convertTo(srcF, CV_32F);

		int ddepth = -1;
		int delta = 0;
		cv::Point anchor;
		anchor.x = -1;
		anchor.y = -1;

		//KernelX
		kernelX.create(3, 3, CV_32FC1);
		kernelX = 0;
		kernelX.at<float>(0, 0) = -1;
		kernelX.at<float>(0, 1) = -2;
		kernelX.at<float>(0, 2) = -1;
		kernelX.at<float>(2, 0) = 1;
		kernelX.at<float>(2, 1) = 2;
		kernelX.at<float>(2, 2) = 1;
		//kernelX /= 8;

		//KernelY
		kernelY.create(3, 3, CV_32FC1);
		kernelY = kernelX.t();

		//Energy Map
		cv::filter2D(srcF, energyX, ddepth, kernelX, anchor, delta, 1);
		cv::filter2D(srcF, energyY, ddepth, kernelY, anchor, delta, 1);

		map = abs(energyX) + abs(energyY);
		return true;
	}

	//Removes a seam, provided an image, its energy map and the seam number to be removed
	bool seamRemoval(const cv::Mat& dstTemp, cv::Mat& dst, const cv::Mat& map, int seamNum){
		int n = seamNum;
		cv::Point min;

		//Seam Removal
		for (int r = 0; r < map.rows - 1; r++) {
			cv::Mat nBor;
			
			nBor.create(1, 3, CV_32F);

			nBor.at<float>(0) = map.at<float>(r + 1, clamp(n - 1, 1, map.cols - 2));
			nBor.at<float>(1) = map.at<float>(r + 1, n);
			nBor.at<float>(2) = map.at<float>(r + 1, clamp(n + 1, 1, map.cols - 2));

			cv::minMaxLoc(nBor, NULL, NULL, &min, NULL);
			n = clamp(n + min.x - 1, 1, map.cols - 2);
			
			//Shift Pixels
			cv::Mat pixelShift(1, map.cols - n, CV_8UC4, cv::Scalar::all(0));
			cv::Rect shift(n + 1, r + 1, map.cols - n - 1, 1);

			dstTemp(shift).copyTo(pixelShift(cv::Rect(0, 0, map.cols - n - 1, 1)));

			pixelShift.copyTo(dstTemp(cv::Rect(n, r + 1, pixelShift.cols, 1)));
		}
		dstTemp(cv::Rect(0, 0, dstTemp.cols - 1, dstTemp.rows)).copyTo(dst);
		return true;
	}

	//Matches the energy history to the seam removal function
	bool seamRemovalE(cv::Mat& dstTemp, cv::Mat& dst, const cv::Mat& map, int seamNum) {
		int n = seamNum;

		cv::Point min;
		//Seam Removal - Vertical
		for (int r = 0; r < map.rows - 1; r++) {
			cv::Mat nBor;

			nBor.create(1, 3, CV_32F);

			nBor.at<float>(0) = map.at<float>(r + 1, clamp(n - 1, 1, map.cols - 2));
			nBor.at<float>(1) = map.at<float>(r + 1, n);
			nBor.at<float>(2) = map.at<float>(r + 1, clamp(n + 1, 1, map.cols - 2));

			cv::minMaxLoc(nBor, NULL, NULL, &min, NULL);
			n = clamp(n + min.x - 1, 1, map.cols - 2);
			//dstTemp.at<uint8_t>(r + 1, n) = 255;


			cv::Mat pixelShift(1, map.cols - n, CV_32F, cv::Scalar::all(0));
			cv::Rect shift(n + 1, r + 1, map.cols - n - 1, 1);

			dstTemp(shift).copyTo(pixelShift(cv::Rect(0, 0, map.cols - n - 1, 1)));

			pixelShift.copyTo(dstTemp(cv::Rect(n, r + 1, pixelShift.cols, 1)));
		}
		dstTemp(cv::Rect(0, 0, dstTemp.cols - 1, dstTemp.rows)).copyTo(dst);
		return true;
	}

	//Inserts a seam, provided an image, its energy map and the seam number to be inserted, logs history in provided mat
	bool seamInsertion(const cv::Mat& dstTemp, cv::Mat& dst, const cv::Mat& map, int seamNum, cv::Mat& energyHist) {
		int n = seamNum;
		cv::Point min;
		
		cv::Mat dstTempUltra(dstTemp.rows, dstTemp.cols + 1, CV_8UC4, cv::Scalar::all(0));
		cv::Mat mapUltra(map.rows, map.cols + 1, CV_32F, cv::Scalar::all(0));

		energyHist.copyTo(mapUltra(cv::Rect(0, 0, energyHist.cols, energyHist.rows)));
		dstTemp.copyTo(dstTempUltra(cv::Rect(0, 0, dstTemp.cols, dstTemp.rows)));
		
		//Seam Insertion
		for (int r = 0; r < map.rows - 1; r++) {
			cv::Mat nBor;

			nBor.create(1, 3, CV_32F);

			nBor.at<float>(0) = map.at<float>(r + 1, clamp(n - 1, 1, map.cols - 2));
			nBor.at<float>(1) = map.at<float>(r + 1, n);
			nBor.at<float>(2) = map.at<float>(r + 1, clamp(n + 1, 1, map.cols - 2));

			cv::minMaxLoc(nBor, NULL, NULL, &min, NULL);
			n = clamp(n + min.x - 1, 1, map.cols - 2);
			mapUltra.at<float>(r + 1, n) = 100;		//Store seam
			
			//Shift pixels
			cv::Mat pixelShift(1, map.cols - n, CV_8UC4, cv::Scalar::all(0));
			cv::Rect shift(n + 1, r + 1, map.cols - n - 1, 1);
			dstTemp(shift).copyTo(pixelShift(cv::Rect(1, 0, map.cols - n - 1, 1)));
			pixelShift.copyTo(dstTempUltra(cv::Rect(n + 1, r + 1, pixelShift.cols, 1)));
			
			//Average neighboring pixels to calculate seam
			uint16_t p1 = (dstTempUltra.at<cv::Vec4b>(r + 1, n)[0] + dstTempUltra.at<cv::Vec4b>(r + 1, n + 2)[0]) / 2.0;
			uint16_t p2 = (dstTempUltra.at<cv::Vec4b>(r + 1, n)[1] + dstTempUltra.at<cv::Vec4b>(r + 1, n + 2)[1]) / 2.0;
			uint16_t p3 = (dstTempUltra.at<cv::Vec4b>(r + 1, n)[2] + dstTempUltra.at<cv::Vec4b>(r + 1, n + 2)[2]) / 2.0;
			uint16_t p4 = (dstTempUltra.at<cv::Vec4b>(r + 1, n)[3] + dstTempUltra.at<cv::Vec4b>(r + 1, n + 2)[3]) / 2.0;
			
			dstTempUltra.at<cv::Vec4b>(r + 1, n + 1)[0] = p1;
			dstTempUltra.at<cv::Vec4b>(r + 1, n + 1)[1] = p2;
			dstTempUltra.at<cv::Vec4b>(r + 1, n + 1)[2] = p3;
			dstTempUltra.at<cv::Vec4b>(r + 1, n + 1)[3] = p4;
		}
		
		dstTempUltra.copyTo(dst);
		energyHist = mapUltra.clone();
		return true;
	}

	//Content Aware Resizing Algorithm
	bool caResize(const cv::Mat& src, cv::Mat& dst, uint32_t& w, uint32_t& h) {

		cv::Mat map;				//Energy map
		cv::Mat energyHist;			//Energy history for seam insertion
		cv::Mat dstTemp;			//Temp matrix for processing
		
		cv::Mat sumsV, sumsH;		//Array of vertical and horizontal seam sums
		float sum;					//Sum of current seam
		cv::Mat nBor;				//Neighboring pixel values
		nBor.create(1, 3, CV_32F);
		int n;				
		int numSeams = 1;			//Seam history count for seam history count insertion
		cv::Point minIdx;			//Minimum seam number
		double minV, minH = 0;		//Seam energy total
		int seamNumH, seamNumV;		//Seam number for removal/insertion
		
		src.copyTo(dstTemp);		
		int diffA = dstTemp.cols - w;
		int diffB = dstTemp.rows - h;

		cv::Mat bgra[4];
		

		uint32_t totSeams = abs(diffA) + abs(diffB);	//Total seams needed
		energyHist.create(dstTemp.rows, dstTemp.cols, CV_32FC1);

		//Loop for total seams
		for (uint32_t i = 0; i < totSeams; i++) {
			cv::Mat roi(dstTemp.rows, dstTemp.cols, CV_8U);
			cv::Mat keep(dstTemp.rows, dstTemp.cols, CV_32F);

			split(dstTemp, bgra);
			bgra[3].copyTo(roi);
			roi.convertTo(roi, CV_32F);
			
			keep = (roi - 1) * 255;
			roi *= pow(2.0, 10);

			//Recalculate Energy Map
			energyMap(dstTemp, map);
			map = map + energyHist - keep;
			
			sumsV.create(1, map.cols, CV_32F);
			sumsV = 0;
			sumsV.at<float>(0) = pow(2.0, 31);
			sumsH.create(1, map.rows, CV_32F);
			sumsH = 0;
			sumsH.at<float>(0) = pow(2.0, 31);
			
			//Energy Sums - Vertical
			for (int c = 1; c < map.cols; c++) {
				n = c; //current neighboring pixels column in seam
				sum = map.at<float>(0, c);
				for (int r = 0; r < map.rows - 1; r++) {
					nBor.at<float>(0) = map.at<float>(r + 1, clamp(n - 1, 1, map.cols - 2));
					nBor.at<float>(1) = map.at<float>(r + 1, n);
					nBor.at<float>(2) = map.at<float>(r + 1, clamp(n + 1, 1, map.cols - 2));
					cv::minMaxLoc(nBor, NULL, NULL, &minIdx, NULL);
					n = clamp(n + minIdx.x - 1, 1, map.cols - 2);
					sum += map.at<float>(r + 1, n);
				}
				sumsV.at<float>(c) = sum;
			}

			cv::rotate(map, map, cv::ROTATE_90_COUNTERCLOCKWISE);
			cv::rotate(energyHist, energyHist, cv::ROTATE_90_COUNTERCLOCKWISE);

			//Energy Sums - Horizontal
			for (int c = 1; c < map.cols; c++) {
				n = c; //current neighboring pixels column in seam
				sum = map.at<float>(0, c);
				for (int r = 0; r < map.rows - 1; r++) {
					nBor.at<float>(0) = map.at<float>(r + 1, clamp(n - 1, 1, map.cols - 2));
					nBor.at<float>(1) = map.at<float>(r + 1, n);
					nBor.at<float>(2) = map.at<float>(r + 1, clamp(n + 1, 1, map.cols - 2));
					cv::minMaxLoc(nBor, NULL, NULL, &minIdx, NULL);
					n = clamp(n + minIdx.x - 1, 1, map.cols - 2);
					sum += map.at<float>(r + 1, n);
				}
				sumsH.at<float>(c) = sum;
			}

			//Seam number - Removal
			cv::minMaxLoc(sumsV, &minV, NULL, &minIdx, NULL);
			seamNumV = minIdx.x;

			cv::minMaxLoc(sumsH, &minH, NULL, &minIdx, NULL);
			seamNumH = minIdx.x;


			//Seam Removal
			if ((minH < minV && dst.rows > h) || (dst.rows > h && dstTemp.cols == w)) {
				cv::rotate(dstTemp, dstTemp, cv::ROTATE_90_COUNTERCLOCKWISE);
				int n = seamNumH;
				cout << "Seam removed at row " << n << endl;
				seamRemoval(dstTemp, dst, map, n);
				seamRemovalE(energyHist, energyHist, map, n); //Update Energy History
				cv::rotate(dst, dst, cv::ROTATE_90_CLOCKWISE);
				cv::rotate(energyHist, energyHist, cv::ROTATE_90_CLOCKWISE);
			}
			else if ((minH >= minV && dst.cols > w) || (dst.cols > w && dstTemp.rows == h)) {
				cv::rotate(map, map, cv::ROTATE_90_CLOCKWISE);
				cv::rotate(energyHist, energyHist, cv::ROTATE_90_CLOCKWISE);
				int n = seamNumV;
				cout << "Seam removed at column " << n << endl;
				seamRemoval(dstTemp, dst, map, n);
				seamRemovalE(energyHist, energyHist, map, n); //Update Energy History
			}
			

			//Seam Insertion
			else if ((minH < minV && dst.rows < h) || (dst.rows < h && dst.cols == w)) {
				//Avoid Selection of same seams
				for (uint32_t j = 0; j < numSeams; j++) {
					cv::minMaxLoc(sumsH, &minH, NULL, &minIdx, NULL);
					sumsH.at<float>(minIdx.x) = pow(2.0, 31);
				}
				cv::minMaxLoc(sumsH, &minH, NULL, &minIdx, NULL);
				seamNumH = minIdx.x;

				cv::rotate(dstTemp, dstTemp, cv::ROTATE_90_COUNTERCLOCKWISE);
				int m = seamNumH;
				cout << "Seam inserted at row " << m << " " << endl;
				seamInsertion(dstTemp, dst, map, m, energyHist);
				cv::rotate(dst, dst, cv::ROTATE_90_CLOCKWISE);
				cv::rotate(energyHist, energyHist, cv::ROTATE_90_CLOCKWISE);
			}
			else if ((minH >= minV && dst.cols < w) || (dst.cols < w && dst.rows == h)) {
				//Avoid Selection of same seam
				for (uint32_t j = 0; j < numSeams; j++) {
					cv::minMaxLoc(sumsV, &minV, NULL, &minIdx, NULL);
					sumsV.at<float>(minIdx.x) = pow(2.0, 31);
				}
				cv::minMaxLoc(sumsV, &minV, NULL, &minIdx, NULL);
				seamNumV = minIdx.x;

				cv::rotate(map, map, cv::ROTATE_90_CLOCKWISE);
				cv::rotate(energyHist, energyHist, cv::ROTATE_90_CLOCKWISE);
				int m = seamNumV;
				cout << "Seam inserted at column " << m << endl;
				seamInsertion(dstTemp, dst, map, m, energyHist);
				cout << "y" << endl;
			}
			
			numSeams++;
			dst.copyTo(dstTemp);			//Update Matrix for Operations in Next Loop
			cv::imshow("In Progress", dst);
			map.convertTo(map, CV_8U);				//View Energy Map
			energyHist.convertTo(energyHist, CV_8U);//View Energy History	
			cv::imshow("Energy Map", map);
			cv::imshow("Energy History", energyHist);
			cv::waitKey(1);
			energyHist.convertTo(energyHist, CV_32F);
		}

		return true;
	}

	//Removes the selected region
	bool objectRemoval(const cv::Mat& src, cv::Mat& dst) {

		cv::Mat map;				//Energy map
		cv::Mat dstTemp;			//Temp matrix for processing

		cv::Mat sumsV, sumsH;		//Array of vertical and horizontal seam sums
		float sum;					//Sum of current seam
		cv::Mat nBor;				//Neighboring pixel values
		nBor.create(1, 3, CV_32F);
		int n;
		cv::Point minIdx;			//Minimum seam number
		double minV, minH = 0;		//Seam energy total
		int seamNumH, seamNumV;		//Seam number for removal/insertion

		src.copyTo(dstTemp);

		cv::Mat bgra[4];
		cv::Mat thresh;
		split(src, bgra);
		bgra[3].copyTo(thresh);
		thresh = (thresh - 1) * 255;

		

		//Loop until all selected pixels are removed
		while (true) {
			cv::Mat roi(dstTemp.rows, dstTemp.cols, CV_8U);
			cv::Mat remove(dstTemp.rows, dstTemp.cols, CV_8U);
			cv::Mat keep(dstTemp.rows, dstTemp.cols, CV_32F);
			split(dstTemp, bgra);
			bgra[3].copyTo(roi);
			remove = (roi - 1) * 255;
			cv::imshow("roi", remove);
			roi.convertTo(roi, CV_32F);
			remove.convertTo(remove, CV_32F);
			keep = (roi - 1) * 255 - remove;
			
			if (cv::sum(remove)[0] > cv::sum(thresh)[0]/10.0) {
				remove *= pow(2.0, 10);
				keep *= pow(2.0, 10);

				//Recalculate Energy Map
				cv::GaussianBlur(remove, remove, cv::Size(151, 151), 10, 0);
				cv::imshow("roiBlur", remove);
				energyMap(dstTemp, map);
				map = map - keep - remove;

				sumsV.create(1, map.cols, CV_32F);
				sumsV = 0;
				sumsV.at<float>(0) = pow(2.0, 31);
				sumsH.create(1, map.rows, CV_32F);
				sumsH = 0;
				sumsH.at<float>(0) = pow(2.0, 31);

				//Energy Sums - Vertical
				for (int c = 1; c < map.cols; c++) {
					n = c; //current neighboring pixels column in seam
					sum = map.at<float>(0, c);
					for (int r = 0; r < map.rows - 1; r++) {
						nBor.at<float>(0) = map.at<float>(r + 1, clamp(n - 1, 1, map.cols - 2));
						nBor.at<float>(1) = map.at<float>(r + 1, n);
						nBor.at<float>(2) = map.at<float>(r + 1, clamp(n + 1, 1, map.cols - 2));
						cv::minMaxLoc(nBor, NULL, NULL, &minIdx, NULL);
						n = clamp(n + minIdx.x - 1, 1, map.cols - 2);
						sum += map.at<float>(r + 1, n);
					}
					sumsV.at<float>(c) = sum;
				}

				cv::rotate(map, map, cv::ROTATE_90_COUNTERCLOCKWISE);

				//Energy Sums - Horizontal
				for (int c = 1; c < map.cols; c++) {
					n = c; //current neighboring pixels column in seam
					sum = map.at<float>(0, c);
					for (int r = 0; r < map.rows - 1; r++) {
						nBor.at<float>(0) = map.at<float>(r + 1, clamp(n - 1, 1, map.cols - 2));
						nBor.at<float>(1) = map.at<float>(r + 1, n);
						nBor.at<float>(2) = map.at<float>(r + 1, clamp(n + 1, 1, map.cols - 2));
						cv::minMaxLoc(nBor, NULL, NULL, &minIdx, NULL);
						n = clamp(n + minIdx.x - 1, 1, map.cols - 2);
						sum += map.at<float>(r + 1, n);
					}
					sumsH.at<float>(c) = sum;
				}

				//Seam number - Removal
				cv::minMaxLoc(sumsV, &minV, NULL, &minIdx, NULL);
				seamNumV = minIdx.x;

				cv::minMaxLoc(sumsH, &minH, NULL, &minIdx, NULL);
				seamNumH = minIdx.x;

				//Seam Removal
				if (minH < minV) {
					cv::rotate(dstTemp, dstTemp, cv::ROTATE_90_COUNTERCLOCKWISE);
					int n = seamNumH;
					cout << "Seam removed at row " << n << endl;
					seamRemoval(dstTemp, dst, map, n);
					cv::rotate(dst, dst, cv::ROTATE_90_CLOCKWISE);
				}
				else {
					cv::rotate(map, map, cv::ROTATE_90_CLOCKWISE);
					int n = seamNumV;
					cout << "Seam removed at column " << n << endl;
					seamRemoval(dstTemp, dst, map, n);
				}

				dst.copyTo(dstTemp);			//Update Matrix for Operations in Next Loop
				cv::imshow("In Progress", dst);
				map.convertTo(map, CV_8U);				//View Energy Map	
				cv::imshow("Energy Map", map);
				cv::waitKey(1);
			}

			else {
				break;
			}
		}

		return true;
	}
}
