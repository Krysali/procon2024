#ifndef PROCON_API_H
#define PROCON_API_H

#include <string>

std::string GetRequest(const std::string& url, const std::string& token);
std::string PostRequest(const std::string& url, const std::string& token, const std::string& jsonData);

#endif // PROCON_API_H