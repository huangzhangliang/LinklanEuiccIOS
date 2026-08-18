//
//  TYLPA.h
//  TYLPA
//
//  Created by once on 2024/6/4.
//

#import <Foundation/Foundation.h>
#import "TYSimChannelDelegate.h"
#import "TYSimChannel.h"
#import "TYLProfileInfo.h"

@interface TYLPA : NSObject
@property(nonatomic, assign)id<TYSimChannelDelegate> delegage;
+ (instancetype)defualtLPA;
- (NSString*)getVersion;
- (NSError*)setPatchUrl:(NSString*)strUrl withEnable:(BOOL)isEnable; //优克联蓝牙sim卡补丁设置接口，如果第二个参数启用了补丁下载，则在调用下载接口时会内部查询是否有esim补丁，如果有补丁则自动下载
- (NSError*)enableUpdatePatch:(BOOL)isEnable;  //天喻蓝牙esim卡提供的补丁开关设置接口
- (NSString*)getEid:(NSError**)pErr;
- (NSArray<TYLProfileInfo*>*)getProfiles:(NSError**)pErr;
- (NSError*)downloadProfileWithActiveCode:(NSString*)activeCode confirmCode:(NSString*)confirmCode withIccid:(NSString**)iccid;
- (NSError*)enableProfile:(NSString*)iccid withRefresh:(BOOL) isAutoRefresh;
- (NSError*)disableProfile:(NSString*)iccid withRefresh:(BOOL) isAutoRefresh;
- (NSError*)deleteProfile:(NSString*)iccid;
- (NSError*)uploadNotification;
- (NSString*)enableOtaAndBip:(BOOL) isOpen;
- (NSError*)setIMEI:(NSString*)IMEI;
- (NSString*)IMEIBySet;
- (NSError*)setBipConfigs:(NSArray<TYBipParamEntity*>*)bipConfigs;
- (int)isBipCatBusy;

- (NSError*)downloadPrivateProfileAndEnable:(NSString*)encryptProfileData;
- (NSError*)setNickName:(NSString*)iccid withNickname:(NSString*)name;

//公司蓝牙sim卡使用，增加私有码号和标准码号的profile类型
- (NSError*)enableProfile:(NSString*)iccid withProfileClass:(NSString*)profileClass withRefresh:(BOOL) isAutoRefresh;
- (NSError*)disableProfile:(NSString*)iccid withProfileClass:(NSString*)profileClass withRefresh:(BOOL) isAutoRefresh;
- (NSError*)deleteProfile:(NSString*)iccid withProfileClass:(NSString*)profileClass;

// eSIM升级
- (NSError*)upgradePatch:(void(^)(int))processBlock;


@end
