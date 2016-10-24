#include "ImageSourceVideoFile.h"

namespace ih
{

ImageSourceVideoFile::ImageSourceVideoFile() :
    m_running(false)
{
}

//------------------------------------------------------------------------------
ImageSourceVideoFile::ImageSourceVideoFile(std::string file) :
    m_running(false),
    m_file(file)

{
}

//------------------------------------------------------------------------------

ImageSourceVideoFile::~ImageSourceVideoFile()
{
}

//------------------------------------------------------------------------------

void ImageSourceVideoFile::subscribe(ImageSubscriberIfc * subscriber)
{
    m_subscriber = subscriber;
}
//------------------------------------------------------------------------------

int ImageSourceVideoFile::start()
{
    m_capture.open(m_file);
    if (!m_capture.isOpened())
    {
        printf("--(!)Error opening video file capture \n");
        return -1;
    }
    else
    {
        printf("--(!)Video File opened!\n");
    }

    m_running = true;

    frameGrabber();
    //m_frameGrabberThread = std::thread(&ImageSourceWebcam::frameGrabber);

    return 0;
}
//------------------------------------------------------------------------------

void ImageSourceVideoFile::stop()
{

    m_running = false;
    m_frameGrabberThread.join();

    m_capture.release();
}

void ImageSourceVideoFile::frameGrabber()
{
    cv::Mat frame;

    while (m_running)
    {
        if (!m_capture.read(frame))
        {
            printf(" --(!) frame read error --!");
            break;
        }

        if (frame.empty())
        {
            printf(" --(!) No captured frame --!");
        }

        m_subscriber->onImage(frame);
    }
}

} //namespace ih
