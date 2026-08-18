//
//  TYOtgSimChannel.h
//  TYLPA
//

#import <Foundation/Foundation.h>
#import "TYSimChannelDelegate.h"

NS_ASSUME_NONNULL_BEGIN

@interface TYOtgSimChannel : NSObject <TYSimChannelDelegate>
+(instancetype)defualtOtgChannel;
// 当otg设备被拔出时的回调
- (void)onOtgDisconnect:(void(^)(void))block;
// 连接otg设备
- (void)connectOtg:(void(^)(BOOL success))block;
// 清除已连接的otg设备(onOtgDisconnect设置的回调并没有清除)
- (void)clearOtg;
@end

NS_ASSUME_NONNULL_END
