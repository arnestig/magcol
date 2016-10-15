#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>


/// Function header
void thresh_callback(int, void* );
void find_squares(cv::Mat& image, std::vector<std::vector<cv::Point> >& squares);
void drawSquares( cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares );
int thresh = 100;
int max_thresh = 255;
cv::RNG rng(12345);
cv::Mat threshold_output, src, src_gray, src_blur,dst, color_dst, dilation_dst, cornersx;

int frame;

int main (int, char**)
{
    //Open the first webcam plugged in the computer
    cv::VideoCapture camera(0);
    if(!camera.isOpened())
    {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    //Create a window to display the images from the webcam
    cv::namedWindow("Webcam", CV_WINDOW_AUTOSIZE);

    //Pick up a new frame and display it until you press a key
    while(1)
    {
        //Capture the next frame from the webcam
        camera >> src;
        //thresh_callback( 0, 0 );

        //Show the image on the window
        /// Convert image to gray and blur it
        cv::cvtColor( src, src_gray, CV_BGR2GRAY );
        cv::blur( src_gray, src_blur, cv::Size(3,3) );
        cv::Canny( src_blur, dst, 200, 500, 3 );
        cv::cvtColor( dst, color_dst, CV_GRAY2BGR );

        cv::threshold( color_dst, threshold_output, thresh, 255, cv::THRESH_BINARY );

        /// Find contours
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours( dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0) );
        int dilation_size = 2;
        cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                cv::Point( dilation_size, dilation_size ) );

        /// Apply the dilation operation
        cv::dilate( color_dst, threshold_output, element );
        cv::imshow( "Dilation Demo", threshold_output );

        cv::imshow("Webcam", src);

        std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
        std::vector<cv::Rect> boundRect( contours.size() );
        std::vector<cv::Point2f>center( contours.size() );
        std::vector<float>radius( contours.size() );

        for( int i = 0; i < contours.size(); i++ )
        { approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( cv::Mat(contours_poly[i]) );
            minEnclosingCircle( (cv::Mat)contours_poly[i], center[i], radius[i] );
        }


        /// Draw polygonal contour
        cv::Mat drawing = cv::Mat::zeros( threshold_output.size(), CV_8UC1 );
        cv::drawContours( drawing, contours, -1, cv::Scalar(255),CV_FILLED);

        /// Detector parameters
        int blockSize = 5;
        int apertureSize = 3;
        double k = 0.04;
        cornersx = cv::Mat::zeros( cornersx.size(), CV_8UC3 );
        cv::cornerHarris( drawing, cornersx, blockSize, apertureSize, k, cv::BORDER_DEFAULT );
        /// Drawing a circle around corners
        for( int j = 0; j < cornersx.rows ; j++ )
        { for( int i = 0; i < cornersx.cols; i++ )
            {
                if( (int) cornersx.at<float>(j,i) > thresh )
                {
                    circle( cornersx, cv::Point( i, j ), 5,  cv::Scalar(0,0,255), 2, 8, 0 );
                }
            }
        }

        /// Show in a window
        cv::namedWindow( "Corners", CV_WINDOW_AUTOSIZE );
        cv::imshow( "Corners", cornersx );

        //Wait for a key to be pressed
        if (cv::waitKey(30) >= 0) break;
    }

    //Success. The program accomplished its mission and now it can go
    // to the heaven of programs.
    return 0;
}
// the function draws all the squares in the image
