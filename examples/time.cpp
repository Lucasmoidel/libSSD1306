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

void showTime(SSD1306::OledI2C& oled) {
    time_t now;
    time(&now);
    struct tm* tm = localtime(&now);
    char time[12];
    strftime(time, sizeof(time), "%l:%M:%S %P", tm);
    SSD1306::OledPoint location{0, 0};
    drawString8x16(location, time, SSD1306::PixelStyle::Set, oled);
}