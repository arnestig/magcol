#include "ImageSourceWebcam.h"

namespace ih
{

ImageSourceWebcam::ImageSourceWebcam() :
    m_running(false)
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
    m_capture.open(0);
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
