//
//  ObjcNativeClient.h
//  eSIMManagerSDK
//
//  Created by auda on 2025/4/23.
//

#import <Foundation/Foundation.h>
#import "data_buffer.h"

@class AbstractPbDevice;
@class PbOptionsArgs;
@class GrpcEuiccServiceRsp;
@class DownloadMetadataResult;
@class DSListResult;
@class Grpc_EuiccServiceRsp;

// 定义一个返回 NSString 的回调 Block 类型
typedef NSString * _Nullable (^StringCallbackBlock)(void);

@interface ObjcNativeClient : NSObject
@property (nonatomic, strong, nullable) AbstractPbDevice* device;
@property (nonatomic,) void* _Nullable lpa_api;
@property (nonatomic, copy, nullable) NSString* identity;

- (instancetype __nonnull)init;
- (instancetype __nonnull)initWithDevice:(AbstractPbDevice*_Nullable)pbDevice identity:(NSString*_Nonnull)identity;

//设置 加密通信时，验证密码回调
+ (void)setUserAuthentication:(StringCallbackBlock _Nonnull )callback;

- (void)close;
- (nullable NSString*)onGetEid:(int)slotId;
- (nullable NSString*)onGetEuiccInfo:(int)slotId;

- (nonnull GrpcEuiccServiceRsp*)onGetEuiccProfileInfoList:(int)slotId;
- (int)onSwitchToSubscriptionWithSlotId:(int)slotId iccid:(NSString*_Nonnull)iccid;
- (int)onUpdateSubscriptionNickname:(int)slotId iccid:(NSString*_Nonnull)iccid nickname:(NSString*_Nonnull)nickname;
- (int)onDeleteSubscription:(int)slotId iccid:(NSString*_Nonnull)iccid;
- (int)onDownloadSubscriptionWithSlotId:(int)slotId activationCode:(NSString* _Nonnull)activationCode confirmationCode:(NSString* _Nonnull)confirmationCode enable:(bool)enable;
- (DownloadMetadataResult*_Nonnull)onGetDownloadableSubscriptionMetadata:(int)slotId activationCode:(NSString* _Nonnull)activationCode;
- (DSListResult*_Nonnull)onGetDefaultDownloadableSubscriptionList:(int)slotId;
- (int)onDisableSubscription:(int)slotId iccid:(NSString* _Nonnull)iccid;
- (int)syncNotifications:(int)slotId;
- (int)extSetSecureCommunication:(int)code;
- (int)extChangeSecurePsk:(NSString*_Nonnull)psk;
- (int)securityDeleteData:(NSString* _Nonnull)identity type:(int)type;

- (nullable NSString*)getDefaultSmdpAddress:(int)slotId;
- (int)setDefaultSmdpAddress:(int)slotId address:(NSString*_Nonnull)address;

- (nullable NSString*)getDefaultSmdsAddress:(int)slotId;
- (int)setDefaultSmdsAddress:(int)slotId address:(NSString*_Nonnull)address;

void print_log(const char * _Nullable message);
void optionsArg(void);
//int (*direct_data_interface)(int type, const char *req, int reqlen, data_buffer *response, void *arg);
int optionsPbDirectDataInterface(int type, const char *req, int reqlen, data_buffer *response, void *arg);

//透传需要加密的指令
- (NSData*_Nullable)universalDirectDataInterfaceForSecurityWithType:(int)type req:(NSData* _Nonnull )req reqLen:(int)reqLen;
@end
