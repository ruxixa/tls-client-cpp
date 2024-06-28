
# 🛡️ TLS client C++ 

## 📙 About 

`tls-client` is a modern C++17+ library designed for performing secure HTTPS connections with ease and flexibility. The library uses CFFI of the tls-client GO library created by bogdanfinn.

## ✨ Installation

To start using the library, follow these steps:

1. Clone the repository
```bash
git clone https://github.com/your_username/tls-client-cpp.git
cd tls-client-cpp
```

2. Copy the `include` folder to your repository
3. Include the library in yout project

```cpp
#include <include/tls_client.hpp>
```

## 💡 Example Usage
```cpp
#include "include/tls_client.hpp"

int main() {
    SessionData sessionData;
    RequestData requestData;
    ResponseData responseData;

    Session session(sessionData);
    requestData.url = "https://httpbin.org/get";
    responseData = session.GET(requestData);

    std::cout << "Status Code: " << responseData.statusCode << std::endl;
    std::cout << "Response Body: " << responseData.body << std::endl;

    return 0;
}
```

You can find more examples in the `example` directory

## 🤝 Contributing

Contributions and pull requests are welcome. Read [CONTRIBUTING.md](CONTRIBUTING.md) for more information.

## 🧾 Licensing

The code is licensed under the [MIT LICENSE](LICENSE)