#include "savekey.h"

// Callback function to write the response from the server
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    // Calculate the real size of the data
    size_t realsize = size * nmemb;
    // Print the server response
    printf("Server Response: %s\n", (char*)contents);
    if (userp != NULL) {
        // Print the user data
        printf("User data: %s\n", (char*)userp);
    } else {
        printf("No user data provided.\n");
    }
    return realsize;
}

void savePrivateKeyToServer(const unsigned char* privateKey, const char* serverURL) {
    // Initialise libcurl
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    // Check if libcurl was initialised successfully
    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, serverURL);
        // Set the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, privateKey);
        // Set the callback function to handle the server response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        // Perform the HTTP request
        res = curl_easy_perform(curl);
        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        // Cleanup
        curl_easy_cleanup(curl);
    }
    // Cleanup libcurl
    curl_global_cleanup();
}

