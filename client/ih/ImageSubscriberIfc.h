#ifndef IMAGE_SUBSCRIBER_IFC_H
#define IMAGE_SUBSCRIBER_IFC_H

#include "opencv2/imgproc.hpp"

namespace ih
{
    class ImageSubscriberIfc
    {
    public:
        ImageSubscriberIfc() {}
        virtual ~ImageSubscriberIfc() {}

        virtual void onImage(cv::Mat img) = 0;
    };

}

#endif // IMAGE_SUBSCRIBER_IFC_H
