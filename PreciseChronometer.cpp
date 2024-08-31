#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>

class Chronometer {
public:
    void start() {
        if (!running) {
            start_time = std::chrono::steady_clock::now();
            running = true;
            std::cout << "Chronometer started." << std::endl;
        } else {
            std::cout << "Chronometer is already running." << std::endl;
        }
    }

    void stop() {
        if (running) {
            end_time = std::chrono::steady_clock::now();
            running = false;
            elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            std::cout << "Chronometer stopped." << std::endl;
        } else {
            std::cout << "Chronometer is already stopped." << std::endl;
        }
    }

    void reset() {
        running = false;
        elapsed = std::chrono::nanoseconds::zero();
        std::cout << "Chronometer reset." << std::endl;
    }

    void display(char unit) const {
        auto current_time = running ? std::chrono::steady_clock::now() : end_time;
        auto total_elapsed = elapsed + (running ? std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time) : std::chrono::nanoseconds::zero());

        switch (unit) {
            case 'h': // Hours
                std::cout << "\r" << std::chrono::duration_cast<std::chrono::hours>(total_elapsed).count() << " hours" << std::flush;
                break;
            case 'm': // Minutes
                std::cout << "\r" << std::chrono::duration_cast<std::chrono::minutes>(total_elapsed).count() << " minutes" << std::flush;
                break;
            case 's': // Seconds
                std::cout << "\r" << std::chrono::duration_cast<std::chrono::seconds>(total_elapsed).count() << " seconds" << std::flush;
                break;
            case 'S': // Milliseconds
                std::cout << "\r" << std::chrono::duration_cast<std::chrono::milliseconds>(total_elapsed).count() << " milliseconds" << std::flush;
                break;
            case 'u': // Microseconds
                std::cout << "\r" << std::chrono::duration_cast<std::chrono::microseconds>(total_elapsed).count() << " microseconds" << std::flush;
                break;
            case 'n': // Nanoseconds
                std::cout << "\r" << std::chrono::duration_cast<std::chrono::nanoseconds>(total_elapsed).count() << " nanoseconds" << std::flush;
                break;
            default:
                std::cout << "\rInvalid time unit." << std::flush;
                break;
        }
    }

    bool is_running() const {
        return running;
    }

private:
    bool running = false;
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    std::chrono::nanoseconds elapsed{0};
};

int main() {
    Chronometer chronometer;

    char command;
    char unit;
    std::cout << "Select time unit: (h)ours, (m)inutes, (s)econds, (S)milliseconds, (u)microseconds, (n)anoseconds: ";
    std::cin >> unit;

    std::chrono::nanoseconds sleep_duration;
    switch (unit) {
        case 'h': sleep_duration = std::chrono::hours(1); break;
        case 'm': sleep_duration = std::chrono::minutes(1); break;
        case 's': sleep_duration = std::chrono::seconds(1); break;
        case 'S': sleep_duration = std::chrono::milliseconds(1); break;
        case 'u': sleep_duration = std::chrono::microseconds(1); break;
        case 'n': sleep_duration = std::chrono::nanoseconds(1); break;
        default: std::cout << "Invalid time unit. Exiting." << std::endl; return 1;
    }

    while (true) {
        std::cout << "\nStart (S), Stop (D), Reset (R), Quit (Q): ";
        std::cin >> command;

        if (command == 's' || command == 'S') {
            chronometer.start();
            while (chronometer.is_running()) {
                chronometer.display(unit);
                std::this_thread::sleep_for(sleep_duration); // Adjust sleep duration according to the selected unit
            }
        } else if (command == 'd' || command == 'D') {
            chronometer.stop();
        } else if (command == 'r' || command == 'R') {
            chronometer.reset();
        } else if (command == 'q' || command == 'Q') {
            std::cout << "Exiting..." << std::endl;
            break;
        }
    }

    return 0;
}
