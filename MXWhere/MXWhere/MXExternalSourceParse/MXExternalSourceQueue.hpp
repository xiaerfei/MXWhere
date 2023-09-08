//
//  MXExternalSourceQueue.hpp
//  MXWhere
//
//  Created by sharexia on 9/8/23.
//

#ifndef MXExternalSourceQueue_hpp
#define MXExternalSourceQueue_hpp

#include <stdio.h>
#include <thread>
#include <iostream>

typedef struct MXExternalSourceQueueNode {
    uint8_t *data;
    int dataSize;
    // decode H264/H265 need vps、sps and pps
    uint8_t *extraData;
    int extraDataSize;
    
    struct MXExternalSourceQueueNode *next;
} MXExternalSourceQueueNode;

class MXExternalSourceQueue {
private:
    
public:
    
};


#endif /* MXExternalSourceQueue_hpp */
