#ifndef MTG_CARD_DETECTOR_H
#define MTG_CARD_DETECTOR_H

#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include <vector>

namespace cr
{
    class MTGCardDetector
    {
    public:
        MTGCardDetector();
        virtual ~MTGCardDetector();
		
		void detectCard(cv::Mat img, std::vector<cv::Rect>& cards);
    
    private: //methods

    private:
        cv::CascadeClassifier m_cascade;
    };

}

#endif //MTG_CARD_DETECTOR_H