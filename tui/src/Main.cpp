#include <cassert>
#include <fstream>
#include <iostream>
#include <thread>

#include <termios.h>
#include <unistd.h>

using namespace std::literals;

struct RawRAII {
    termios t;
    RawRAII() {
        tcgetattr(STDIN_FILENO, &t);

        t.c_iflag &= ~IGNBRK;
        t.c_iflag &= ~BRKINT;
        t.c_iflag &= ~PARMRK;
        t.c_iflag &= ~ISTRIP;
        t.c_iflag &= ~INLCR;
        t.c_iflag &= ~IGNCR;
        t.c_iflag &= ~ICRNL;
        t.c_iflag &= ~IXON;
        t.c_lflag &= ~ECHO;
        t.c_lflag &= ~ECHONL;
        t.c_lflag &= ~ICANON;
        t.c_lflag &= ~ISIG;
        t.c_lflag &= ~IEXTEN;
        t.c_cflag |= CS8;
        t.c_cc[VMIN] = 0;
        t.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }

    ~RawRAII() {
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }
};

void clearing() {
    std::cout << "\033[2K";
    
    // TODO: I'm missing a \r somewhere, but not sure where it's supposed to go:
    // * \r\n looks redundant, \n already resets the cursor position. Is this a thing for variable width or some shit like that?
    // * - never mind
    // \r is necessary because \n apparently doesn't move horizontally per the standard, so \r is required for the
    // terminals where \n doesn't also imply \r: https://en.wikipedia.org/wiki/ANSI_escape_code#C0_control_codes
    std::cout << "Test 1123\n";
    std::cout << "Test 2\n";
    std::cout << "Test 3\n";
    std::this_thread::sleep_for(3s);

    std::cout << "\033[2K";
    for (size_t i = 0; i < 3; ++i) {
        std::cout << "\033[1A";
        std::cout << "\033[2K";
    }
    std::cout << "Test 4\n";
    std::cout << "Test 5\n";
    std::cout << "Test 6\n";
}

void loading() {
    RawRAII rawMode;
    std::ofstream f("log.txt");
    assert(bool(f));
    for (size_t i = 10; i <= 200; ++i) {
        std::cout << i << std::flush;
        std::this_thread::sleep_for(1s);
        std::cout << "\r" << std::flush;
        std::this_thread::sleep_for(1s);
        std::cout << "\033[2K" << std::flush;
        std::this_thread::sleep_for(1s);

        char c;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            f << "input: " << std::hex << c << std::oct << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    std::string type = "1";

    if (argc == 2) {
        type = argv[1];
    }

    if (type == "1") {
        clearing();
    } else if (type == "2") {
        loading();
    } else {
        std::cout << "Bad girl" << std::endl;
    }
}
