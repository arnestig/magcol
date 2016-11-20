#include "ManaCostIdentifier.h"

#include <opencv2/highgui/highgui.hpp>


#include <vector>

namespace cr
{
    static const std::string WHITE_CASCADE_CLASSIFIER_FILE = "resources/white_cascade.xml";
    static const std::string BLUE_CASCADE_CLASSIFIER_FILE = "resources/blue_cascade.xml";
    static const std::string BLACK_CASCADE_CLASSIFIER_FILE = "resources/black_cascade.xml";
    static const std::string RED_CASCADE_CLASSIFIER_FILE = "resources/red_cascade.xml";
    static const std::string GREEN_CASCADE_CLASSIFIER_FILE = "resources/green_cascade.xml";
    static const std::string COLORLESS_CASCADE_CLASSIFIER_FILE = "resources/colorless_cascade.xml";
    static const std::string GENERIC_CASCADE_CLASSIFIER_FILE = "resources/generic_cascade.xml";
    static const std::string LIFE_CASCADE_CLASSIFIER_FILE = "resources/life_cascade.xml";
    

    ManaCostIdentifier::ManaCostIdentifier()
    {
        for (int i = 0; i < MANA_SIZE; ++i)
        {
            if ( !m_cascadeMana[i].load(getCascadeFileName(ManaColor(i))) )
            {
                printf("--(!)Error loading cascade file\n");
            }
            else
            {
                printf("--(!)Cascade loaded!\n");
            }
        }
    }


    ManaCostIdentifier::~ManaCostIdentifier()
    {
    }

    std::string ManaCostIdentifier::getManaCost(cv::Mat img)
    {
        cv::Rect manaRoI = getManaRoI(img);
        cv::Mat subImg = img(manaRoI);

        cv::Size minDetectSize(subImg.cols*0.05, subImg.cols*0.05);
        cv::Size maxDetectSize(subImg.cols*0.15, subImg.cols*0.15);

        for (int i = 0; i < MANA_SIZE; ++i)
        {
            if (!m_cascadeMana[i].empty())
            {
                std::vector<cv::Rect> symbols;
                cv::Mat frame_gray;

                cv::cvtColor(subImg, frame_gray, cv::COLOR_BGR2GRAY);
                cv::equalizeHist(frame_gray, frame_gray);

                m_cascadeMana[i].detectMultiScale(
                    frame_gray, symbols, 1.01, 1, 0, minDetectSize, maxDetectSize);

                for (unsigned int s = 0; s < symbols.size() && s < 10; ++s)
                {
                    //Draw rectangle
                    cv::rectangle(subImg, symbols[s], getManaColor(ManaColor(i)), 3, 8, 0);
                }
            }
        }

        return "";
    }


    cv::Rect ManaCostIdentifier::getManaRoI(cv::Mat img)
    {
        int x = int(img.cols*0.5);
        int y = 0;
        int w = img.cols - x;
        int h = int(img.rows*0.15) - y;

        cv::Rect manaRoI = cv::Rect(x, y, w, h);
        return manaRoI;
    }


    const std::string & ManaCostIdentifier::getCascadeFileName(ManaColor cm)
    {
        switch (cm)
        {
        case MANA_WHITE:
            return WHITE_CASCADE_CLASSIFIER_FILE;
        case MANA_BLUE:
            return BLUE_CASCADE_CLASSIFIER_FILE;
        case MANA_BLACK:
            return BLACK_CASCADE_CLASSIFIER_FILE;
        case MANA_RED:
            return RED_CASCADE_CLASSIFIER_FILE;
        case MANA_GREEN:
            return GREEN_CASCADE_CLASSIFIER_FILE;
        case MANA_COLORLESS:
            return COLORLESS_CASCADE_CLASSIFIER_FILE;
        case MANA_GENERIC:
            return GENERIC_CASCADE_CLASSIFIER_FILE;
        case MANA_LIFE:
            return LIFE_CASCADE_CLASSIFIER_FILE;
        default:
            return "";
        }
    }

    cv::Scalar ManaCostIdentifier::getManaColor(ManaColor cm)
    {
        switch (cm)
        {
        case MANA_WHITE:
            return cv::Scalar(225, 225, 225);
        case MANA_BLUE:
            return cv::Scalar(255, 0, 0);
        case MANA_BLACK:
            return cv::Scalar(10, 10, 10);
        case MANA_RED:
            return cv::Scalar(0, 0, 255);
        case MANA_GREEN:
            return cv::Scalar(0, 255, 0);
        case MANA_COLORLESS:
            return cv::Scalar(150, 50, 150);
        case MANA_GENERIC:
            return cv::Scalar(50, 150, 50);
        case MANA_LIFE:
            return cv::Scalar(100, 100, 150);
        default:
            return cv::Scalar(123, 123, 132);
        }
    }

    
} //namespace cr

