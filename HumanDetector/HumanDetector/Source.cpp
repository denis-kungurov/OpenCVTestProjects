#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>

IplImage* image = 0;
IplImage* src = 0;
IplImage* dst = 0;
IplImage* mono = 0;


int main(int argc, char* argv[])
{
	// имя картинки задаётся первым параметром
	char* filename = "ExampleImg/example_15_1.jpg";
	// получаем картинку
	image = cvLoadImage(filename, 1);
	// клонируем картинку 
	src = cvCloneImage(image);
	dst = cvCreateImage(CvSize(src->width/3, src->height/3),src->depth, src->nChannels);

	mono = cvCreateImage(cvGetSize(dst), IPL_DEPTH_8U, 1);



	printf("[i] image: %s\n", filename);
	assert(src != 0);

	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);

	cvSmooth(src, src, CV_MEDIAN, 51, 51);

	cvResize(src, dst, 0);


	cvCvtColor(dst, mono, CV_RGB2GRAY);
	cvCanny(mono, mono, 60, 100, 3);

	// показываем картинку
	cvShowImage("original", mono);


	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&src);
	// удаляем окно
	cvDestroyWindow("original");
	return 0;
}