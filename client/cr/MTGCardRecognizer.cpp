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
    std::vector<cv::Rect> cardRoIs;
    m_detector.detectCard(img, cardRoIs);

    for (unsigned int i = 0; i < cardRoIs.size() && i < 10; ++i)
    {
        cv::Mat card = img(cardRoIs[i]);

	int x = int(card.cols*0.8);
	int y = int(card.rows*0.5);
        int w = card.cols - x;
	int h = int(card.rows*0.7) - y;
        cv::Rect symbolRoI = cv::Rect(x, y, w, h);

        cv::Mat symbol = card(symbolRoI);

#ifdef DEBUG
        std::stringstream sstr;
        sstr << "DetectedCard " << i;

        std::stringstream sstr2;
        sstr2 << sstr.str() << " symbol";

        cv::imshow(sstr.str(), card);
        cv::imshow(sstr2.str(), symbol);
        cv::waitKey(5);

	//Draw rectangle
        cv::rectangle(img, cardRoIs[i], cv::Scalar(255, 0, 255), 4, 8, 0);
#endif // DEBUG
    }

#ifdef DEBUG
    cv::imshow("MTGCardRecognizer debugging window", img);
    cv::waitKey(10);
#endif // DEBUG
}
//------------------------------------------------------------------------------






} //namespace cr
