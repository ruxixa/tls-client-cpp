/**
 * This file is a part of tls-client implementation for
 * modern C++ (17+ standard)
 *
 * Thanks for bogdanfinn for creating the original tls-client
 * library in GO https://github.com/bogdanfinn/tls-client
 */
#pragma once

//
// Determine the platform and find the DLL name depending on it
//
#if defined(_WIN32)
#define OS_WIN
#if defined(_WIN64)
    #define DLL_NAME "tls-client-windows-64-1.7.5.dll"
#else
    #define DLL_NAME "tls-client-windows-32-1.7.5.dll"
#endif
#elif defined(__linux__)
#define OS_LINUX
#if defined(__x86_64__)
    #define DLL_NAME "tls-client-xgo-1.7.5-linux-amd64.so"
#elif defined(__aarch64__)
    #define DLL_NAME "tls-client-xgo-1.7.5-linux-arm64.so"
#elif defined(__i386__)
    #define DLL_NAME "tls-client-xgo-1.7.5-linux-386.so"
#else
    #error "Unsupported Linux architecture"
#endif
#elif defined(__APPLE__)
#define OS_APPLE
#if defined(__x86_64__)
    #define DLL_NAME "tls-client-xgo-1.7.5-darwin-amd64.dyli"
#elif defined(__aarch64__)
    #define DLL_NAME "tls-client-xgo-1.7.5-darwin-arm64.dylib"
#else
    #error "Unsupported macOS architecture"
#endif
#else
    #error "Unsupported operating system"
#endif

//
// Check if the C++ standard is higher than 17 
//
#if __cplusplus == 201703L
    #pragma message("__cplusplus: 201703L (C++17)")
#elif __cplusplus == 202002L
    #pragma message("__cplusplus: 202002L (C++20)")
#else
    #pragma error("Unsupported C++ standard detected")
#endif

 /**
  * @brief LOAD_LIBRARY macro
  *
  * This macro is used to load a shared library
  * (DLL on Windows or .so on Linux/macOS) and retrieve function
  * pointers for specific functions (`request` and `freeMemory`).
  *
  * @param hLib A smart pointer to hold the handle to the loaded library.
  * @param lib_path The file path of the library to be loaded.
  *
  * @throws std::runtime_error if the library fails to load.
  *
  * Usage example:
  * @code
  * std::shared_ptr<void> hLib;
  * LOAD_LIBRARY(hLib, "path/to/library");
  * @endcode
  */
#if defined(OS_WIN)
#include <Windows.h>

#define LOAD_LIBRARY(hLib, lib_path)                                                                                   \
    hLib = std::shared_ptr<void>(LoadLibrary(lib_path.c_str()), &FreeLibrary);                                         \
    if (!hLib) {                                                                                                       \
        throw std::runtime_error("Failed to load library: " + lib_path);                                               \
    }                                                                                                                  \
    request = reinterpret_cast<RequestFunc>(GetProcAddress(static_cast<HMODULE>(hLib.get()), "request"));              \
    freeMemory = reinterpret_cast<FreeMemoryFunc>(GetProcAddress(static_cast<HMODULE>(hLib.get()), "freeMemory"));

#elif defined(OS_LINUX) || defined(OS_APPLE)
#include <dlfcn.h>

#define LOAD_LIBRARY(hLib, lib_path)                                                                                   \
    hLib = std::shared_ptr<void>(dlopen(lib_path.c_str(), RTLD_LAZY), &dlclose);                                       \
    if (!hLib) {                                                                                                       \
        throw std::runtime_error("Failed to load library: " + lib_path + " " + dlerror());                             \
    }                                                                                                                  \
    request = reinterpret_cast<RequestFunc>(dlsym(hLib.get(), "request"));                                             \
    freeMemory = reinterpret_cast<FreeMemoryFunc>(dlsym(hLib.get(), "freeMemory"));
#endif

/**
 * @brief CHECK_INITIALIZED macro
 *
 * This macro is used to check if a variable is initialized.
 * If the variable is not initialized, it throws a `std::logic_error`.
 *
 * @param variable The variable to be checked for initialization.
 * @throws std::logic_error if the variable is not initialized.
 */
#define CHECK_INITIALIZED(variable)                                                                                    \
    if (!(variable)) {                                                                                                 \
        throw std::logic_error("Variable " #variable " is not initialized.");                                          \
    }

/**
 * @brief JSON_VALUE macro
 *
 * This macro simplifies accessing JSON values using a helper function `JsonHelper::jsonValue`.
 * It converts the provided value into a JSON value using the helper function.
 *
 * @param value The value to be converted into a JSON value.
 * @return The JSON value corresponding to the input value.
 */
#define JSON_VALUE(value) JsonHelper::jsonValue(value)

#include <any>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

/**
 * @brief clientIdentifiers vector
 *
 * This vector contains identifiers for various client applications.
 * These identifiers are used to uniquely identify different clients.
 */
std::vector<std::string> clientIdentifiers = {
    // Chrome
    "chrome_103",
    "chrome_104",
    "chrome_105",
    "chrome_106",
    "chrome_107",
    "chrome_108",
    "chrome_109",
    "chrome_110",
    "chrome_111",
    "chrome_112",
    "chrome_116_PSK",
    "chrome_116_PSK_PQ",
    "chrome_117",
    "chrome_120",
    // Safari
    "safari_15_6_1",
    "safari_16_0",
    // iOS(Safari)
    "safari_ios_15_5",
    "safari_ios_15_6",
    "safari_ios_16_0",
    // iPadOS(Safari)
    "safari_ios_15_6",
    // FireFox
    "firefox_102",
    "firefox_104",
    "firefox_105",
    "firefox_106",
    "firefox_108",
    "firefox_110",
    "firefox_117",
    "firefox_120",
    // Opera
    "opera_89",
    "opera_90",
    "opera_91",
    // OkHttp4
    "okhttp4_android_7",
    "okhttp4_android_8",
    "okhttp4_android_9",
    "okhttp4_android_10",
    "okhttp4_android_11",
    "okhttp4_android_12",
    "okhttp4_android_13",
    // Custom
    "zalando_ios_mobile",
    "zalando_android_mobile",
    "nike_ios_mobile",
    "nike_android_mobile",
    "mms_ios",
    "mms_ios_2",
    "mms_ios_3",
    "mesh_ios",
    "mesh_ios_2",
    "mesh_android",
    "mesh_android_2",
    "confirmed_ios",
    "confirmed_android",
    "confirmed_android_2"
};

/**
 * @brief SessionData struct containing tls session information
 *
 * This struct encapsulates various configuration options related to a session.
 * It includes client identifier, connection flow, TLS settings, debugging options,
 * and other optional configuration parameters.
 */
struct SessionData {
    /**
     * @brief clientIdentifier field
     * 
     * This field specifies the client software and version being emulated
     * during the TLS session. It helps in mimicking the behavior and
     * characteristics of different browsers or client applications.
     * 
     * Example client identifiers:
     * - chrome_120    (chrome) *DEFAULT*
     * - chrome_103    (chrome)
     * - chrome_112    (chrome)
     * - safari_15_6_1 (safari)
     * - firefox_105   (firefox)
     * - opera_89      (opera)
     *
     * @note Check the @ref clientIdentifiers vector for full list of all avaiable
     * client identifiers
     */
    std::string clientIdentifier = "chrome_120";

    /**
     * @brief connectionFlow field
     *
     * This optional field specifies a unique identifier for the connection
     * flow. It can be used to track and manage the sequence of operations
     * during the TLS session.
     * 
     * Example: 15663105
     */
    std::optional<int> connectionFlow;
    
    /**
     * @brief randomTlsExtensionOrder field
     * 
     * Specifies whether TLS extensions should be randomized in order during communication.
     * When set to true, TLS extensions are randomized; when false, they are used in their default order.
     */
    bool randomTlsExtensionOrder = false;

    /**
     * @brief forceHttp1 field
     * 
     * Specifies whether to force the use of HTTP/1.x protocol instead of HTTP/2 or later versions.
     * When set to true, HTTP/1.x is forced; when false, other supported protocols may be used.
     */
    bool forceHttp1 = false;

    /**
     * @brief debug field
     * 
     * Specifies whether debug mode is enabled.
     * When set to true, debug mode is enabled; when false, it is disabled.
     */
    bool debug = false;

    /**
     * @brief catchPanics field
     * 
     * Specifies whether to catch and handle panics (fatal errors) gracefully.
     * When set to true, panics are caught and handled; when false, they may cause the application to crash.
     */
    bool catchPanics = false;

    /**
     * @brief ja3String field
     * 
     * This optional field is a JA3 string which uniquely identifies the client
     * fingerprint in a TLS handshake. The string format includes:
     * - TLSVersion: The version of the TLS protocol being used.
     * - Ciphers: A list of cipher suites supported by the client.
     * - Extensions: A list of TLS extensions supported by the client.
     * - EllipticCurves: A list of elliptic curves supported by the client.
     * - EllipticCurvePointFormats: The formats for elliptic curve points supported by the client.
     *
     * ja3 string format: <TLSVersion>,<Ciphers>,<sslen>,<Extensions>,<EllipticCurves>,<EllipticCurvePointFormats>
     *
     * Example: 771,4865-4866-4867-49195-49199-49196-49200-52393-52392-49171-49172-156-157-47
     * -53,0-23-65281-10-11-35-16-5-13-18-51-45-43-27-17513,29-23-24,0
     */
    std::optional<std::string> ja3String;

    /**
     * @brief h2Settings field
     * 
     * This field contains settings for HTTP/2 connections, allowing the configuration of various 
     * parameters that control the behavior of the HTTP/2 protocol. These settings are sent by both 
     * the client and the server to communicate their preferences and capabilities.
     * 
     * Possible Settings:
     * - HEADER_TABLE_SIZE: The size of the header compression table used to decode header blocks.
     * - SETTINGS_ENABLE_PUSH: A boolean indicating whether server push is enabled (1) or disabled (0).
     * - MAX_CONCURRENT_STREAMS: The maximum number of concurrent streams the sender is willing to accept.
     * - INITIAL_WINDOW_SIZE: The initial window size in bytes for stream-level flow control.
     * - MAX_FRAME_SIZE: The maximum size of a frame the sender is willing to receive.
     * - MAX_HEADER_LIST_SIZE: The maximum size of header list that the sender is prepared to accept.
     * 
     * Example:
     * {
     *     "HEADER_TABLE_SIZE": 65536,
     *     "MAX_CONCURRENT_STREAMS": 1000,
     *     "INITIAL_WINDOW_SIZE": 6291456,
     *     "MAX_HEADER_LIST_SIZE": 262144
     * }
     */
    std::optional<std::string> h2Settings;

    /**
     * @brief h2SettingsOrder field
     * 
     * This optional field specifies the order of HTTP/2 header frame settings. These settings control 
     * various parameters for HTTP/2 connections, influencing behaviors such as header table size, 
     * maximum concurrent streams, initial window size, and maximum header list size.
     * 
     * The order of settings:
     * - "HEADER_TABLE_SIZE": Size of the header compression table.
     * - "MAX_CONCURRENT_STREAMS": Maximum number of concurrent streams.
     * - "INITIAL_WINDOW_SIZE": Initial window size for flow control.
     * - "MAX_HEADER_LIST_SIZE": Maximum size of header list.
     * 
     * Example:
     * [
     *     "HEADER_TABLE_SIZE",
     *     "MAX_CONCURRENT_STREAMS",
     *     "INITIAL_WINDOW_SIZE",
     *     "MAX_HEADER_LIST_SIZE"
     * ]
     */
    std::optional<std::string> h2SettingsOrder;

    /**
     * @brief supportedSignatureAlgorithms field
     * 
     * This optional field specifies the supported signature algorithms for cryptographic operations. 
     * It allows the configuration of various algorithms that can be used for creating and verifying 
     * digital signatures in secure communications.
     * 
     * Possible Settings:
     * - PKCS1WithSHA256: RSA signature with PKCS #1 and SHA-256.
     * - PKCS1WithSHA384: RSA signature with PKCS #1 and SHA-384.
     * - PKCS1WithSHA512: RSA signature with PKCS #1 and SHA-512.
     * - PSSWithSHA256: RSA-PSS signature with SHA-256.
     * - PSSWithSHA384: RSA-PSS signature with SHA-384.
     * - PSSWithSHA512: RSA-PSS signature with SHA-512.
     * - ECDSAWithP256AndSHA256: ECDSA signature with P-256 curve and SHA-256.
     * - ECDSAWithP384AndSHA384: ECDSA signature with P-384 curve and SHA-384.
     * - ECDSAWithP521AndSHA512: ECDSA signature with P-521 curve and SHA-512.
     * - PKCS1WithSHA1: RSA signature with PKCS #1 and SHA-1 (deprecated and less secure).
     * - ECDSAWithSHA1: ECDSA signature with SHA-1 (deprecated and less secure).
     * 
     * Example:
     * [
     *     "ECDSAWithP256AndSHA256",
     *     "PSSWithSHA256",
     *     "PKCS1WithSHA256",
     *     "ECDSAWithP384AndSHA384",
     *     "PSSWithSHA384",
     *     "PKCS1WithSHA384",
     *     "PSSWithSHA512",
     *     "PKCS1WithSHA512"
     * ]
     */
    std::optional<std::string> supportedSignatureAlgorithms; 

    /**
     * @brief supportedVersions field
     * 
     * This optional field specifies the supported protocol versions for secure communications. 
     * It allows the configuration of various versions that can be used for establishing secure 
     * connections.
     * 
     * Possible Settings:
     * - GREASE: A mechanism to ensure future protocol versions can be gracefully handled.
     * - 1.3: TLS version 1.3, the latest version with improved security and performance.
     * - 1.2: TLS version 1.2, widely used and supports modern cryptographic algorithms.
     * - 1.1: TLS version 1.1, older version with fewer security features (deprecated).
     * - 1.0: TLS version 1.0, the initial version with basic security features (deprecated).
     * 
     * Example:
     * [
     *     "GREASE",
     *     "1.3",
     *     "1.2"
     * ]
     */
    std::optional<std::string> supportedVersions;

    /**
     * @brief keyShareCurves field
     * 
     * This optional field specifies the supported elliptic curves for key exchange in secure 
     * communications. It allows the configuration of various curves that can be used for 
     * establishing shared secrets in secure connections.
     * 
     * Possible Settings:
     * - GREASE: A mechanism to ensure future elliptic curves can be gracefully handled.
     * - P256: NIST P-256 elliptic curve.
     * - P384: NIST P-384 elliptic curve.
     * - P521: NIST P-521 elliptic curve.
     * - X25519: Curve25519 for use with Elliptic-curve Diffie–Hellman (ECDH) key exchange.
     * 
     * Example:
     * [
     *     "GREASE",
     *     "X25519"
     * ]
     */
    std::optional<std::string> keyShareCurves;

    /**
     * @brief certCompressionAlgo field
     * 
     * This optional field specifies the supported certificate compression algorithms for secure 
     * communications. It allows the configuration of various algorithms that can be used to 
     * compress certificates, reducing the size of the data transmitted during the handshake.
     * 
     * Examples:
     * - "zlib": A widely-used compression algorithm providing a good balance between speed and compression ratio.
     * - "brotli": A modern compression algorithm that provides higher compression ratios, especially for text data.
     * - "zstd": A fast compression algorithm with a very high compression ratio, suitable for real-time compression.
     */
    std::optional<std::string> certCompressionAlgo;

    /**
     * @brief pseudoHeaderOrder field
     * 
     * This optional field specifies the order of pseudo headers used in HTTP/2 requests. Pseudo headers 
     * are headers that begin with a colon (:) and convey essential metadata about the HTTP request or 
     * response.
     * 
     * Pseudo Header Order:
     * - ":authority": The authority part of the request URI.
     * - ":method": The HTTP method (GET, POST, etc.) of the request.
     * - ":path": The path component of the request URI.
     * - ":scheme": The scheme (http, https) of the request URI.
     * 
     * Example:
     * [
     *     ":method",
     *     ":authority",
     *     ":scheme",
     *     ":path"
     * ]
     */
    std::optional<std::string> pseudoHeaderOrder;

    /**
     * @brief priorityFrames field
     * 
     * This optional field specifies the priority frames used in HTTP/2 to manage stream prioritization. 
     * Each frame contains information about a stream's priority parameters such as weight, dependency, 
     * and exclusivity.
     * 
     * Each priority frame specifies:
     * - streamID: The identifier of the stream for which the priority parameters are defined.
     * - priorityParam: Object containing the priority parameters:
     *   - weight: The weight of the stream in relation to its peers.
     *   - streamDep: The dependency stream ID (0 indicates no dependency).
     *   - exclusive: Indicates if the dependency is exclusive (true) or not (false or 0).
     * 
     * Example:
     * [
     *     {
     *         "streamID": 3,
     *         "priorityParam": {
     *             "weight": 201,
     *             "streamDep": 0,
     *             "exclusive": false
     *         }
     *     },
     *     {
     *         "streamID": 5,
     *         "priorityParam": {
     *             "weight": 101,
     *             "streamDep": false,
     *             "exclusive": 0
     *         }
     *     }
     * ]
     */
    std::optional<std::string> priorityFrames;

    /**
     * @brief headerOrder field
     * 
     * This optional field specifies the order of headers used in a communication protocol or message. 
     * It defines the sequence in which headers should appear or be processed.
     * 
     * Example:
     * [
     *     "key1",
     *     "key2"
     * ]
     * 
     * The order of headers:
     * - "key1": The first header key in the sequence.
     * - "key2": The second header key in the sequence.
     * 
     * Note: The actual content or type of headers can vary depending on the specific protocol or message format.
     */
    std::optional<std::string> headerOrder;
};

/**
 * @brief RequestData struct containing HTTP request data
 *
 * This struct encapsulates various configuration options related to an HTTP request.
 * It includes URL, redirection behavior, TLS verification settings, timeouts,
 * proxy configuration, headers, cookies, and optional JSON payload.
 */
struct RequestData {
    /**
     * @brief url field
     *
     * This field specifies the URL of the HTTP request.
     *
     * Example: "https://example.com/api"
     */
    std::string url;

    /**
     * @brief allowRedirects field
     *
     * Specifies whether to allow HTTP redirects.
     * When set to true, redirects are allowed; when false, they are not.
     */
    bool allowRedirects = false;

    /**
     * @brief insecureSkipVerify field
     *
     * Specifies whether to skip TLS certificate verification.
     * When set to true, certificate verification is skipped; when false, it is enabled.
     */
    bool insecureSkipVerify = false;

    /**
     * @brief timeoutSeconds field
     *
     * This optional field specifies the timeout duration in seconds for the HTTP request.
     *
     * Example: 30
     */
    std::optional<int> timeoutSeconds;

    /**
     * @brief proxy field
     *
     * This optional field specifies the proxy server to be used for the HTTP request.
     *
     * Example: "http://proxy.example.com:8080"
     */
    std::optional<std::string> proxy;

    /**
     * @brief headers field
     *
     * This optional field specifies custom headers to be included in the HTTP request.
     * Should be represented as a JSON object.
     *
     * Example: {
     *  "Authorization": "Bearer token", 
     *  "Content-Type": "application/json"
     * }
     */
    std::optional<std::string> headers;

    /**
     * @brief cookies field
     *
     * This optional field specifies cookies to be included in the HTTP request.
     * Should be represented as a JSON object.
     *
     * Example: {
     *  "cookie1": "value1",
     *  "cookie2": "value2"
     * }
     */
    std::optional<std::string> cookies;

    /**
     * @brief json field
     *
     * This optional field specifies JSON payload to be included in the HTTP request body.
     * Should be represented as a JSON string.
     *
     * Example: {"key": "value"}
     */
    std::optional<std::string> data;
};

/**
 * @brief ResponseData struct containing response information
 *
 * This struct encapsulates various details related to an HTTP response.
 * It includes status code, response body, cookies, headers, target URL,
 * and the protocol used for the response.
 */
struct ResponseData {
    /**
     * @brief statusCode field
     *
     * This field specifies the HTTP status code of the response.
     *
     * Example: 200 (OK)
     */
    int statusCode;

    /**
     * @brief body field
     *
     * This field contains the body content of the HTTP response.
     * The body can be either in HTML or JSON format.
     *
     * Example (HTML): "<html><body>Hello, World!</body></html>"
     * Example (JSON): {"key": "value"}
     */
    std::string body;

    /**
     * @brief cookies field
     *
     * This field contains cookies set in the HTTP response, represented in JSON format.
     *
     * Example: {
     *  "cookie1": "value1",
     *  "cookie2": "value2"
     * }
     */
    std::string cookies;

    /**
     * @brief headers field
     *
     * This field contains headers included in the HTTP response, represented in JSON format.
     *
     * Example: {
     *  "Content-Type": "text/html",
     *  "Server": "Apache"
     * }
     */
    std::string headers;

    /**
     * @brief target field
     *
     * This field specifies the target or final URL after any redirects.
     * It should be a valid link.
     *
     * Example: "https://example.com/final-page"
     */
    std::string target;

    /**
     * @brief usedProtocol field
     *
     * This field specifies the protocol used for the HTTP response.
     * Possible values include HTTP/1.1 and HTTP/2.
     *
     * Example: "HTTP/1.1"
     */
    std::string usedProtocol;
};

/**
 * @brief TlsClient class for performing TLS requests.
 */
class TlsClient {
public:
    /**
     * @brief Performs a TLS request with the provided input.
     *
     * @param input The input data for the request.
     * @return std::string The response from the TLS request.
     */
    static std::string performRequest(const std::string& input);

    /**
     * @brief Destructor for the TlsClient class.
     *
     * This destructor is private to prevent direct instantiation and to manage
     * the lifecycle of the TLS client within the defined class structure.
     */
     ~TlsClient();
private:
    using RequestFunc = char* (*)(const char*);   /**< Type definition for request function pointer. */
    using FreeMemoryFunc = void (*)(char*);       /**< Type definition for free memory function pointer. */

    static inline RequestFunc request;            /**< Pointer to the request function. */
    static inline FreeMemoryFunc freeMemory;      /**< Pointer to the free memory function. */
    static inline std::shared_ptr<void> hLib;     /**< Handle to the loaded library. */

    /**
     * @brief Ensures the TLS client is initialized.
     *
     * This function ensures that the necessary components of the TLS client
     * are initialized before performing any request.
     */
    static inline void ensureInitialized();
};

/**
 * @brief JsonHelper class provides utilities for JSON parsing and generation.
 */
class JsonHelper {
public:
    /**
     * @brief Parses JSON response into ResponseData structure.
     *
     * @param json The JSON string to parse.
     * @return ResponseData The parsed response data.
     */
    [[nodiscard]] static inline ResponseData parseResponse(const std::string& json);

    /**
     * @brief Builds JSON string from given data.
     *
     * @tparam Args Variadic template parameter pack for data types.
     * @param data The unordered map containing key-value pairs.
     * @return std::string The JSON string representation of the data.
     */
    template <typename... Args>
    [[nodiscard]] static inline std::string buildJson(const std::unordered_map<std::string, std::any>& data);

private:
    /**
     * @brief Converts a value to its JSON string representation.
     *
     * @tparam T Type of the value.
     * @param value The value to convert.
     * @return std::string The JSON string representation of the value.
     */
    template <typename T>
    [[nodiscard]] static inline std::string jsonValue(const T& value);

    /**
     * @brief Appends a key-value pair to the JSON string stream.
     *
     * @param oss The output string stream to append to.
     * @param key The key for the JSON object.
     * @param value The value associated with the key.
     */
    static inline void appendValue(std::ostringstream& oss, [[maybe_unused]] const std::string& key, const std::any& value);

    /**
     * @brief Tokenizes a JSON string into individual tokens.
     *
     * @param json The JSON string to tokenize.
     * @return std::vector<std::string> Vector of tokens extracted from the JSON string.
     */
    [[nodiscard]] static inline std::vector<std::string> tokenize(const std::string& json);

    /**
     * @brief Appends a key-value pair to the JSON string stream.
     *
     * This function is used for basic data types.
     *
     * @tparam T Type of the value.
     * @param oss The output string stream to append to.
     * @param key The key for the JSON object.
     * @param value The value associated with the key.
     */
    template <typename T>
    static inline void appendKeyValue(std::ostringstream& oss, const std::string& key, const T& value);

    /**
     * @brief Helper struct for always_false type trait.
     *
     * Provides a type trait that is always false.
     */
    template <typename T>
    struct always_false : std::false_type {};
};


/**
 * @brief Session class for managing HTTP session operations.
 */
class Session {
public:
    /**
     * @brief Constructor to initialize the session with provided session data.
     *
     * @param sessionData The session data to initialize the session with.
     */
    Session(SessionData sessionData) : sessionData(sessionData) {};

    /**
     * @brief Sends a GET request using the session.
     *
     * @param requestData The request data for the GET request.
     * @return ResponseData The response from the GET request.
     */
    ResponseData GET(RequestData requestData);

    /**
     * @brief Sends a POST request using the session.
     *
     * @param requestData The request data for the POST request.
     * @return ResponseData The response from the POST request.
     */
    ResponseData POST(RequestData requestData);

    /**
     * @brief Sends a PUT request using the session.
     *
     * @param requestData The request data for the PUT request.
     * @return ResponseData The response from the PUT request.
     */
    ResponseData PUT(RequestData requestData);

    /**
     * @brief Sends a DELETE request using the session.
     *
     * @param requestData The request data for the DELETE request.
     * @return ResponseData The response from the DELETE request.
     */
    ResponseData _DELETE(RequestData requestData);

    /**
     * @brief Sends a PATCH request using the session.
     *
     * @param requestData The request data for the PATCH request.
     * @return ResponseData The response from the PATCH request
     */
    ResponseData PATCH(RequestData requestData);

    /**
     * @brief Sends a HEAD request using the session.
     *
     * @param requestData The request data for the HEAD request.
     * @return ResponseData The response from the HEAD request.
     */
    ResponseData HEAD(RequestData requestData);

    /**
     * @brief Sends an OPTIONS request using the session.
     *
     * @param requestData The request data for the OPTIONS request.
     * @return ResponseData The response from the OPTIONS request.
     */
    ResponseData OPTIONS(RequestData requestData);

private:
    SessionData sessionData; /**< The session data associated with this session. */

    /**
     * @brief Performs an HTTP request with the specified method.
     *
     * @param requestData The request data for the HTTP request.
     * @param method The HTTP method to use (e.g., "POST", "GET", etc.).
     * @return ResponseData The response from the HTTP request.
     */
    [[nodiscard]] inline ResponseData performRequest(RequestData requestData, const std::string& method);

    /**
     * @brief Adds a key-value pair to the request body if the value is present.
     *
     * @tparam T Type of the value to add.
     * @param body The map representing the request body.
     * @param key The key to add to the body.
     * @param value The value to add to the body.
     */
    template <typename T>
    inline void addToBodyIfPresent(std::unordered_map<std::string, std::any>& body, const std::string& key,
        const T& value);

    /**
     * @brief Builds the request body for the HTTP request.
     *
     * @param requestData The request data for the HTTP request.
     * @param method The HTTP method being used.
     * @return std::string The constructed request body.
     */
    [[nodiscard]] inline std::string buildRequestBody(RequestData requestData, std::string method);
};

std::string TlsClient::performRequest(const std::string& input) {
    ensureInitialized();

    char* result = request(input.c_str());
    std::string response(result);
    freeMemory(result);
    return response;
}

void TlsClient::ensureInitialized() {
    static bool initialized = false;
    if (!initialized) {
        std::string root_dir = std::filesystem::current_path().string();
        std::string lib_path = root_dir + "/dependencies/" + DLL_NAME;

        LOAD_LIBRARY(hLib, lib_path);

        CHECK_INITIALIZED(request);
        CHECK_INITIALIZED(freeMemory);

        initialized = true;
    }
}

inline TlsClient::~TlsClient() { hLib.reset(); }

template <typename... Args>
std::string JsonHelper::buildJson(const std::unordered_map<std::string, std::any>& data) {
    std::ostringstream oss;
    oss << "{";

    bool first = true;

    for (auto it = data.begin(); it != data.end(); ++it) {
        if (!first) {
            oss << ", ";
        }
        first = false;

        const std::string& key = it->first;
        const std::any& value = it->second;

        appendValue(oss, key, value);
    }

    oss << "}";
    return oss.str();
}

template <typename T> inline std::string JsonHelper::jsonValue(const T& value) {
    if constexpr (std::is_same_v<T, std::string>) {
        bool isJson = std::regex_match(value, std::regex(R"((\{.*\})|(\[.*\]))"));
        if (!isJson) {
            return "\"" + value + "\"";
        }
        return value;
    }

    else if constexpr (std::is_same_v<T, bool>) {
        return value ? "true" : "false";
    }

    else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(value);
    }

    else {
        static_assert(always_false<T>::value, "Unsupported type for jsonValue");
    }
}

std::vector<std::string> JsonHelper::tokenize(const std::string& json) {                            
    int indents = 0;
    bool inString = false;

    std::vector<std::string> tokens;
    std::string token;

    for (size_t i = 0; i < json.size(); ++i) {
        char ch = json[i];

        if (ch == '"') {
            inString = !inString;
        }

        if (ch == '{') {
            indents++;
        }

        if (ch == '}') {
            if (indents != 1) {
                token += ch;
            }
            indents--;
        }

        if (isspace(ch) && !inString) {
            continue;
        }

        if (!inString && (ch == '{' || ch == '}' || ch == ':' || ch == ',')) {
            if (indents != 1) {
                token += ch;
                continue;
            }

            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, ch));
        }

        else {
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

template <typename T>
void JsonHelper::appendKeyValue(std::ostringstream& oss, const std::string& key, const T& value) {
    oss << "\"" << key << "\": " << JSON_VALUE(value);
}

void JsonHelper::appendValue(std::ostringstream& oss, const std::string& key, const std::any& value) {
    if (!value.has_value()) {
        return;
    }
    try {
        const std::type_info& ti = value.type();
        if (ti == typeid(std::string)) {
            appendKeyValue(oss, key, std::any_cast<std::string>(value));
        }
        else if (ti == typeid(int)) {
            appendKeyValue(oss, key, std::any_cast<int>(value));
        }
        else if (ti == typeid(double)) {
            appendKeyValue(oss, key, std::any_cast<double>(value));
        }
        else if (ti == typeid(bool)) {
            appendKeyValue(oss, key, std::any_cast<bool>(value));
        }
    }
    catch (const std::bad_any_cast& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

ResponseData JsonHelper::parseResponse(const std::string& json) {
    auto tokens = tokenize(json);
    ResponseData responseData;
    std::string key;
    bool isKey = true;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& token = tokens[i];

        if (token == "{" || token == "}") {
            continue;
        }
        else if (token == ":") {
            isKey = false;
        }
        else if (token == ",") {
            isKey = true;
        }
        else {
            if (isKey) {
                key = token.substr(1, token.size() - 2);
            }
            else {
                if (key == "status" && !token.empty()) {
                    responseData.statusCode = std::stoi(token);
                }
                else if (key == "body" && !token.empty()) {
                    responseData.body = token.substr(1, token.size() - 2);
                }
                else if (key == "target" && !token.empty()) {
                    responseData.target = token.substr(1, token.size() - 2);
                }
                else if (key == "usedProtocol" && !token.empty()) {
                    responseData.usedProtocol = token.substr(1, token.size() - 2);
                }
                else if (key == "headers") {
                    responseData.headers = token;
                }
                else if (key == "cookies") {
                    responseData.cookies = token;
                }
                isKey = true;
            }
        }
    }

    return responseData;
}

template <typename T>
void Session::addToBodyIfPresent(std::unordered_map<std::string, std::any>& body,
    const std::string& key, const T& value) {
    if (value.has_value()) {
        body[key] = value.value();
    }
};

std::string Session::buildRequestBody(RequestData requestData, std::string method) {
    std::unordered_map<std::string, std::any> body;

    addToBodyIfPresent(body, "h2Settings", sessionData.h2Settings);
    addToBodyIfPresent(body, "ja3String", sessionData.ja3String);
    addToBodyIfPresent(body, "h2SettingsOrder", sessionData.h2SettingsOrder);
    addToBodyIfPresent(body, "supportedSignatureAlgorithms", sessionData.supportedSignatureAlgorithms);
    addToBodyIfPresent(body, "supportedVersions", sessionData.supportedVersions);
    addToBodyIfPresent(body, "keyShareCurves", sessionData.keyShareCurves);
    addToBodyIfPresent(body, "certCompressionAlgo", sessionData.certCompressionAlgo);
    addToBodyIfPresent(body, "pseudoHeaderOrder", sessionData.pseudoHeaderOrder);
    addToBodyIfPresent(body, "connectionFlow", sessionData.connectionFlow);
    addToBodyIfPresent(body, "priorityFrames", sessionData.priorityFrames);
    addToBodyIfPresent(body, "headerOrder", sessionData.headerOrder);
    addToBodyIfPresent(body, "headers", requestData.headers);
    addToBodyIfPresent(body, "requestCookies", requestData.cookies);
    addToBodyIfPresent(body, "requestBody", requestData.data);
    addToBodyIfPresent(body, "timeoutSeconds", requestData.timeoutSeconds);
    addToBodyIfPresent(body, "proxyUrl", requestData.proxy);

    body["requestMethod"] = method;
    body["allowRedirects"] = requestData.allowRedirects;
    body["insecureSkipVerify"] = requestData.allowRedirects;
    body["requestUrl"] = requestData.url;
    body["clientIdentifier"] = sessionData.clientIdentifier;
    body["randomTlsExtensionOrder"] = sessionData.randomTlsExtensionOrder;
    body["forceHttp1"] = sessionData.forceHttp1;
    body["catchPanics"] = sessionData.catchPanics;
    body["debug"] = sessionData.debug;

    std::string jsonBody = JsonHelper::buildJson(body);
    return jsonBody;
}

ResponseData Session::performRequest(RequestData requestData, const std::string& method) {
    std::string body = buildRequestBody(requestData, method);
    std::string response = TlsClient::performRequest(body);

    ResponseData responseData = JsonHelper::parseResponse(response);
    return responseData;
}

ResponseData Session::POST(RequestData requestData) {
    return performRequest(requestData, "POST");
}

ResponseData Session::GET(RequestData requestData) {
    return performRequest(requestData, "GET");
}

ResponseData Session::PUT(RequestData requestData) {
    return performRequest(requestData, "PUT");
}

ResponseData Session::_DELETE(RequestData requestData) {
    return performRequest(requestData, "DELETE");
}

ResponseData Session::PATCH(RequestData requestData) {
    return performRequest(requestData, "PATCH");
}

ResponseData Session::HEAD(RequestData requestData) {
    return performRequest(requestData, "HEAD");
}

ResponseData Session::OPTIONS(RequestData requestData) {
    return performRequest(requestData, "OPTIONS");
}