#include "ff.h"

class FileManager {
public:
    FileManager();
    void initSDCard();  // Add this method
    void readFromSDCard(char* filename, char* buffer, int bufferSize);  // Add this method
    void writeToSDCard(char* filename, char* buffer, int bufferSize);  // Add this method

private:
    FATFS fs;
};