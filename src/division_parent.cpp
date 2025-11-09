#include "division_os.h"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t division_by_zero = 0;

void handle_signal(int sig) {
    division_by_zero = 1;
}

int main() {
    int pipe_to_child[2];
    int pipe_from_child[2];
    pipeCreate(pipe_to_child);
    pipeCreate(pipe_from_child);

    ProcessRole role = ProcessCreate();

    if (role == IS_CHILD) {
        pipeClose(pipe_to_child[1]);
        redirectFd(pipe_to_child[0], 0);
        pipeClose(pipe_to_child[0]);

        pipeClose(pipe_from_child[0]);
        redirectFd(pipe_from_child[1], 1);
        pipeClose(pipe_from_child[1]);
        
        execl("./division_child", "child", NULL);
        std::cerr << "Fault execl" << std::endl;
        exit(-1);
    }
    
    if (role == IS_PARENT) {
        pipeClose(pipe_to_child[0]);
        pipeClose(pipe_from_child[1]);

        signal(SIGUSR1, handle_signal);

        std::string file_name;
        std::cout << "Type file's name: ";
        std::cin >> file_name;
        
        std::string data = file_name + "\n";
        pipeWrite(pipe_to_child[1], data.c_str(), data.size());

        std::cout << "Write numbers thought space(first divides by others):" << std::endl;
        std::cout << "For exit write 'q'" << std::endl;
        
        std::string input;
        std::cin.ignore();
        
        int signal_from_child;
        
        while (!division_by_zero) {
            std::cout << "Write numbers: ";
            std::getline(std::cin, input);
            
            if (input == "q" || input == "quit") {
                break;
            }
            
            if (input.empty()) {
                continue;
            }
            
            std::vector<float> numbers;
            std::stringstream ss(input);
            float num;
            
            while (ss >> num) {
                numbers.push_back(num);
            }
            
            if (numbers.size() > 0) {
                int count = numbers.size();
                pipeWrite(pipe_to_child[1], &count, sizeof(count));
                
                pipeWrite(pipe_to_child[1], numbers.data(), count * sizeof(float));
                
                pipeRead(pipe_from_child[0], &signal_from_child, sizeof(signal_from_child));
                
                if (signal_from_child == 1) {
                    std::cout << "Divide by zero. Program is closing" << std::endl;
                    division_by_zero = true;
                }
            } else {
                std::cout << "Error: false format" << std::endl;
            }
        }
        
        if (!division_by_zero) {
            int count = 0;
            pipeWrite(pipe_to_child[1], &count, sizeof(count));
        }
        
        pipeClose(pipe_to_child[1]);
        pipeClose(pipe_from_child[0]);
        
        wait(NULL);
        std::cout << "Program finished" << std::endl;
    }

    return 0;
}