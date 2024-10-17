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
void splitString(std::string& input, std::string arr[], int& index);
void showUsers(SSD1306::OledI2C& oled) {
    std::system("users | tr \" \" \"\n\" | uniq | tr \"\n\" \" \" > text.txt");
    std::ifstream File("text.txt");
    std::string text;
    std::getline (File, text);
    File.close();
    std::string arr[5];
    int index = 0;
    splitString(text, arr, index);
    int num = 0;
    for (int i = 0; i < 5; i++) {
        if (arr[i] != "") {
            num++;
        }
    }
    for (int i = 0; i < num; i++){
        int y = 32;
        drawString8x16(SSD1306::OledPoint{0, y}, arr[i], SSD1306::PixelStyle::Set, oled);
        y+=16;
    }
}

void splitString(std::string& input, std::string arr[], int& index){
    // Creating an input string stream from the input string
    std::istringstream stream(input);

    // Temporary string to store each token
    std::string token;

    // Read tokens from the string stream separated by the
    // delimiter
    while (std::getline(stream, token, ' ')) {
        // Add the token to the array
        arr[index++] = token;
    }
}