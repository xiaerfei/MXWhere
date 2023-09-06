//
//  MXExternalSourceParse.m
//  MXWhere
//
//  Created by sharexia on 9/6/23.
//

#import "MXExternalSourceParseManager.h"
#include "MXExternalSourceParse.hpp"

@interface MXExternalSourceParseManager ()

@property (nonatomic, assign) MXExternalSourceParse *externalSourceParse;
@property (nonatomic, strong) dispatch_queue_t queue;
@property (nonatomic, strong) NSLock *externalSourceParseLock;

@end

@implementation MXExternalSourceParseManager

+ (instancetype)manager {
    static MXExternalSourceParseManager *manager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = [[MXExternalSourceParseManager alloc] init];
        [manager configure];
    });
    return manager;
}

- (void)configure {
    self.queue = dispatch_queue_create("com.parseQueue.MXWhere", NULL);
    self.externalSourceParseLock = [[NSLock alloc] init];
}

- (void)startWithPath:(NSString *)path index:(int)index complete:(void(^)(BOOL success))complete {
    dispatch_async(self.queue, ^{
        if (self.externalSourceParse != nullptr) {
            NSAssert(NO, @"externalSourceParse must be nullptr");
            return;
        }
        [self.externalSourceParseLock lock];
        self.externalSourceParse = new MXExternalSourceParse();
        self.externalSourceParse->Start(path.UTF8String, index);
        [self.externalSourceParseLock unlock];
    });
}

- (void)stop {
    dispatch_async(self.queue, ^{
        if (!self.externalSourceParse) {
            NSAssert(NO, @"externalSourceParse must not be nullptr");
            return;
        }

        self.externalSourceParse->Stop();
        
        [self.externalSourceParseLock lock];
        delete self.externalSourceParse;
        self.externalSourceParse = nullptr;
        [self.externalSourceParseLock unlock];
    });
}

- (BOOL)running {
    BOOL isRunning = NO;
    [self.externalSourceParseLock lock];
    if (self.externalSourceParse) {
        isRunning = (BOOL)self.externalSourceParse->Running();
    }
    [self.externalSourceParseLock unlock];
    return isRunning;
}


@end
