#ifndef IMAGE_SOURCE_VIDEOFILE_H
#define IMAGE_SOURCE_VIDEOFILE_H

#include "ImageSourceIfc.h"

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

    private:
        ImageSubscriberIfc * m_subscriber;
        std::thread m_frameGrabberThread;

        std::string m_file;
    };

}

#endif //IMAGE_SOURCE_VIDEOFILE_H