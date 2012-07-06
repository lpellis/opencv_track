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
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    Mat edges;
    namedWindow("edges",1);

    Mat subimage;
    int count = 0;
    for(;;)
    {
    	count++;
        Mat frame;
        cap >> frame; // get a new frame from camera

        cvtColor(frame, edges, CV_BGR2GRAY);

        if (count == 1){
        	edges(Rect(300, 200, 50, 50)).copyTo(subimage);
//        	subimage = ;
//        	subimage.copyTo(subimage);
        }

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
    			Point(matchLoc.x + templ.cols+50, matchLoc.y + templ.rows+50),
    			Scalar::all(0), 2, 8, 0);
    	rectangle(edges, matchLoc,
    			Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
    			Scalar::all(0), 2, 8, 0);
//        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
//        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        imshow("sampleimg", subimage);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor




    return 0;
	int i = 3;
	int *k = new int[15];
	k[5] = 54;
	int a[] = { 1, 2, 3 };

	Mat m1 = Mat::ones(Size(2, 2), CV_32FC1) * 2;
	Mat m2 = Mat::eye(Size(2, 2), CV_32FC1) * 2;
	m2 = m2.t();
//	m1.at<float>(0, 0) = 8;



//	cout << m1 << endl << m2 << endl << m1.mul( m2, 1) << endl ;



	Summer<int> s(5);
	s.add(2.5);
	s.add(2);
	s.sub(1);
	s.sub(1);
	s.sub(1);
	s.sub(1);

//	cout << s.getSum() << endl;
	delete[] k;
	k = 0;

//	return 0;

/// Load image and template
	img = imread("big.jpg", 0);
	templ = imread("small.jpg", 0);


	kk = Mat::zeros(Size(img.cols, img.rows), 8);
	img.convertTo(kk, 8);


	cout << CV_64FC1 << endl;
	cout << kk.type() << endl;

//	img.copyTo(kk);
//	cv::convertScaleAbs(img, kk);
	cout << kk.type() << endl;
	cout << kk.t().type() << endl;
//	cv::multiply(kk, kk.t(), kk, 1.0, CV_32FC1);
	kk = kk * 2;
	kk = cv::min(kk, 100);

//	img = kk;
//	cout << kk;

//	return 0;
//	normalize(img, img, 0, 8.0, NORM_MINMAX, -1, Mat());
//	normalize(templ, templ, 0, 8.0, NORM_MINMAX, -1, Mat());


	/// Create windows
	namedWindow(image_window, CV_WINDOW_NORMAL);
	namedWindow(result_window, CV_WINDOW_NORMAL);
	namedWindow(test_window);
	cmat = img(Rect(0, 0, 20, 200));

/// Create Trackbar
	char* trackbar_label =
			"Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar,
			MatchingMethod);

	MatchingMethod(0, 0);

	waitKey(0);
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
	rectangle(img_display, matchLoc,
			Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
			Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc,
			Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
			Scalar::all(0), 2, 8, 0);

	imshow(image_window, img_display);
	imshow(result_window, result);

	imshow(test_window, kk);

	return;
}
