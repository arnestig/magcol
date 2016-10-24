#ifndef MTG_CARD_RECOGNIZER_H
#define MTG_CARD_RECOGNIZER_H

#include "../ih/ImageSubscriberIfc.h"

#include "MTGCardDetector.h"
#include "MTGCardIdentifier.h"

namespace cr
{
    class MTGCardRecognizer : public ih::ImageSubscriberIfc
    {
    public:
        MTGCardRecognizer();
        virtual ~MTGCardRecognizer();

        virtual void onImage(cv::Mat img);
    
    private: //methods

    private:
        MTGCardDetector m_detector;
        MTGCardIdentifier m_identifier;
    };

}

#endif //MTG_CARD_RECOGNIZER_H