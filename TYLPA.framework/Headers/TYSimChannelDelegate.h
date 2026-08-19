//
//  TYSimChannelDelegate.h
//  TYLPADemo
//
//  Created by 李泳华 on 2022/1/4.
//

#ifndef TYSimChannelDelegate_h
#define TYSimChannelDelegate_h

@protocol TYSimChannelDelegate <NSObject>
- (NSError*)openChannel;
- (NSString*)sendApdu:(NSString*)hexApdu error:(NSError**)pErr;
- (NSError*)closeChannel;

@end


#endif /* TYSimChannelDelegate_h */
