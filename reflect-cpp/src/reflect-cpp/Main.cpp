#include <chrono>
#include <iostream>
#include <rfl/json.hpp>
#include <rfl.hpp>

struct User {
    long long userId;
    std::string username;
    bool isAdmin;
    std::optional<std::string> whatever;
};

struct X {
    long long userID;
    rfl::Skip<std::string> password;
};

int main() {
    std::cout << ".whatever == nullopt: " << rfl::json::write(
        User {
            .userId = 2,
            .username = "JoeMama69",
            .isAdmin = false,
            .whatever = std::nullopt
        }
    ) << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    auto str = rfl::json::write(
        User {
            .userId = 2,
            .username = "JoeMama69",
            .isAdmin = false,
            .whatever = "Greetings, peasants"
        }
    );
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << ".whatever != nullopt: " << str << std::endl;
    std::cout << "\n\nMeta:" << std::endl;
    std::cout << "Writing User[non-nullopt] took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start) << std::endl;
    std::cout << "\t ~" << (std::chrono::seconds(1) / (end - start)) << " writes/sec" << std::endl;
    std::cout << "\t ~" << (std::chrono::seconds(1) / (end - start)) * str.length() / 1e6 << " MB/sec" << std::endl;

    std::cout << rfl::json::write(X{ 69, "super seekrit passwørd" }) << std::endl;
    std::cout << "Password: " << (rfl::json::read<X>(R"({"userID": 69, "password": "I should not be injected"})")->password.get())
        << std::endl;
}
