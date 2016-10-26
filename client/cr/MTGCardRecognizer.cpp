#include "MTGCardRecognizer.h"

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace cr
{

#define DEBUG 1

MTGCardRecognizer::MTGCardRecognizer()
{
}
//------------------------------------------------------------------------------

MTGCardRecognizer::~MTGCardRecognizer()
{
}
//------------------------------------------------------------------------------

void MTGCardRecognizer::onImage(cv::Mat img)
{
    std::vector<cv::Rect> rois;
    m_detector.detectCard(img, rois);

    for (int i = 0; i < rois.size() && i < 10; ++i)
    {
        std::stringstream sstr;
        sstr << "DetectedCard " << i;
        cv::Mat card = img(rois[i]);
        
        cv::Rect setSymbolRoI = cv::Rect(
            int(card.cols*0.8), int(card.rows*0.55), 
            card.cols - int(card.cols*0.8) , int(card.rows*0.7) - int(card.rows*0.55));
        cv::Mat setSymbol = card(setSymbolRoI);

        std::stringstream sstr2;
        sstr2 << sstr.str() << " symbol";

        cv::imshow(sstr.str(), card);
        cv::imshow(sstr2.str(), setSymbol);
        cv::waitKey(10);

        cv::rectangle(img, rois[i], cv::Scalar(255, 0, 255), 4, 8, 0);
    }

#ifdef DEBUG
    //std::this_thread::sleep_for(std::chrono::seconds(1));
   cv::imshow("MTGCardRecognizer debugging window", img);
   cv::waitKey(10);
#endif // DEBUG
}
//------------------------------------------------------------------------------






} //namespace cr
