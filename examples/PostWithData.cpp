/**
 * This file is a part of tls-client implementation for
 * modern C++ (17+ standard)
 *
 * Thanks for bogdanfinn for creating the original tls-client
 * library in GO https://github.com/bogdanfinn/tls-client
 */
#include "tls_client.hpp"

int main() {
    SessionData sessionData;
    RequestData requestData;
    ResponseData responseData;

    Session session(sessionData);
    requestData.url = "https://httpbin.org/post";
    requestData.data = "Hello, world!";
    responseData = session.POST(requestData);

    std::cout << "Status Code: " << responseData.statusCode << std::endl;
    std::cout << "Response Body: " << responseData.body << std::endl;

    return 0;
}
