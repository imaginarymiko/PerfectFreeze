#include <curl/curl.h>

// Callback function for writing data from a URL
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);

// Saves the private key to a server
void savePrivateKeyToServer(const unsigned char* privateKey, const char* serverURL);