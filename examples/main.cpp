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
    try
    {
        constexpr std::array<int, 2> signals{SIGINT, SIGTERM};
        for (auto signal : signals)
        {
            if (std::signal(signal, signalHandler) == SIG_ERR)
            {
                std::string what{"installing "};
                what += strsignal(signal);
                what += " signal handler";
                throw std::system_error(errno, std::system_category(), what);
            }
        }
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
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }
    return 0;
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