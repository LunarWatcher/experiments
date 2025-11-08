#include "stc/Environment.hpp"
#include <string>
#include <vector>

/**
 * This is a mock program for simulating sub-subprocess spawning
 */
int main(int argc, const char* argv[]) {
    std::cout << "mocksub has started" << std::endl;
    std::cout << "argc=" << argc << std::endl;
    if (argc == 1) {
        std::string buff;
        while (std::getline(std::cin, buff)) {
            if (buff == "EOF") {
                std::cout << "EOF received" << std::endl;
                return 0;
            }
            std::cout << buff << std::endl;
        }
    } else {
        std::vector<std::string> argvAsVec {
            argv + 1, argv + argc
        };
        if (argvAsVec[0] == "spawn") {

            int code = 0;

            stc::syscommandNoCapture(std::vector<const char*> {
                "/usr/bin/env", "bash", "-i"
            }, &code);
            // std::cout << "Terminal output: " << out << std::endl;
            std::cout << "Exit code: " << code << std::endl;
            return code;
        }
        return -2;
    }
}
