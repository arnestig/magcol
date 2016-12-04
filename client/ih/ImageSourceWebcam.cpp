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

void ImageSourceWebcam::setResolution(int width, int height)
{
    if (m_capture.isOpened())
    {
        m_capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
        m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }
}

//------------------------------------------------------------------------------

void ImageSourceWebcam::frameGrabber()
{
    cv::Mat frame;

    while (m_running)
    {
        if (!m_capture.read(frame))
        {
            printf(" --(!) webcam frame read error --!\n");
        }

        if (!frame.empty())
        {
            m_subscriber->onImage(frame);
        }
        else
        {
            printf(" --(!) No captured webcam frame --!\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
    }
}

} //namespace ih
