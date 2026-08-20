//
//  TYLProfileInfo.h
//  TYLPAObjc
//
//  Created by 李泳华 on 2022/1/5.
//

#import <Foundation/Foundation.h>





@interface TYBipParamEntity : NSObject

@property (nonatomic, copy) NSString *url;
@property (nonatomic, copy) NSString *ip;
@property (nonatomic, copy) NSString *agentId;
@property (nonatomic) NSInteger port;

@end





@interface TYLProfileInfo : NSObject
@property(nonatomic, copy)NSString *iccid;
@property(nonatomic, copy)NSString *profileNickName;
@property(nonatomic, copy)NSString *profileState;
@property(nonatomic, copy)NSString *serviceproviderName;
@property(nonatomic, copy)NSString *profileName;
@property(nonatomic, copy)NSString *iconType;
@property(nonatomic, copy)NSString *icon;
@property(nonatomic, copy)NSString *profileClass;
@property(nonatomic, copy)NSString *notificationConfigurationInfo;
@property(nonatomic, copy)NSString *profileOwner;
@property(nonatomic, copy)NSString *dpProprietaryData;
@property(nonatomic, copy)NSString *profilePolicyRules;
@property(nonatomic, copy)NSString *isdPAid;
@property(nonatomic, copy)NSString *operaMark;
@property(nonatomic, copy)NSString *writeState;
@property(nonatomic, copy)NSString *phoneNumber;
@property(nonatomic, copy)NSString *packageName;
@property(nonatomic, copy)NSString *profileIndex;
@property(nonatomic, copy)NSString *mcc;
@property(nonatomic, copy)NSString *mnc;
@end

 
