//
//  TYSimChannel.h
//  TYSimChannel
//
//  Created by 李泳华 on 2022/1/4.
//

#import <Foundation/Foundation.h>
#import "TYSimChannelDelegate.h"



@interface TYSimChannel : NSObject <TYSimChannelDelegate>
@property(nonatomic, copy)NSString *serverHost;
@end
