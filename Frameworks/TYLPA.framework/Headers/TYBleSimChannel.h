//
//  TYBleSimChannel.h
//  TYLPA
//
//  Created by 桂靖 on 2024/8/29.
//

#import <Foundation/Foundation.h>
#import "TYSimChannelDelegate.h"

NS_ASSUME_NONNULL_BEGIN

@interface TYBleSimChannel : NSObject <TYSimChannelDelegate>

+(instancetype)shareSdk;

@end

NS_ASSUME_NONNULL_END
