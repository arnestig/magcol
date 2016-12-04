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

    //frameGrabber();
    m_frameGrabberThread = std::thread(&ImageSourceVideoFile::frameGrabber, this);

    return 0;
}
//------------------------------------------------------------------------------

void ImageSourceVideoFile::stop()
{

    m_running = false;
    m_frameGrabberThread.join();

    m_capture.release();
}

//------------------------------------------------------------------------------

void ImageSourceVideoFile::setResolution(int width, int height)
{
    m_capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
    m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
}

//------------------------------------------------------------------------------

void ImageSourceVideoFile::frameGrabber()
{
    cv::Mat frame;

    while (m_running)
    {
        if (!m_capture.read(frame))
        {
            printf(" --(!) video frame read error --!\n");
            break;
        }

        if (!frame.empty())
        {
            m_subscriber->onImage(frame);
        }
        else
        {
            printf(" --(!) No captured video frame --!");
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
    }
}

} //namespace ih
