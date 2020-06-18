//
//  Node.hpp
//  UI
//
//  Created by Hao Zhou on 16/06/2020.
//  Copyright © 2020 aiofwa. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include <string>


class Bus;

enum State {
    LISTENING, SENDING, WARNING, IDLE
};

class Node {
public:
    Node() {}
    Node(Bus *bus);
    
    void update();

    int delay;
    int buffer[256];
    Bus *bus;
    State state;
    int ptr;
    int attempts;
};

#endif /* Node_hpp */
