/**
 * This file is a part of tls-client implementation for
 * modern C++ (17+ standard)
 *
 * Thanks for bogdanfinn for creating the original tls-client
 * library in GO https://github.com/bogdanfinn/tls-client
 */
#include <string>
#include <gtest/gtest.h>
#include <filesystem>
#include <iostream>

#include "../include/tls_client.hpp"

class TlsClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        requestData.url = "https://httpbin.org";
        session = new Session(sessionData);
    }

    void TearDown() override {
        delete session;
    }

    RequestData requestData;
    ResponseData responseData;
    SessionData sessionData;
    Session* session;
};

// Function to list contents of a directory
void listDirectoryContents(const std::string& path) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
    }
}

// Test request methods
TEST_F(TlsClientTest, TestGETRequest) {
    requestData.url += "/get";
    responseData = session->GET(requestData);
    ASSERT_EQ(responseData.statusCode, 200);
}

TEST_F(TlsClientTest, TestPOSTRequest) {
    requestData.url += "/post";
    responseData = session->POST(requestData);
    ASSERT_EQ(responseData.statusCode, 200);
}

TEST_F(TlsClientTest, TestPUTRequest) {
    requestData.url += "/put";
    responseData = session->PUT(requestData);
    ASSERT_EQ(responseData.statusCode, 200);
}

TEST_F(TlsClientTest, TestDELETERequest) {
    requestData.url += "/delete";
    responseData = session->_DELETE(requestData);
    ASSERT_EQ(responseData.statusCode, 200);
}

TEST_F(TlsClientTest, TestPATCHRequest) {
    requestData.url += "/patch";
    responseData = session->PATCH(requestData);
    ASSERT_EQ(responseData.statusCode, 200);
}

// Test status codes
TEST_F(TlsClientTest, Test200StatusCode) {
    requestData.url += "/status/200";
    responseData = session->GET(requestData);
    ASSERT_EQ(responseData.statusCode, 200);
}

TEST_F(TlsClientTest, Test300StatusCode) {
    requestData.url += "/status/300";
    responseData = session->GET(requestData);
    ASSERT_EQ(responseData.statusCode, 300);
}

TEST_F(TlsClientTest, Test400StatusCode) {
    requestData.url += "/status/400";
    responseData = session->GET(requestData);
    ASSERT_EQ(responseData.statusCode, 400);
}

// Test request attributes
TEST_F(TlsClientTest, TestRequestHeaders) {
    requestData.url += "/anything";
    requestData.headers = R"({"authorization": "token"})";
    responseData = session->GET(requestData);

    ASSERT_TRUE(responseData.body.find(R"(\"Authorization\": \"token\")") != std::string::npos);
}

TEST_F(TlsClientTest, TestRequestData) {
    requestData.url += "/anything";
    requestData.data = "Hello, world!";

    responseData = session->GET(requestData);

    ASSERT_TRUE(responseData.body.find(R"(\"data\": \"Hello,world!\")") != std::string::npos);
}

TEST_F(TlsClientTest, TestRequestCookies) {
    requestData.url += "/anything";
    requestData.cookies = R"([{"cookie": "cookie_value"}])";

    responseData = session->GET(requestData);

    // Since httpbin is not providing us with the cookies 
    // in the response, the only thing we can do is just 
    // to check if the request was successful.
    ASSERT_EQ(responseData.statusCode, 200);
}

TEST_F(TlsClientTest, TestRequestAllowRedirects) {
    requestData.url += "/anything";
    requestData.allowRedirects = true;

    responseData = session->GET(requestData);

    // Since httpbin is not providing us with allowRedirects 
    // attribute in the response, the only thing we can do is just 
    // to check if the request was successful.
    ASSERT_EQ(responseData.statusCode, 200);
}

TEST_F(TlsClientTest, TestRequestInsecureSkipVerify) {
    requestData.url += "/anything";
    requestData.insecureSkipVerify = true;

    responseData = session->GET(requestData);

    // Since httpbin is not providing us with allowRedirects 
    // attribute in the response, the only thing we can do is just 
    // to check if the request was successful.
    ASSERT_EQ(responseData.statusCode, 200);
}

TEST_F(TlsClientTest, TestRequestProxy) {
    requestData.url += "/anything";
    requestData.proxy = "https://test_proxy:1234";
    requestData.timeoutSeconds = 10; // The request will timeout anyway

    responseData = session->GET(requestData);

    // Of course, https://test_proxy:1234 is not a valid proxy,
    // so we will check if the request failed (status code will be 0)
    ASSERT_EQ(responseData.statusCode, 0);
}

// We don't have to test url attribute, since we have already
// used it in every test

int main(int argc, char** argv) {
    // List directory contents before running tests
    std::cout << "Listing directory contents before running tests:" << std::endl;
    listDirectoryContents(".");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
