//
//  Bus.hpp
//  CSMA
//
//  Created by 周昊 on 2020/3/25.
//  Copyright © 2020 aiofwa. All rights reserved.
//

#ifndef Bus_hpp
#define Bus_hpp

#define JAMMING_SIGNAL -1
#define IDLE_SIGNAL 0
// 10M/s ~= 11.9209ns/b
#define BITNANOS 11.9209
#define DELAY 256

class Bus {
public:
    Bus();

    void send(int signal);
    void start();
    void stop();
    
    bool isJammed();
    bool isIdle();

    int signal;
    bool running;
};

bool sleep(long nanos);

#endif /* Bus_hpp */
