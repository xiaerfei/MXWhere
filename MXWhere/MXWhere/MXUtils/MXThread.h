//
//  MXThread.h
//  MXWhere
//
//  Created by sharexia on 9/4/23.
//

#ifndef MXThread_h
#define MXThread_h

#include <stdio.h>

void *create_thread(void *func, void *param);

int mx_join(void *thread, void *param);
#endif /* MXThread_h */
