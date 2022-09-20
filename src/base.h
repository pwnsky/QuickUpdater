#ifndef BASE_H
#define BASE_H
#include <QString>

enum class LocalCmd {
    exit,
    show_main_window,
};

#define RESOURCE_URL_BASE "http://pd.pwnsky.com:4000"
#define RESOURCE_URL_VERSION RESOURCE_URL_BASE "/version"


#define WWW_URL "http://pd.pwnsky.com"
#define WWW_DOWNLOAD_URL "http://pd.pwnsky.com/#download"
#define API_SERVER_URL_BASE "https://pd.pwnsky.com"
#define API_URL_LOG_ADD API_SERVER_URL_BASE "/pub/windows/log/add"


//#define SERVER_URL "http://127.0.0.1:8080/"
//#define DATA_DIR "./data/"
extern QString WorkspaceDir; // ~/tflash/data
#endif // BASE_H
