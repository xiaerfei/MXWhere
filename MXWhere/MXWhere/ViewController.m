//
//  ViewController.m
//  MXWhere
//
//  Created by sharexia on 9/4/23.
//

#import "ViewController.h"
#include "MXThread.h"
#include <pthread.h>
void thread_func(void *param) {
    static int cnt = 0;
    while (1) {
        sleep(1);
        printf("\n thread run %d ~~~ \n", cnt++);
    }
}

@implementation ViewController {
    pthread_t thread;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    pthread_create(&thread, NULL, thread_func, NULL);
    [NSTimer scheduledTimerWithTimeInterval:5 repeats:NO block:^(NSTimer * _Nonnull timer) {
        [timer invalidate];
        printf("\n will stop thread \n");
        pthread_join(self->thread, NULL);
    }];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
