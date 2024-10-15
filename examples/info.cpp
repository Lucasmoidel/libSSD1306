//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Andrew Duncan
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

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
void showUsers(SSD1306::OledI2C& oled);
void showTime(SSD1306::OledI2C& oled);
void showAddresses(SSD1306::OledI2C& oled);
static void signalHandler(int signalNumber);

int main(){
    constexpr std::array<int, 2> signals{SIGINT, SIGTERM};
    for (auto signal : signals)
    {
        SSD1306::OledI2C oled{"/dev/i2c-1", 0x3C};
        while (run)
        {
            oled.clear();
            showTime(oled);
            showAddresses(oled);
            showUsers(oled);
            oled.displayUpdate();
            constexpr auto oneSecond(std::chrono::seconds(1));
            std::this_thread::sleep_for(oneSecond);
        }

        oled.clear();
        oled.displayUpdate();
    }
    return 0;
}

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
        j = "Y";
    }
    drawString8x16(SSD1306::OledPoint{0, 32}, "L: " + l, SSD1306::PixelStyle::Set, oled);
    drawString8x16(SSD1306::OledPoint{0, 46}, "J: " + j, SSD1306::PixelStyle::Set, oled);
}

void showTime(SSD1306::OledI2C& oled) {
    time_t now;
    time(&now);
    struct tm* tm = localtime(&now);
    char time[12];
    strftime(time, sizeof(time), "%l:%M:%S %P", tm);
    SSD1306::OledPoint location{0, 0};
    drawString8x16(location, time, SSD1306::PixelStyle::Set, oled);
}

void showAddresses(SSD1306::OledI2C& oled){
    struct ifaddrs *ifAddrStruct = NULL;
    getifaddrs(&ifAddrStruct);
    for (struct ifaddrs* ifa = ifAddrStruct; ifa != NULL ; ifa = ifa->ifa_next) {
        struct sockaddr* ifa_addr = ifa->ifa_addr;
        if (ifa_addr->sa_family == AF_INET)
        {
            // is a valid IP4 Address
            void *tmpAddrPtr = &((struct sockaddr_in *)ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            std::string address;
            if (strncmp(ifa->ifa_name, "wlan", 4) == 0)
            {
                address = "W";
            }
            address += addressBuffer;
            SSD1306::OledPoint location{0, 16};
            drawString8x16(location, address, SSD1306::PixelStyle::Set, oled);
        }
    }
    if (ifAddrStruct != NULL)
    {
        freeifaddrs(ifAddrStruct);
    }
}

static void signalHandler(int signalNumber){
    switch (signalNumber)
    {
    case SIGINT:
    case SIGTERM:

        run = 0;
        break;
    };
}
