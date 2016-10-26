#ifndef IMAGE_SOURCE_VIDEOFILE_H
#define IMAGE_SOURCE_VIDEOFILE_H

#include "ImageSourceIfc.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <atomic>
#include <thread>

namespace ih
{
    class ImageSourceVideoFile : public ImageSourceIfc
    {
    public:
        ImageSourceVideoFile();
        ImageSourceVideoFile(std::string file);
        virtual ~ImageSourceVideoFile();

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

        std::string m_file;
    };

}

#endif //IMAGE_SOURCE_VIDEOFILE_H
