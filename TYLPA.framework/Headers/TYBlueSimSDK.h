//
//  TYBlueSimSDK.h
//  TYBlueSimSDK
//
//  Created by 桂靖 on 2024/7/22.
//

#import <Foundation/Foundation.h>


@protocol TYBlueSimDelegate <NSObject>

@optional
-(void)didBlueConnect;//蓝牙连接成功
-(void)didConnect;  //认证后连接成功的回调
-(void)didDisconnect; //断开连接
-(void)didConnectFailed; //连接失败
-(void)didConnectOvertime; //连接超时或者搜索超时

-(void)didScanSuccess; //搜索到设备的回调

@end


@interface TYBlueSimSDK : NSObject

@property (nonatomic, weak) id<TYBlueSimDelegate> delegate;  //蓝牙回调代理
@property (nonatomic, assign, readonly) BOOL connectStatus;  //当前蓝牙连接状态
@property (nonatomic, assign) NSUInteger overtimeInterval; //超时时间设置，默认15秒

@property (nonatomic, assign) BOOL isAuthPin; //是否做PIN认证

@property (nonatomic, assign) NSUInteger mtuLength;
@property(nonatomic,assign)BOOL isAddApduLen;



+(TYBlueSimSDK*)defaultSdk;   //单例

-(int)scanAndConnectWithBlueName:(NSString*)deviceName;

-(void)disconnectBlueSim; //断开连接

-(NSString*)getVersion; //获取sdk版本号

-(NSString*)sendSyncApdu:(NSString*)apdu; //同步发送apdu指令

@end
