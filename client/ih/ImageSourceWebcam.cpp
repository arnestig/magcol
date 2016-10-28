#include "ImageSourceWebcam.h"

namespace ih
{

ImageSourceWebcam::ImageSourceWebcam() :
    m_running(false),
    m_webcamIdx(0)
{

}

//------------------------------------------------------------------------------

ImageSourceWebcam::ImageSourceWebcam(int webcamIdx) :
    m_running(false),
    m_webcamIdx(webcamIdx)
{

}

//------------------------------------------------------------------------------

ImageSourceWebcam::~ImageSourceWebcam()
{
}

//------------------------------------------------------------------------------

void ImageSourceWebcam::subscribe(ImageSubscriberIfc * subscriber)
{
    m_subscriber = subscriber;
}
//------------------------------------------------------------------------------

int ImageSourceWebcam::start()
{
    m_capture.open(m_webcamIdx);
    if (!m_capture.isOpened())
    {
        printf("--(!)Error opening video capture\n");
        return -1;
    }
    else
    {
        printf("--(!)Webcam opened!\n");
    }

    m_running = true;

    //frameGrabber();
    m_frameGrabberThread = std::thread(&ImageSourceWebcam::frameGrabber, this);

    return 0;
}
//------------------------------------------------------------------------------

void ImageSourceWebcam::stop()
{
    m_running = false;
    m_frameGrabberThread.join();

    m_capture.release();
}

//------------------------------------------------------------------------------

void ImageSourceWebcam::frameGrabber()
{
    cv::Mat frame;

    while (m_running)
    {
        if (!m_capture.read(frame))
        {
            printf(" --(!) frame read error --!");
        }

        if (frame.empty())
        {
            printf(" --(!) No captured frame --!");
        }

        m_subscriber->onImage(frame);
    }
}

} //namespace ih
