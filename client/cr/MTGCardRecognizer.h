#ifndef MTG_CARD_RECOGNIZER_H
#define MTG_CARD_RECOGNIZER_H

#include "../ih/ImageSubscriberIfc.h"

#include "MTGCardDetector.h"
#include "MTGCardIdentifier.h"

namespace cr
{
    enum crFlags
    {
        CR_SAVE_CARDS = 1 << 0 // binary 0000 0001
    };

    class MTGCardRecognizer : public ih::ImageSubscriberIfc
    {
    public:
        MTGCardRecognizer();
        MTGCardRecognizer(int m_flags);
        virtual ~MTGCardRecognizer();

        virtual void onImage(cv::Mat img);

        void setOutputPath(std::string path);
    
    private: //methods

    private:
        MTGCardDetector m_detector;
        MTGCardIdentifier m_identifier;

        std::string m_outputPath;
        int m_flags;
        unsigned int m_cardCounter;
    };

}

#endif //MTG_CARD_RECOGNIZER_H