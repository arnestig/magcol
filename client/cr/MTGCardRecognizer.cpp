#include "MTGCardRecognizer.h"

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <thread>        
#include <chrono>   

namespace cr
{

#define DEBUG 1

MTGCardRecognizer::MTGCardRecognizer() :
    m_flags(0),
    m_cardCounter(0)
{
}
//------------------------------------------------------------------------------

MTGCardRecognizer::MTGCardRecognizer(int flags) :
    m_flags(flags),
    m_cardCounter(0)
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

    int x, y, w, h;

    for (unsigned int i = 0; i < cardRoIs.size() && i < 10; ++i)
    {
        cv::Mat card = img(cardRoIs[i]);
        ++m_cardCounter;

        if (m_flags & CR_SAVE_CARDS)
        {
            imwrite(m_outputPath +
                std::to_string(m_cardCounter) + ".jpg", card);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

	    x = int(card.cols*0.8);
	    y = int(card.rows*0.5);
        w = card.cols - x;
	    h = int(card.rows*0.7) - y;

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

void MTGCardRecognizer::setOutputPath(std::string path)
{
    m_outputPath = path;
}
//------------------------------------------------------------------------------






} //namespace cr
