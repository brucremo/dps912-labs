// listdir.c
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cctype>
#include <fstream>
#include <regex>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[])
{
  struct dirent *de=NULL;
  DIR *d=NULL;
  std::string dirPath = "/proc";
  unsigned char isFile = 0x8;

  d=opendir(dirPath.c_str());
  if(d == NULL)
  {
    perror("Couldn't open directory");
    return(2);
  }
  
  // Loop while not NULL
  while(de = readdir(d)){
    
    if(*de->d_name == '.' || !isdigit(de->d_name[0])){
        continue;
    }

    struct dirent *ent = NULL;
    DIR *dir = NULL;

    std::string dirName(de->d_name);
    std::string processDirPath = dirPath + "/" + dirName;
    std::string statusPath = processDirPath + "/status";

    std::ifstream file;
    string line;
    file.open(statusPath.c_str());

    std::string procName;

    while(getline(file,line)){
        int namePos = line.find("Name");
        if(namePos != -1){
            procName = line;
        }

        int memPos = line.find("VmRSS");
        if(memPos != -1){
            std::string memoryUsed = std::regex_replace(
                line, std::regex("[^0-9]*([0-9]+).*"), std::string("$1")
            );
            if(std::stoi(memoryUsed) > 10000){
                std::cout << procName << std::endl << 
                "Pid: " << dirName << std::endl << 
                "Memory Used: "  << memoryUsed << " kB" << 
                std::endl << std::endl;
            }
        }
    }
    closedir(dir);
  }

  closedir(d);
  return(0);
}