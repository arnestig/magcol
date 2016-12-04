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

        class ManaSymbolCandidate
        {
        public:
            ManaSymbolCandidate() {}
            ManaSymbolCandidate(cv::Rect rect, ManaColor color)
            :m_rect(rect), m_color(color) {}
            
            bool operator<(const ManaSymbolCandidate& msc)
            {

                return m_rect.x < msc.m_rect.x;
            }

            int overlap(const ManaSymbolCandidate& msc)
            {
                int overlap = 0;
                if (m_rect.x < msc.m_rect.x)
                {
                    overlap = 
                        (m_rect.x + m_rect.width) - msc.m_rect.x;
                }
                else
                {
                    overlap =
                        (msc.m_rect.x + msc.m_rect.width) - m_rect.x;
                }
            }

            cv::Rect rect() { return m_rect; }
            ManaColor color() { return m_color; }

            int x() { return m_rect.x; }
            int y() { return m_rect.y; }
            int width() { return m_rect.width; }
            int height() { return m_rect.height; }


        private:
            ManaColor m_color;
            cv::Rect m_rect;

            int points = 0;
        };
        typedef std::vector<ManaSymbolCandidate> ManaSymbolCandidates;

        cv::Rect getManaRoI(cv::Mat img);

        void getManaSymbolCandidates(cv::Mat img, ManaSymbolCandidates& candidates);

        void getClusteredColors(cv::Mat img, int colorCount, cv::Mat& colors);

        ManaColor getClosestManaColorRGB(cv::Mat colorCandidates);
        ManaColor getClosestManaColorHUE(cv::Mat colorCandidates);

        const std::string getCascadeFileName(ManaColor cm);
        cv::Scalar getManaColor(ManaColor mc);
        int rgb2hsv(
            const double r, const double g, const double b,
            double& h, double& s, double& v);

    private:
        cv::CascadeClassifier m_cascadeMana[MANA_SIZE];
    };

} //namespace cr

#endif //MANA_COST_IDENTIFIER_H