//
//  MXExternalSourceParseManager.h
//  MXWhere
//
//  Created by sharexia on 9/6/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MXExternalSourceParseManager : NSObject

+ (instancetype)manager;

- (void)startWithPath:(NSString *)path index:(int)index complete:(void(^)(BOOL success))complete;
- (void)stop;
- (BOOL)running;
@end

NS_ASSUME_NONNULL_END
