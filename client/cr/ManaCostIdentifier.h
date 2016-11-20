#ifndef MANA_COST_IDENTIFIER_H
#define MANA_COST_IDENTIFIER_H


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <string>

namespace cr
{
    class ManaCostIdentifier
    {
    public:
        ManaCostIdentifier();
        ~ManaCostIdentifier();

        std::string getManaCost(cv::Mat img);
    private: //methods
        enum ManaColor
        {
            MANA_WHITE,
            MANA_BLUE,
            MANA_BLACK,
            MANA_RED,
            MANA_GREEN,
            MANA_COLORLESS,
            MANA_GENERIC,
            MANA_LIFE,
            MANA_SIZE
        };

        cv::Rect getManaRoI(cv::Mat img);

        const std::string& getCascadeFileName(ManaColor cm);
        cv::Scalar getManaColor(ManaColor cm);
    private:


        cv::CascadeClassifier m_cascadeMana[MANA_SIZE];
    };
} //namespace cr

#endif //MANA_COST_IDENTIFIER_H