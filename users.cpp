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
#include "OledFont8x12.h"
#include "OledFont8x8.h"
#include "OledI2C.h"
#include <ifaddrs.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sstream>  
#include <fstream>

namespace { volatile static std::sig_atomic_t run = 1;}
void splitString(std::string& input, std::string arr[]);
void showUsers(SSD1306::OledI2C& oled) {
    std::system("users | tr \" \" \"\n\" | uniq | tr \"\n\" \" \" > text.txt");
    std::ifstream File("text.txt");
    std::string text;
    std::getline (File, text);
    File.close();
    int usrammount = 20;
    std::string arr[usrammount];
    splitString(text, arr);
    int y = 32;
    for (int i = 0; i < 4; i+=2){
        drawString8x12(SSD1306::OledPoint{0, y}, arr[i] + " ", SSD1306::PixelStyle::Set, oled);
        drawString8x12(SSD1306::OledPoint{((int)arr[i].length()+1) * 8, y}, arr[i+1], SSD1306::PixelStyle::Set, oled);
        y+=12;
    }
    int count = 0;
    for (int i = 0; i < usrammount; i++) {
        if ( arr[i] != "") {
            count++;
        }
    }
    if (count > 4) {
        drawString8x8(SSD1306::OledPoint{0, 64-8}, "+" + std::to_string(count-4), SSD1306::PixelStyle::Set, oled);
    }
}

void splitString(std::string& input, std::string arr[]){
    // Creating an input string stream from the input string
    std::istringstream stream(input);

    // Temporary string to store each token
    std::string token;
    int index = 0;
    // Read tokens from the string stream separated by the
    // delimiter
    while (std::getline(stream, token, ' ')) {
        // Add the token to the array
        arr[index++] = token;
    }
}