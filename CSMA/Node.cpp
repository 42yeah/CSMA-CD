//
//  Node.cpp
//  CSMA
//
//  Created by 周昊 on 2020/3/25.
//  Copyright © 2020 aiofwa. All rights reserved.
//

#include "Node.hpp"
#include <thread>
#include <random>
#include <iostream>


Node::Node(Bus *bus) : destination(bus) {
}

void start(Node *node) {
    std::random_device dev;
    while (node->running && node->dataIndex <= node->data.length() - 1) {
        // === LISTEN === //
        bool idle = true;
        for (int i = 0; i < WAIT_BITSECS; i++) {
            idle &= node->destination->isIdle();
            sleep(BITNANOS);
        }
        if (!idle) {
            std::uniform_int_distribution<> distrib(1, 25);
            sleep(BITNANOS * distrib(dev));
            continue;
        }
        // === IT'S IDLE. SEND DATA! === //
        node->send();
    }
}

void Node::send() {
    while (running && dataIndex <= data.length() - 1) {
        std::random_device dev;
        destination->send(data[dataIndex]);
        if (destination->signal != data[dataIndex]) {
            std::uniform_int_distribution<> distrib(0, pow(attempts, 2) * 10);
            float nanos = ((JAM_BITS + 1) * (BITNANOS + DELAY)) + BITNANOS * distrib(dev);
            std::string message = "[! 拥堵, 等待 " + std::to_string(nanos) + " 纳秒 !] ";
            std::cerr << message << std::endl;
            if (attempts > MAX_ATTEMPTS) {
                std::cerr << "[! 过多重试 !] " << std::endl;
                running = false;
                return;
            }
            if (destination->signal != JAMMING_SIGNAL) {
                std::cerr << "[! 发送拥堵信号 !] " << std::endl;
                for (int i = 0; i < JAM_BITS; i++) {
                    destination->send(JAMMING_SIGNAL);
                }
            }
            attempts++;
            sleep(nanos);
            continue;
        }
        attempts = 1;
        dataIndex++;
    }
    std::cout << "[+ 发送完成 +] " << std::endl;
}

void Node::start(std::string data) {
    this->data = data;
    attempts = 1; // 1st attempt
    dataIndex = 0;
    running = true;
    std::thread thread(::start, this);
    thread.detach();
}

void Node::sleep(long nanos) {
    if (!::sleep(nanos)) {
        running = false;
    }
    
}
