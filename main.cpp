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
#include <wiringPi.h>

namespace { volatile static std::sig_atomic_t run = 1;}
void showUsers(SSD1306::OledI2C& oled);
void showTime(SSD1306::OledI2C& oled);
void showAddresses(SSD1306::OledI2C& oled);
static void signalHandler(int signalNumber);

int main(){
    try
    {
        wiringPiSetupGpio();
        pinMode(17, INPUT);
        pinMode(27, OUTPUT);
        digitalWrite(27, HIGH);
        pinMode(21, INPUT);
        pinMode(20, OUTPUT);
        digitalWrite(20, HIGH);
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
        int t = 10;
        bool shutting = false;
        while (run) {
            if (digitalRead(21) && !shutting) {
                std::cout << "shutting down" << std::endl;
                system("shutdown");
                shutting = true;
                t = 60;
            }
            if (digitalRead(17) && !shutting) {
                t = 10;
            }
            while (t > 0) {
                if (digitalRead(21) && !shutting) {
                    std::cout << "shutting down" << std::endl;
                    system("shutdown");
                    shutting = true;
                    t = 60;
                }
                if (digitalRead(17) && !shutting) {
                    t = 10;
                }
                if (digitalRead(17) && shutting) {
                    system("shutdown -c");
                    shutting = false;
                    std::cout << "shutdown cancled" << std::endl;
                }
                oled.clear();
                if (shutting) {
                    drawString8x16(SSD1306::OledPoint{0, 0}, "SHUTTING DOWN", SSD1306::PixelStyle::Set, oled);
                    drawString8x16(SSD1306::OledPoint{0, 16}, std::to_string(t), SSD1306::PixelStyle::Set, oled);
                } else {
                    showTime(oled);
                    showAddresses(oled);
                    showUsers(oled);
                }
                oled.displayUpdate();
                constexpr auto oneSecond(std::chrono::seconds(1));
                std::this_thread::sleep_for(oneSecond);
                t--;
            }
            oled.clear();
            oled.displayUpdate();

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