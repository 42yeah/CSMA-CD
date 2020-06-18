//
//  Node.cpp
//  UI
//
//  Created by Hao Zhou on 16/06/2020.
//  Copyright © 2020 aiofwa. All rights reserved.
//

#include "Node.hpp"
#include "Bus.hpp"
#include <random>
#define NUMDATA 256
#define JAM_BITS 48


Node::Node(Bus *bus) : bus(bus) {
    std::uniform_int_distribution<> distrib(0, 1);
    std::random_device dev;
    for (int i = 0; i < NUMDATA; i++) {
        buffer[i] = distrib(dev);
    }
    state = LISTENING;
    delay = 96;
    ptr = 0;
    attempts = 0;
}

void Node::update() {
    if (delay <= 0) {
        switch (state) {
            case LISTENING:
                ptr = 0;
                state = SENDING;
                delay = 1;
                break;
                
            case SENDING:
                bus->channel = -1;
            case IDLE:
                ptr = 0;
                delay = 96;
                state = LISTENING;
                break;
                
            case WARNING:
                state = IDLE;
                std::uniform_int_distribution<> distrib(0, pow(attempts, 2) * 10);
                std::random_device dev;
                delay = distrib(dev);
                bus->channel = -1;
                break;
        }
        return;
    }
    if (bus->channel == -2 && state != WARNING && state != IDLE) {
        attempts++;
        state = IDLE;
        std::uniform_int_distribution<> distrib(0, pow(attempts, 2) * 10);
        std::random_device dev;
        delay = distrib(dev);
        return;
    }
    switch (state) {
        case LISTENING:
            if (bus->channel != -1) {
                delay++;
            }
            delay--;
            break;
            
        case SENDING:
            if (ptr != 0 && (bus->channel != buffer[ptr - 1] && bus->channel != -1)) {
                // JAMMED!
                state = WARNING;
                delay = JAM_BITS;
                attempts++;
                break;
            } else if (ptr >= 256) {
                state = LISTENING;
                bus->channel = -1;
                delay = 96;
                attempts = 0;
                break;
            }
            bus->channel = buffer[ptr];
            ptr++;
            break;
            
        case WARNING:
            bus->channel = -2;
            delay--;
            break;
            
        case IDLE:
            delay--;
            break;
    }
}
