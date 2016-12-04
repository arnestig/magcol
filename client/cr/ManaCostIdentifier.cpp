#include "ManaCostIdentifier.h"

#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

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


    static const double MANA_SYMBOL_IMG_ROI_WIDTH = 0.5;
    static const double MANA_SYMBOL_IMG_ROI_HEIGHT = 0.15;
    static const double MANA_SYMBOL_MIN_WIDTH_SIZE_RATIO = 0.025;
    static const double MANA_SYMBOL_MAX_WIDTH_SIZE_RATIO = 0.075;

    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------

    ManaCostIdentifier::ManaCostIdentifier()
    {
        for (int i = 0; i < MANA_SIZE; ++i)
        {
            const std::string fileName = getCascadeFileName(ManaColor(i));
            if (m_cascadeMana[i].load(fileName))
            {
                printf("--(!)Cascade loaded: %s!\n", fileName.c_str());
            }
            else
            {
                printf("--(!)Error loading cascade file: %s!\n", fileName.c_str());
            }
        }
    }

    //------------------------------------------------------------------------------
    ManaCostIdentifier::~ManaCostIdentifier()
    {
    }


    //------------------------------------------------------------------------------
    std::string ManaCostIdentifier::getManaCost(cv::Mat img)
    {
        cv::Rect manaRoI = getManaRoI(img);
        cv::Mat subImg = img(manaRoI);

        ManaSymbolCandidates candidates;

        getManaSymbolCandidates(subImg, candidates);

        for (int c = 0; c < candidates.size(); ++c)
        {
            cv::Mat symbolImg = subImg(candidates[c].rect()).clone();

            cv::Mat colorCandidates;
            getClusteredColors(symbolImg, 10, colorCandidates);

            ManaColor closestColorRGB = getClosestManaColorRGB(colorCandidates);
            ManaColor closestColorHue = getClosestManaColorHUE(colorCandidates);

            std::cout << "DEBUG: Cascade Color "
                << candidates[c].color()
                << ", rgb color: " << closestColorRGB
                << ", hue color " << closestColorHue
                << " pos " << candidates[c].x() << ", "
                << candidates[c].y() << std::endl;

        }

        //draw candidates
        for (int c = 0; c < candidates.size(); ++c)
        {
            cv::rectangle(
                subImg,
                candidates[c].rect(),
                getManaColor(candidates[c].color()),
                3,
                8,
                0);
        }

        return "";


    }

    //------------------------------------------------------------------------------

    cv::Rect ManaCostIdentifier::getManaRoI(cv::Mat img)
    {
        int x = int(img.cols*0.5);
        int y = 0;
        int w = img.cols - x;
        int h = int(img.rows*0.15) - y;

        cv::Rect manaRoI = cv::Rect(x, y, w, h);
        return manaRoI;
    }

    //------------------------------------------------------------------------------

    void ManaCostIdentifier::getManaSymbolCandidates(
        cv::Mat img, ManaSymbolCandidates& candidates)
    {
        double minSizeFactor =
            MANA_SYMBOL_MIN_WIDTH_SIZE_RATIO / MANA_SYMBOL_IMG_ROI_WIDTH;
        double maxSizeFactor =
            MANA_SYMBOL_MAX_WIDTH_SIZE_RATIO / MANA_SYMBOL_IMG_ROI_WIDTH;

        cv::Size minDetectSize(
            int(img.cols*minSizeFactor),
            int(img.cols*minSizeFactor));
        cv::Size maxDetectSize(
            int(img.cols*maxSizeFactor),
            int(img.cols*maxSizeFactor));

        cv::Mat frameGray;

        cv::cvtColor(img, frameGray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frameGray, frameGray);

        for (int i = 0; i < MANA_SIZE; ++i)
        {
            if (!m_cascadeMana[i].empty())
            {
                std::vector<cv::Rect> symbols;

                m_cascadeMana[i].detectMultiScale(
                    frameGray, symbols, 1.02, 1, 0, minDetectSize, maxDetectSize);

                for (unsigned int s = 0; s < symbols.size() && s < 10; ++s)
                {
                    candidates.push_back(
                        ManaSymbolCandidate(symbols[s], ManaColor(i)));
                }
            }
        }
    }

    //------------------------------------------------------------------------------

    void ManaCostIdentifier::getClusteredColors(
        cv::Mat img,
        int colorCount,
        cv::Mat& colors)
    {
        cv::Mat centers;

        cv::Mat colVector = img.reshape(3, img.rows*img.cols);
        cv::Mat colVectorF, bestLabels;
        colVector.convertTo(colVectorF, CV_32FC3, 1.0 / 255.0);

        double compactness = cv::kmeans(colVectorF, colorCount, bestLabels,
            cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
            10, cv::KMEANS_PP_CENTERS, centers);

        colors = centers.reshape(3, centers.rows);

        //debug
        for (int x = 0; x < colVector.rows; ++x)
        {
            colVectorF.at<cv::Vec3f>(x).val[0] = colors.at<cv::Vec3f>(bestLabels.at<int>(x)).val[0];
            colVectorF.at<cv::Vec3f>(x).val[1] = colors.at<cv::Vec3f>(bestLabels.at<int>(x)).val[1];
            colVectorF.at<cv::Vec3f>(x).val[2] = colors.at<cv::Vec3f>(bestLabels.at<int>(x)).val[2];
        }

        cv::Mat clusteredImg = colVectorF.reshape(3, img.rows);
    }

    //------------------------------------------------------------------------------

    ManaCostIdentifier::ManaColor ManaCostIdentifier::getClosestManaColorRGB(
        cv::Mat colorCandidates)
    {
        std::vector<double> distances;

        for (int mc = 0; mc <= MANA_GREEN; ++mc)
        {
            double minDist = std::sqrt(3.0);

            cv::Vec3f manaColorVec;
            cv::Scalar manaColor = getManaColor(ManaColor(mc));
            manaColorVec.val[0] = float(manaColor[0] / 255.0);
            manaColorVec.val[1] = float(manaColor[1] / 255.0);
            manaColorVec.val[2] = float(manaColor[2] / 255.0);

            for (int i = 0; i < colorCandidates.rows; ++i)
            {
                cv::Vec3f colorCandidate = colorCandidates.at<cv::Vec3f>(i);
                double distance = cv::norm(manaColorVec - colorCandidate);

                if (distance < minDist)
                {
                    minDist = distance;
                }
            }

            distances.push_back(minDist);
        }

        long long minPos = std::distance(
            distances.begin(),
            std::min_element(distances.begin(), distances.end()));

        return ManaColor(minPos);

    }

    //------------------------------------------------------------------------------

    ManaCostIdentifier::ManaColor ManaCostIdentifier::getClosestManaColorHUE(
        cv::Mat colorCandidates)
    {
        std::vector<double> distances;

        for (int mc = 0; mc <= MANA_GREEN; ++mc)
        {
            double minHueDistance = 180.0;

            cv::Scalar manaColor = getManaColor(ManaColor(mc));

            const double rRef = manaColor[2] / 255.0;
            const double gRef = manaColor[1] / 255.0;
            const double bRef = manaColor[0] / 255.0;

            double hRef;
            double sRef;
            double vRef;

            rgb2hsv(rRef, gRef, bRef, hRef, sRef, vRef);

            for (int i = 0; i < colorCandidates.rows; ++i)
            {
                double r = colorCandidates.at<cv::Vec3f>(i).val[2];
                double g = colorCandidates.at<cv::Vec3f>(i).val[1];
                double b = colorCandidates.at<cv::Vec3f>(i).val[0];
                double h;
                double s;
                double v;

                if (rgb2hsv(r, g, b, h, s, v) != -1)
                {
                    if (s > 0.07 && v > 0.2)
                    {
                        double hueDistance = std::max(h, hRef) - std::min(h, hRef);
                        hueDistance = std::min(hueDistance, 360 - hueDistance);

                        if (hueDistance < minHueDistance)
                        {
                            minHueDistance = hueDistance;
                        }
                    }
                }
                else
                {
                    std::cout << "Hue calc failed for color candidates " << h << " " << s << " " << v << std::endl;
                }

            }

            distances.push_back(minHueDistance);
        }

        long long minPosHue =
            std::distance(
                distances.begin(),
                std::min_element(distances.begin(), distances.end()));

        return ManaColor(minPosHue);
    }



    //------------------------------------------------------------------------------

    const std::string ManaCostIdentifier::getCascadeFileName(ManaColor cm)
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
            return std::string();
        }
    }

    //------------------------------------------------------------------------------

    cv::Scalar ManaCostIdentifier::getManaColor(ManaColor mc)
    {
        //BGR
        switch (mc)
        {
        case MANA_WHITE:
            return cv::Scalar(174, 225, 228);
        case MANA_BLUE:
            return cv::Scalar(202, 181, 149);
        case MANA_BLACK:
            return cv::Scalar(133, 137, 142);
        case MANA_RED:
            return cv::Scalar(101, 123, 194);
        case MANA_GREEN:
            return cv::Scalar(146, 171, 137);
        case MANA_COLORLESS:
            return cv::Scalar(133, 137, 142);
        case MANA_GENERIC:
            return cv::Scalar(133, 137, 142);
        case MANA_LIFE:
            return cv::Scalar(133, 137, 142);
        default:
            return cv::Scalar(123, 123, 132);
        }
    }

    //------------------------------------------------------------------------------

    int ManaCostIdentifier::rgb2hsv(
        const double r, const double g, const double b,
        double& h, double& s, double& v)
    {
        h = -1.0;

        double min = std::min(std::min(r, g), b);
        double max = std::max(std::max(r, g), b);

        double C = max - min;
        v = max;

        if (v > 0.0001)
        {
            s = C / v;
        }
        else
        {
            s = 0.0;
        }

        if (C < 0.001)
        {
            return -1;
        }
        else if (r == max) //r is max
        {
            h = (g - b) / C;
        }
        else if (g == max) // g is max
        {
            h = 2.0 + (b - r) / C;
        }
        else if (b == max)
        {
            h = 4.0 + (r - g) / C;
        }
        else
        {
            std::cout << "Error calculating hue..." << std::endl;
            return -1;
        }

        h = h * 60;

        if (h < 0)
        {
            h += 360;
        }

        return 0;
    }

} //namespace cr

