#include "division_os.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t division_by_zero = 0;

void handle_division_signal(int sig) {
    division_by_zero = 1;
}

int main() {
    signal(SIGUSR1, handle_division_signal);
    
    std::string filename;
    std::cin >> filename;
    
    std::ofstream out(filename, std::ios::app);
    if (!out.is_open()) {
        std::cerr << "Fault of opening file" << std::endl;
        return 1;
    }

    int count;
    int signal_to_parent;
    
    while (pipeRead(0, &count, sizeof(count))) {
        if (count == 0) break;
        
        std::vector<float> numbers(count);
        pipeRead(0, numbers.data(), count * sizeof(float));
        
        if (count < 2) {
            out << "Fault: program is needed minimum 2 numbers" << std::endl;
            signal_to_parent = 0;
        } else {

            float result = numbers[0];
            bool error_occurred = false;
            
            for (int i = 1; i < count; i++) {
                if (numbers[i] == 0.0f) {
                    out << "Fault: division by zero" << std::endl;
                    error_occurred = true;
                    signal_to_parent = 1;
                    break;
                }
                result /= numbers[i];
            }
            
            if (!error_occurred) {
                out << numbers[0];
                for (int i = 1; i < count; i++) {
                    out << " / " << numbers[i];
                }
                out << " = " << result << std::endl;
                signal_to_parent = 0;
            }
        }
        
        out.flush();
        
        pipeWrite(1, &signal_to_parent, sizeof(signal_to_parent));
        
        if (signal_to_parent == 1) {
            break;
        }
    }
    
    out.close();
    return 0;
}