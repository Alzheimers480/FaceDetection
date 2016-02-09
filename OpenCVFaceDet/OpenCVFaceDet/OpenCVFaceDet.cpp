
#include "stdafx.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// Function Headers
void detectAndDisplay(Mat frame);

/* Global variables */
//String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
//CascadeClassifier eyes_cascade;

///* @function main */
//int main( int argc, char** argv ){
//    if( argc != 2)
//    {
//     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
//     return -1;
//    }
//	
//	//TODO add argument to select image or video feed
//    
//	Mat image;
//    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
//
//    if(! image.data )                              // Check for invalid input
//    {
//        cout <<  "Could not open or find the image" << std::endl ;
//        return -1;
//    }
//
//	detectAndDisplay(image);
//    waitKey(0);
//    return 0;
//}
//
////int main( void )
////{
////    VideoCapture capture;
////    Mat frame;
////    //-- 1. Load the cascades
////    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
////    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };
////    //-- 2. Read the video stream
////    capture.open( -1 );
////    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
////    while (  capture.read(frame) )
////    {
////        if( frame.empty() )
////        {
////            printf(" --(!) No captured frame -- Break!");
////            break;
////        }
////        //-- 3. Apply the classifier to the frame
////        detectAndDisplay( frame );
////        int c = waitKey(10);
////        if( (char)c == 27 ) { break; } // escape
////    }
////    return 0;
////}
//
///* @function detectAndDisplay */
//void detectAndDisplay( Mat frame ){
//	cout << "detect face method called";
//    std::vector<Rect> faces;
//    Mat frame_gray;
//    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
//    equalizeHist( frame_gray, frame_gray );
//    //-- Detect faces
//    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
//    for( size_t i = 0; i < faces.size(); i++ )
//    {
//		cout << "face found";
//        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
//        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
//        Mat faceROI = frame_gray( faces[i] );
//        std::vector<Rect> eyes;
//        //-- In each face, detect eyes
//        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
//        for( size_t j = 0; j < eyes.size(); j++ )
//        {
//            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
//            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
//        }
//    }
//    //-- Show what you got
//    imshow( window_name, frame );
//}

// Variables
// Copy this file from opencv/data/haarscascades to target folder
string face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
int filenumber; // Number of file to be saved
string filename;

// Function main
int main( int argc, char** argv )
{
    // Load the cascade
    if (!face_cascade.load(face_cascade_name)){
        printf("--(!)Error loading\n");
        return (-1);
    }

    // Read the image file
    Mat frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    // Apply the classifier to the frame
    if (!frame.empty()){
        detectAndDisplay(frame);
    }
    else{
        printf(" --(!) No captured frame -- Break!");
        return 0;
    }

    int c = waitKey(10);

    if (27 == char(c)){
        return 0;
    }

    return 0;
}

// Function detectAndDisplay
void detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat crop;
    Mat res;
    Mat gray;
    string text;
    stringstream sstm;

	cout << "detecting faces"  << "\n";
	cout << "Press any key to continue...";
	getchar();
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

	imshow( window_name, frame );
    // Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	cout << "Detected " << faces.size() << " faces." << "\n";
    
	// Set Region of Interest
    cv::Rect roi_b;
    cv::Rect roi_c;

    size_t ic = 0; // ic is index of current element
    int ac = 0; // ac is area of current element

    size_t ib = 0; // ib is index of biggest element
    int ab = 0; // ab is area of biggest element

    for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)
    {
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);

        ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);

        ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

        if (ac > ab)
        {
            ib = ic;
            roi_b.x = faces[ib].x;
            roi_b.y = faces[ib].y;
            roi_b.width = (faces[ib].width);
            roi_b.height = (faces[ib].height);
        }

        crop = frame(roi_c);
        resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
        cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

        // Form a filename
        filename = "";
        stringstream ssfn;
        ssfn << filenumber << ".png";
        filename = ssfn.str();
        filenumber++;

		//save and show output face
		cout << "Saving file: " << filename << "\n";
        imwrite(filename, gray);
		//namedWindow(filename, WINDOW_NORMAL);
		cout << "Opening file: " << filename << "\n";
		imshow( filename, gray );
		//cv::resizeWindow(filename, 100, 100);
        Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
        Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);

		cout << "Press any key to continue...";
		getchar();
    }

    // Show image
    sstm << "Crop area size: " << roi_b.width << "x" << roi_b.height << " Filename: " << filename;
    text = sstm.str();

    putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
    
	cout << "Saving file: " << "original.png" << "\n";
	imwrite("original.png", frame);
	cout << "Opening file: " << "original.png" << "\n";
	imshow("original", frame);
	
	waitKey(0);

    if (!crop.empty())
    {
        imshow("detected", crop);
    }
    else
        destroyWindow("detected");
}