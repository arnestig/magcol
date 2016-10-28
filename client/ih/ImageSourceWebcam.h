#ifndef IMAGE_SOURCE_WEBCAM_H
#define IMAGE_SOURCE_WEBCAM_H

#include "ImageSourceIfc.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <atomic>
#include <thread>

namespace ih
{
    class ImageSourceWebcam : public ImageSourceIfc
    {
    public:
        ImageSourceWebcam();
        ImageSourceWebcam(int webcamIdx);
        virtual ~ImageSourceWebcam();

        virtual void subscribe(ImageSubscriberIfc * subscriber);
        virtual int start();
        virtual void stop() ;
    
    private: //methods
        void frameGrabber();
    private:
        ImageSubscriberIfc * m_subscriber;
        std::thread m_frameGrabberThread;

        cv::VideoCapture m_capture;
        
        std::atomic<bool> m_running;

        int m_webcamIdx;
    };

}

#endif //IMAGE_SOURCE_WEBCAM_H
