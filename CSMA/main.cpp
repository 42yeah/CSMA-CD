//
//  main.cpp
//  CSMA
//
//  Created by 周昊 on 2020/3/25.
//  Copyright © 2020 aiofwa. All rights reserved.
//

#include <iostream>
#include "Bus.hpp"
#include "Node.hpp"


int main() {
    Bus bus;
    Node a(&bus), b(&bus);
    a.start("I can eat the glass, it doesn't hurt me.");
    b.start("The quick brown fox jumps over the lazy dog.");
    bus.start();
    return 0;
}
