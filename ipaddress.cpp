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
            if (strncmp(ifa->ifa_name, "eth", 3) == 0)
            {
                address = "E";
            }
            else if (strncmp(ifa->ifa_name, "wlan", 4) == 0)
            {
                address = "W";
            }
            else if (strncmp(ifa->ifa_name, "usb", 3) == 0)
            {
                address = "U";
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