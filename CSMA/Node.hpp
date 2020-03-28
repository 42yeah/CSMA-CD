//
//  Node.hpp
//  CSMA
//
//  Created by 周昊 on 2020/3/25.
//  Copyright © 2020 aiofwa. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#define WAIT_BITSECS 96
#define JAM_BITS 48
#define MAX_ATTEMPTS 10

#include "Bus.hpp"
#include <string>


class Node {
    friend void start(Node *node);

public:
    Node(Bus *bus);
    
    void start(std::string data);
    void sleep(long nanos);
    void send();
    
private:
    Bus *destination;
    bool running;
    std::string data;
    int dataIndex;
    
    int attempts;
};

#endif /* Node_hpp */
