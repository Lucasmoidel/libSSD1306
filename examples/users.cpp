#include <array>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <thread>
#include <system_error>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "OledFont8x16.h"
#include "OledI2C.h"
#include <ifaddrs.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sstream>  
#include <fstream>

namespace { volatile static std::sig_atomic_t run = 1;}

void showUsers(SSD1306::OledI2C& oled) {
    std::string l = "N";
    std::string j = "N";
    std::system("users | tr \" \" \"\n\" | uniq | tr \"\n\" \" \" > text.txt");
    std::ifstream File("text.txt");
    std::string text;
    std::getline (File, text);
    File.close();
    if (text == "lucas ") {
        l = "Y";
    } else if (text == "josh ") {

        j = "Y";
    } else if (text == "lucas josh ") {
        l = "Y";
        j = "Y";
    } else if (text == "josh lucas ") {
        l = "Y";
        j = "Y";
    }
    drawString8x16(SSD1306::OledPoint{0, 32}, "L: " + l, SSD1306::PixelStyle::Set, oled);
    drawString8x16(SSD1306::OledPoint{0, 46}, "J: " + j, SSD1306::PixelStyle::Set, oled);
}