#ifndef MTG_CARD_IDENTIFIER_H
#define MTG_CARD_IDENTIFIER_H

#include "opencv2/imgproc.hpp"

namespace cr
{
    class MTGCardIdentifier
    {
    public:
        MTGCardIdentifier();
        virtual ~MTGCardIdentifier();
		
		void identifyCard(cv::Mat img);
    
    private: //methods

    private:

    };

}

#endif //MTG_CARD_IDENTIFIER_H