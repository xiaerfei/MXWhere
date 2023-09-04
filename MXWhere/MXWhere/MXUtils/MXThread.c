//
//  MXThread.c
//  MXWhere
//
//  Created by sharexia on 9/4/23.
//

#include "MXThread.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdatomic.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

void *create_thread(void *func, void *param) {
    pthread_t thread = NULL;
    
    int err = pthread_create(&thread, NULL, func, param);
    if (err != 0) {
        return NULL;
    }
    
    return thread;
}

int mx_join(void *thread, void *param) {
    if (!thread) {
        return -1;
    }
    return pthread_join(thread, param);
}
