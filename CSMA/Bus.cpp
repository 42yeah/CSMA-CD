//
//  Bus.cpp
//  CSMA
//
//  Created by 周昊 on 2020/3/25.
//  Copyright © 2020 aiofwa. All rights reserved.
//

#include "Bus.hpp"
#include <iostream>


Bus::Bus() : signal(0), running(false) {
}

bool sleep(long nanos) {
    int seconds = 0;
    timespec spec = { seconds, nanos }, rem = { 0, 0 };
    nanosleep(&spec, &rem);
    return !((rem.tv_nsec != 0 || rem.tv_sec != 0));
}

void Bus::start() {
    running = true;
    while (running) {
        if (!sleep(BITNANOS)) {
            std::cerr << "BUS: INTERRUPTED" << std::endl;
            stop();
        }
//        signal = IDLE_SIGNAL; // Just glob the signal
    }
}

void Bus::stop() { 
    running = false;
}

bool Bus::isJammed() { 
    return signal == JAMMING_SIGNAL;
}

bool Bus::isIdle() {
    return signal == IDLE_SIGNAL;
}

void Bus::send(int signal) {
    sleep(BITNANOS + DELAY * BITNANOS);
    this->signal = signal;
    if (signal >= 32 && signal <= 127) {
        std::cout << (char) signal << " ";
    } else if (signal == -1) {
        std::cout << "[- 拥堵 -] ";
    } else if (signal != 0) {
        std::cout << signal << " ";
    }
}
