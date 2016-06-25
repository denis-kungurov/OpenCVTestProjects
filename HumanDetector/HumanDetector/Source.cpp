#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

IplImage* image = 0;
IplImage* src = 0;
IplImage* dst = 0;
IplImage* detector = 0;
IplImage* mono = 0;

void filter_rects(const std::vector<cv::Rect>& candidates, std::vector<cv::Rect>& objects)
{
	size_t i, j;
	for (i = 0; i < candidates.size(); ++i)
	{
		cv::Rect r = candidates[i];

		for (j = 0; j < candidates.size(); ++j)
			if (j != i && (r & candidates[j]) == r)
				break;

		if (j == candidates.size())
			objects.push_back(r);
	}
}


int main(int argc, char* argv[])
{
	// имя картинки задаётся первым параметром
	char* filename = "ExampleImg/example_1_1.jpeg";
	// получаем картинку
	image = cvLoadImage(filename, 1);
	// клонируем картинку 
	src = cvCloneImage(image);
	dst = cvCreateImage(CvSize(src->width/3, src->height/3),src->depth, src->nChannels);

	//detector = cvCreateImage(cv::Size(64, 128), src->depth, src->nChannels);

	//mono = cvCreateImage(cvGetSize(dst), IPL_DEPTH_8U, 1);



	printf("[i] image: %s\n", filename);
	assert(src != 0);

	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);

	//cvSmooth(src, src, CV_MEDIAN, 51, 51);

	cvResize(src, dst, 0);


	//cvCvtColor(dst, mono, CV_RGB2GRAY);
	//cvCanny(mono, mono, 60, 100, 3);
	
	cv::HOGDescriptor hog;
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
	std::vector<cv::Rect> foundLocations, found_filtered;

		cv::Mat m = cv::cvarrToMat(src);
		//cv::Mat window;
		//cv::resize(m, window, cv::Size(64, 128));
		hog.detectMultiScale(m, foundLocations, 0, cv::Size(4, 4), cv::Size(32, 32), 1.05, 2);
		if (!foundLocations.empty())
		{
			std::cout << "Finded person" << std::endl;
		}
		filter_rects(foundLocations, found_filtered);

		for (int i = 0; i < found_filtered.size(); i++){
			cv::rectangle(m, found_filtered[i].tl(), found_filtered[i].br(), cv::Scalar(0, 255, 0), 3);
		}
		mono = new IplImage(m);
		cvResize(mono, dst, 0);
	// показываем картинку
	cvShowImage("original", dst);


	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&src);
	// удаляем окно
	cvDestroyWindow("original");
	return 0;
}