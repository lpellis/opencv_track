#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

template<typename T>
class Summer {
private:
	T s;

public:
	Summer(T x) {
		s = x;
	}
	T getSum() {
		return s;
	}
	T add(T x) {
		s += x;
	}
	T sub(T x);
};
template<typename T>
T Summer<T>::sub(T x) {
	s -= x;
}
template<>
class Summer<int> {
private:
	int s;

public:
	Summer(int x) {
		s = x;
	}
	int getSum() {
		return s;
	}
	int add(int x) {
		s += x;
	}
	int sub(int x);
};

int Summer<int>::sub(int x) {
	s -= x;
}

/// Global Variables
Mat img;
Mat templ;
Mat result;
char* image_window = "Source Image";
char* test_window = "Test Image";
char* result_window = "Result window";
Mat cmat;
int match_method;
int max_Trackbar = 5;
Mat kk;

template<typename Type, typename Type2>
Type2 maxx(Type x, Type2 y) {
	if (x > y) {
		return x;
	}
	return y;
}

/// Function Headers
void MatchingMethod(int, void*);

/** @function main */
int main(int argc, char** argv) {
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	Mat edges;
	namedWindow("edges", 1);

	Mat subimage;
	Mat frame;
	int count = 0;

	while (true) {
		count++;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);
		cvtColor(edges, edges, CV_GRAY2BGR);

		rectangle(edges, Point(300, 200), Point(350, 250), CV_RGB(255, 0, 0), 2, 8, 0);

		imshow("edges", edges);

		if (waitKey(30) >= 0)
			break;

		if (count > 20) {
			break;
		}
	}

	cvtColor(frame, edges, CV_BGR2GRAY);
	edges(Rect(300, 200, 50, 50)).copyTo(subimage);

	while (true){

		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);

		/// Source image to display
		Mat img_display;
		edges.copyTo(img_display);

		/// Create the result matrix
		int result_cols = edges.cols - subimage.cols + 1;
		int result_rows = edges.rows - subimage.rows + 1;

		result.create(result_cols, result_rows, CV_32FC1);

		/// Do the Matching and Normalize
		matchTemplate(edges, subimage, result, match_method);

		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

		/// Localizing the best match with minMaxLoc
		double minVal;
		double maxVal;
		Point minLoc;
		Point maxLoc;
		Point matchLoc;

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
			matchLoc = minLoc;
		} else {
			matchLoc = maxLoc;
		}

		/// Show me what you got
		rectangle(edges, matchLoc,
				Point(matchLoc.x + templ.cols + 50, matchLoc.y + templ.rows + 50), Scalar::all(0),
				2, 8, 0);

		imshow("edges", edges);
		imshow("sampleimg", subimage);
		if (waitKey(30) >= 0){
			break;
		}
	}
	// the camera will be deinitialized automatically in VideoCapture destructor

	return 0;

}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod(int, void*) {
	/// Source image to display
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(img, templ, result, match_method);

	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
		matchLoc = minLoc;
	} else {
		matchLoc = maxLoc;
	}

	/// Show me what you got
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
			Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
			Scalar::all(0), 2, 8, 0);

	imshow(image_window, img_display);
	imshow(result_window, result);

	imshow(test_window, kk);

	return;
}
