#ifndef IMAGE_SOURCE_IFC_H
#define IMAGE_SOURCE_IFC_H

#include "ImageSubscriberIfc.h"

namespace ih
{
    class ImageSourceIfc
    {
    public:
        ImageSourceIfc() {}
        virtual ~ImageSourceIfc() {}

        virtual void subscribe(ImageSubscriberIfc * subscriber) = 0;
        virtual int start() = 0;
        virtual void stop() = 0;

        virtual void setResolution(int width, int height) = 0;
    };

}

#endif //IMAGE_SOURCE_IFC_H