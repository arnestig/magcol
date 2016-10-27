// ClassifierTester.cpp : Defines the entry point for the console application.
//

#include "ih/ImageSourceIfc.h"

#include "ih/ImageSourceVideoFile.h"
#include "ih/ImageSourceWebcam.h"
#include "cr/MTGCardRecognizer.h"


#include <thread>        
#include <chrono>        

/** @function main */
int main(int argc, char ** argv)
{
    ih::ImageSourceIfc* imgSrc;

    if (argc == 2)
    {
        imgSrc = new ih::ImageSourceVideoFile(std::string(argv[1]));
    }
    else
    {
        imgSrc = new ih::ImageSourceWebcam();
    }

    cr::MTGCardRecognizer* mtgCR = new cr::MTGCardRecognizer();

    imgSrc->subscribe(dynamic_cast<ih::ImageSubscriberIfc*>(mtgCR));

    imgSrc->start();


    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
