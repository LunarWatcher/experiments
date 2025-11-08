#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stc/Environment.hpp>
#include <thread>
#include <unistd.h>
#include <pty.h>
#include <poll.h>
#include <fcntl.h>


template <class T, typename = std::enable_if_t<std::is_base_of_v<std::ios_base, T>>>
class StreamCapturer {
private:
    T& stream;
    decltype(stream.rdbuf()) buffer;
public:
    std::stringstream control;
    StreamCapturer(T& stream) : stream(stream) {
        this->buffer = stream.rdbuf();
        this->stream.rdbuf(control.rdbuf());
    }
    ~StreamCapturer() { restore(); }
    void restore() {
        if (buffer != nullptr) {
            stream.rdbuf(buffer);
            buffer = nullptr;
        }
    }
    void reset() { stream.rdbuf(buffer); }
    std::stringstream& getBuffer() { return this->control; }
    std::string getBufferContent() { return this->control.str(); }
};

void goal1() {
    std::cout << "Running goal 1" << std::endl;
    StreamCapturer c{ std::cin };
    std::string line = "hi";

    c.control << line << std::endl;

    std::string buff;
    std::getline(std::cin, buff);

    std::cout << "stdin got [[\"" << buff << "\"]]" << std::endl;;
    std::getline(std::cin, buff);
    std::cout << "stdin 2 got [[\"" << buff << "\"]]" << std::endl;;
    std::getline(std::cin, buff);
    std::cout << "stdin 3 got [[\"" << buff << "\"]]" << std::endl;;
    std::cout << "Control buffer contains " << c.control.str() << std::endl;

    c.control = {};
    std::getline(std::cin, buff);
    std::cout << "stdin 4 got [[\"" << buff << "\"]]" << std::endl;;
    std::cout << "Control buffer contains " << c.control.str() << std::endl;

    c.control << "exit 69" << std::endl;
    int code;
    std::string s = stc::syscommand(std::vector<const char*>{
        "zsh", "-s"
    }, &code);
    std::cout << "Output code is " << code << std::endl;
    std::cout << "Full output is " << s << std::endl;
}

std::string runAndHijackStdin(std::vector<const char*> command, std::stringstream& ss) {
    command.push_back(nullptr);
    std::array<char, 256> buffer;
    std::string res;
    int codeOutput;

    int fd[2], stdinFd[2];
    if (pipe(fd) != 0 || pipe(stdinFd) != 0) {
        throw std::runtime_error("Failed to create pipe");
    }

    auto pid = fork();

    if (pid < 0) {
        throw std::runtime_error("Fork error");
    } else if (pid == 0) {
        // Child process

        dup2(fd[1], STDOUT_FILENO);
        dup2(fd[1], STDERR_FILENO);
        dup2(stdinFd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        close(stdinFd[0]);
        close(stdinFd[1]);

        execv(command.at(0), (char**) command.data());
        exit(1);
    } else {
        close(fd[1]);
        close(stdinFd[0]);
        size_t bytes = 0;
        std::string buff;
        while (std::getline(ss, buff)) {
            buff += "\n";
            size_t sum = 0;
            while ((bytes = write(stdinFd[1], buff.data(), buff.size())) > 0) {
                sum += bytes;
                std::cout << "wrote " << bytes << " bytes (" << buff << ")" << std::endl;
                if (sum >= buff.size()) {
                    break;
                }
            }
            std::cout << "Out of loop" << std::endl;
        }
        close(stdinFd[1]);
        bytes = 0;
        while ((bytes = read(
            fd[0],
            buffer.data(), buffer.size()
        )) > 0) {
            res.insert(res.end(), buffer.begin(), buffer.begin() + bytes);
        }

        int status;
        waitpid(pid, &status, 0);
        std::cout << "Process exit status is " << WEXITSTATUS(status) << std::endl;
        return res;
    }

}

void goal2() {
    std::stringstream ss1;
    ss1 << "echo 'This is line 1'" << std::endl << "echo 'This is line 2'" << std::endl;
    // ss1 << "EOF" << std::endl;

    std::string res = runAndHijackStdin({
        "./bin/mocksub", "spawn"
    }, ss1);

    std::cout << "Output:\n-------\n" << res << std::endl << "=== END ===\n";
}


void goal3() {
    int master, slave;
    if (openpty(&master, &slave, nullptr, nullptr, nullptr) == -1) {
        throw std::runtime_error("Failed to open pty");
    }
    int flags = fcntl(master, F_GETFL, 0);
    fcntl(master, F_SETFL, flags | O_NONBLOCK);
    // std::vector<const char*> command = {
    //     "/usr/bin/env", "bash", "-i",
    //     nullptr
    // };
    std::vector<const char*> command = {
        "./bin/mocksub", "spawn",
        nullptr
    };

    struct termios ts;
    if (tcgetattr(slave, &ts) == -1) { perror("tcgetattr"); exit(1); }

    // ts.c_lflag &= ~ECHO;
    if (tcsetattr(slave, TCSANOW, &ts) == -1) { perror("tcsetattr"); exit(1); }

    auto pid = fork();
    if (pid < 0) {
        throw std::runtime_error("Fork error");
    } else if (pid == 0) {
        // Child process
        setsid();
        dup2(slave, STDOUT_FILENO);
        dup2(slave, STDERR_FILENO);
        dup2(slave, STDIN_FILENO);
        close(slave);
        close(master);

        execv(command.at(0), (char**) command.data());
        exit(1);
    } else {

        std::string buffer;
        size_t bytes = 0;
        std::string buff;
        std::stringstream ss;
        ss << "echo 'hi'" << std::endl;
        ss << "exit 69" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        while (std::getline(ss, buff)) {
            buff += "\n";
            size_t sum = 0;
            while ((bytes = write(master, buff.data(), buff.size())) > 0) {
                sum += bytes;
                std::cout << "wrote " << bytes << " bytes (" << buff << ")" << std::endl;
                if (sum >= buff.size()) {
                    break;
                }
            }
            std::cout << "Out of loop" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::array<char, 4096> buf;
        ssize_t size;
        nfds_t nfds = 1;

        std::string res;
        pollfd pdfs = {
            .fd = master,
            .events = POLLIN
        };
        size_t count = 0;
        while (true) {
            if (poll(&pdfs, nfds, 100)) {
                while ((bytes = read(
                            master,
                            buf.data(), buf.size()
                )) > 0) {
                    if (bytes == (size_t) -1) {
                        break;
                    }
                    res.insert(res.end(), buf.begin(), buf.begin() + bytes);
                }
            }

            int status;
            if (waitpid(pid, &status, WNOHANG)) {
                std::cout << "Process exit status is " << WEXITSTATUS(status) << std::endl;
                std::cout << "Output: " << res << std::endl;
                break;
            }
        }

        close(master);
        close(slave);
    }
}

int main() {
    // goal1();
    // goal2();
    goal3();
}
