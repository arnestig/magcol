#include "ImageSourceVideoFile.h"

namespace ih
{

ImageSourceVideoFile::ImageSourceVideoFile()
{
}

//------------------------------------------------------------------------------
ImageSourceVideoFile::ImageSourceVideoFile(std::string file)
    :m_file(file)

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
    return -1;
}
//------------------------------------------------------------------------------

void ImageSourceVideoFile::stop()
{
}

} //namespace ih
