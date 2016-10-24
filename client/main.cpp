// ClassifierTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ih\ImageSourceIfc.h"

#include "ih\ImageSourceVideoFile.h"
#include "ih\ImageSourceWebcam.h"
#include "cr\MTGCardRecognizer.h"


#include <thread>        
#include <chrono>        

/** @function main */
int main(int argc, char ** argv)
{
    ih::ImageSourceIfc* imgSrc;

    if (argc == 2)
    {
        imgSrc = new ih::ImageSourceVideoFile();
    }
    else
    {
        imgSrc = new ih::ImageSourceWebcam();
    }

    ih::ImageSubscriberIfc* mtgCR = new cr::MTGCardRecognizer();

    imgSrc->subscribe(mtgCR);

    imgSrc->start();
    
    //when start has become non blocking
    //while (true)
    //{
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //}

    return 0;
}
