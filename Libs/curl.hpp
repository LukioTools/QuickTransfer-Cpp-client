#include <iostream>
#include <curl/curl.h>
#include <string>
#include <utility>
#include <vector>


class GET_REQUEST{
    CURL* curl;
    CURLcode res;
    std::string URL;
    std::string readBuffer;
    std::vector<std::pair<std::string, std::string>> headers_data;

    GET_REQUEST(std::string url, std::vector<std::pair<std::string, std::string>> heads) : URL(url), headers_data(heads){
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
    }

    std::string Perform(){
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/verifyUser");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        struct curl_slist *headers = NULL;

        for (auto i : headers_data) {
            headers = curl_slist_append(headers, (i.first + ":" +  i.second).c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_global_cleanup();

        // Check for errors
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return "";
        } else {
            return readBuffer;
        }

        // Cleanup

    }
};
