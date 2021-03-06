#include "ih/ImageSourceIfc.h"

#include "ih/ImageSourceVideoFile.h"
#include "ih/ImageSourceWebcam.h"
#include "cr/MTGCardRecognizer.h"

#include "optionparser.h"

#include <thread>        
#include <chrono>        
#include <iostream>




void printVersion()
{
    std::string version = "0.1";
    std::cout << "cardrecognition " << version << std::endl;
    std::cout << "Copyright (C) 2016  Tobias Eliasson <arnestig@gmail.com> Tommy Glimberg <tommy.glimberg@gmail.com>" << std::endl;
    std::cout << "Licencse GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << std::endl;
    std::cout << "This is free software: you are free to change and redistribute it." << std::endl;
    std::cout << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
    exit(0);
}

void printUsage()
{
    std::cout << "Usage: cardrecognition [option(s)]" << std::endl;
    std::cout << " -w  --webcam=[enum]       Set the enumerated webcam to use (default 0)" << std::endl;
    std::cout << " -f  --file                Path to video file as source"<< std::endl;
    std::cout << " -o  --output              Saves every recognised card (in every detection frame) to specified output path" << std::endl;
    std::cout << " -x  --width               Set the width resolution of the image source (default native)" << std::endl;
    std::cout << " -y  --height              Set the height resolution of the image source (default native)" << std::endl;
    std::cout << " -h  --help                Display this information" << std::endl;
    std::cout << " -v  --version             Print dhcrawl version" << std::endl;
    exit(0);
}




enum  optionIndex { UNKNOWN, HELP, VERSION, WEBCAM, VIDEO_FILE, OUTPUT, WIDTH, HEIGHT };
const option::Descriptor usage[] =
{
    { UNKNOWN,    0, "" , "",      option::Arg::None, "USAGE: cardrecognition [option(s)]\n\n"
    "Options:" },
    { HELP,       0, "h", "help",    option::Arg::None,         "  --help, -h  \tPrint usage and exit." },
    { VERSION,    0, "v", "version", option::Arg::None,        "  --version, -v  \tPrint version and exit." },
    { WEBCAM,     0, "w", "webcam",  option::Arg::Optional,    "  --webcam, -c  \tSet Webcam index (default 0)." },
    { VIDEO_FILE, 0, "f", "file",    option::Arg::Optional,    "  --file, -f  \tSet video file as image source" },
    { OUTPUT,     0, "o", "output",  option::Arg::Optional,    "  --output, -o  \tSet output directory for recognised card images (saves images for each frame)" },
    { WIDTH,      0, "x", "width",   option::Arg::Optional,    "  --width, -x  \tSet the width resolution of the image source (default native)" },
    { HEIGHT,     0, "y", "height",  option::Arg::Optional,    "  --height, -y  \tSet the height resolution of the image source (default native)" },
    { UNKNOWN,    0, "" , "",        option::Arg::None, 
        "\nExamples:\n"
        "  cardrecognition --webcam 0 -o outputfolder \n" },
    { 0,0,0,0,0,0 }
};



/** @function main */
int main(int argc, char * argv[])
{
    argc -= (argc>0); argv += (argc>0); // skip program name argv[0] if present
    option::Stats  stats(usage, argc, argv);
    std::vector<option::Option> options(stats.options_max);
    std::vector<option::Option> buffer(stats.buffer_max);
    option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

    if (parse.error())
        return 1;

    if (options[HELP]) {
        option::printUsage(std::cout, usage);
        return 0;
    }

    if (options[VERSION] ) {
        printVersion();
        return 0;
    }

    for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
        std::cout << "Unknown option: " << std::string(opt->name, opt->namelen) << "\n";

    for (int i = 0; i < parse.nonOptionsCount(); ++i)
        std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";




    ih::ImageSourceIfc* imgSrc;

    if (options[VIDEO_FILE])
    {
        imgSrc = new ih::ImageSourceVideoFile(std::string(options[VIDEO_FILE].arg));
    }
    else if(options[WEBCAM])
    {
        imgSrc = new ih::ImageSourceWebcam(atoi(options[WEBCAM].arg));
    }
    else
    {
        imgSrc = new ih::ImageSourceWebcam();
    }


    cr::MTGCardRecognizer* mtgCR;

    if (options[OUTPUT])
    {
        mtgCR = new cr::MTGCardRecognizer(cr::CR_SAVE_CARDS);
        mtgCR->setOutputPath(std::string(options[OUTPUT].arg));
    }
    else
    {
        mtgCR = new cr::MTGCardRecognizer();
    }
    
    imgSrc->subscribe(dynamic_cast<ih::ImageSubscriberIfc*>(mtgCR));
     

    imgSrc->start();

    if (options[WIDTH] && options[HEIGHT])
    {
        imgSrc->setResolution(
            atoi(options[WIDTH].arg),
            atoi(options[HEIGHT].arg));
    }
    else if(options[WIDTH])
    {
        int width = atoi(options[WIDTH].arg);
        int height = width * 9 / 16;

        imgSrc->setResolution(width, height);
    }
    else if (options[HEIGHT])
    {
        int height = atoi(options[HEIGHT].arg);
        int width = height * 16 / 9;

        imgSrc->setResolution(width, height);
    }


    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
