/* Bruno Alexander Cremonese de Morais - Lab2 - July 15th 2020
    This program retrieves screen capabilities from /dev/fb0
    Run it as sudo or set 777 permissions on the file (not advised)

    To build:
        make lab2
    To run:
        ./lab2
*/

#include <fcntl.h> 
#include <linux/hdreg.h> 
#include <linux/fs.h> 
#include <linux/fb.h> 
#include <string.h>
#include <sys/ioctl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <iostream>

using namespace std;

//Gets fixed screen info with FBIOGET_FSCREENINFO
void getFixedScreenInfo(int fd){
    struct fb_fix_screeninfo screenInfo;
    ioctl(fd, FBIOGET_FSCREENINFO, &screenInfo);
    cout << endl << "Visual: " << screenInfo.visual 
    << " Accelerator: " << screenInfo.accel 
    << " Capabilities: " << screenInfo.capabilities << endl << endl;
}

//Gets variable screen info with FBIOGET_VSCREENINFO
void getVariableScreenInfo(int fd){
    struct fb_var_screeninfo screenInfo;
    ioctl(fd, FBIOGET_VSCREENINFO, &screenInfo);
    cout << endl << "XResolution: " << screenInfo.xres 
    << " Accelerator: " << screenInfo.yres 
    << " Capabilities: " << screenInfo.bits_per_pixel << endl << endl;
}

int main()
{
    string screenFile = "/dev/fb0";
    int fd = open(screenFile.c_str(), O_RDONLY | O_NONBLOCK);
    int optionSelected;

    if(fd < 0){
        cout << "Cannot open file..." << endl;
        return -1;
    }

    while(optionSelected != 0){
        cout << "--- Please select one of the following options ---" << endl << endl;
        cout << "1. Fixed Screen Info" << endl;
        cout << "2. Variable Screen Info" << endl;
        cout << "0. Exit" << endl;
        cin >> optionSelected;

        switch (optionSelected)
        {
        case 0:
            return 0;
            break;
        case 1:
            getFixedScreenInfo(fd);
            break;
        case 2:
            getVariableScreenInfo(fd);
            break;
        default:
            cout << "Please select an option according to the disply betweeen 0 and 2" << endl << endl;
            break;
        }
    }
}