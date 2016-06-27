#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

IplImage* image = 0;
IplImage* src = 0;
IplImage* dst = 0;
IplImage* dstOld = 0;
IplImage* detector = 0;
IplImage* mono = 0;



int main(int argc, char* argv[])
{
	int type = 1;


	for (int k = 1; k < 26; k++)
	{
		if (k == 2 || k == 4 || k == 13)
			continue;
		char filename[50];

		sprintf(filename, "ExampleImg/example_%d_1.jpg", k);


		image = cvLoadImage(filename, 1);

		src = cvCloneImage(image);

		dst = cvCreateImage(CvSize(src->width, src->height), src->depth, src->nChannels);
		dstOld = cvCreateImage(CvSize(src->width, src->height), src->depth, src->nChannels);

		detector = cvCreateImage(cv::Size(320, 400), src->depth, src->nChannels);

		mono = cvCreateImage(cvGetSize(dst), IPL_DEPTH_8U, 1);

		//printf("[i] image: %s\n", filename);
		assert(src != 0);


		if (type == 0){

			cvResize(src, dstOld, 0);
			cvSmooth(src, src, CV_MEDIAN, 11, 11);

			cvResize(src, dst, 0);


			cvCvtColor(dst, mono, CV_RGB2GRAY);
			cvCanny(mono, mono, 60, 100, 3);

			
			int minX = mono->width, minY = mono->height, maxX = 0, maxY = 0;
			//std::cout << minX << " " << minY << " " << maxX << " " << maxY << " " << std::endl;
			cv::Mat tmp = cv::cvarrToMat(mono);
			for (int i = 0; i < tmp.rows; i++)
			{
				//unsigned char *const scanLine(tmp.ptr<unsigned char>(i));
				for (int j = 0; j < tmp.cols; j++)
				{
					if (tmp.at<uchar>(i, j) == 255)
					{
						minX = (minX < i) ? minX : i;
						minY = (minY < j) ? minY : j;
						maxX = (maxX >= i) ? maxX : i;
						maxY = (maxY >= j) ? maxY : j;
					}
				}
			}
			//std::cout << minX << " " << minY << " " << maxX << " " << maxY << " " << std::endl;
			cvRectangle(dstOld, cvPoint(minY, minX), cvPoint(maxY, maxX), CV_RGB(0, 255, 0), 1, 8);
			//mono = new IplImage(tmp);
			sprintf(filename, "CannyDetector/example_%d_1.jpg", k);
			//cv::imwrite(filename, cv::cvarrToMat(dstOld));
			cvSaveImage(filename, dstOld);
		}
		else if (type == 1)
		{
			cvResize(src, dstOld, 0);

			cvResize(src, dst, 0);
			cv::HOGDescriptor hog;
			hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
			std::vector<cv::Rect> foundLocations;

			cv::Mat m = cv::cvarrToMat(src);
			cv::Mat window;
			cv::resize(m, window, cv::Size(src->width/2, src->height/2));
			hog.detectMultiScale(window, foundLocations, 0, cv::Size(2, 2), cv::Size(32, 32), 1.05, 2);
			//hog.detect(window, foundLocations);

			if (!foundLocations.empty())
			{
				std::cout << "Finded person" << std::endl;
				cv::Rect bigger = foundLocations[0];
				for (int i = 0; i < foundLocations.size(); i++){
					if (bigger.area() < foundLocations[i].area())
					{
						bigger = foundLocations[i];
					}
				}
				cv::rectangle(window, bigger.tl(), bigger.br(), cv::Scalar(0, 255, 0), 3);
			}
			mono = new IplImage(window);
			cvResize(mono, dstOld, 0);

			sprintf(filename, "HOGDetector/example_%d_1.jpg", k);
			//cv::imwrite(filename, cv::cvarrToMat(dstOld));
			cvSaveImage(filename, dstOld);
		}


		//delete(filename);
	}


	cvWaitKey(0);

	cvReleaseImage(&image);
	cvReleaseImage(&src);
	/*cvReleaseImage(&dst);
	cvReleaseImage(&dstOld);
	cvReleaseImage(&detector);
	cvReleaseImage(&mono);*/

	return 0;
}