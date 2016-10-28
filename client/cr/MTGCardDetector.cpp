#include "MTGCardDetector.h"

#include <fstream>

namespace cr
{

    static const std::string CASCADE_CLASSIFIER_FILE = "resources/cascade.xml";

MTGCardDetector::MTGCardDetector()
{
    if (!m_cascade.load(CASCADE_CLASSIFIER_FILE))
    {
        printf("--(!)Error loading cascade file\n");
    }
    else
    {
        printf("--(!)Cascade loaded!\n");
    }

}

//------------------------------------------------------------------------------

MTGCardDetector::~MTGCardDetector()
{
}

//------------------------------------------------------------------------------


void MTGCardDetector::detectCard(cv::Mat img, std::vector<cv::Rect>& cards)
{
    if (!m_cascade.empty())
    {
        cv::Mat frame_gray;

        cv::cvtColor(img, frame_gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frame_gray, frame_gray);

        m_cascade.detectMultiScale(frame_gray, cards, 1.1, 2, 0 /*| cv::CASCADE_SCALE_IMAGE*/, cv::Size(110, 155));
    }
}
//------------------------------------------------------------------------------



} //namespace cr
