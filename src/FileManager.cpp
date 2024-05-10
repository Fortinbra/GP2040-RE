#include "headers/FileManager.h"

#define SDIO_CMD_PIN 17
#define SDIO_D0_PIN 18

FileManager::FileManager()
	: sdio(SDIO()) // Initialize the SDIO instance
{
}

void FileManager::initSDCard()
{
	// Initialize the SDIO hardware
	sdio_init();

	// Mount the file system
	FRESULT res = f_mount(&fs, "", 1);

	// Check for errors
	if (res != FR_OK)
	{
		printf("Failed to mount the file system (res = %d)\n", res);
		return;
	}

	// The SD card is now ready to use
	printf("SD card initialized successfully\n");
}

void FileManager::readFromSDCard(char *filename, char *buffer, int bufferSize)
{
	// Read data from the SD card using the SDIO instance
}

void FileManager::writeToSDCard(char *filename, char *buffer, int bufferSize)
{
	// Write data to the SD card using the SDIO instance
}