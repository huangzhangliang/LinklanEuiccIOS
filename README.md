# LinklanLPA iOS SDK 对接文档

**SDK 版本：1.0.8**

**模块名：`LinklanEuicc`**

## 目录

- [1. 概述](#1-概述)
- [2. 环境要求](#2-环境要求)
- [3. 集成配置](#3-集成配置)
  - [3.1 权限配置](#31-权限配置)
  - [3.2 依赖配置](#32-依赖配置)
- [4. 核心数据模型](#4-核心数据模型)
  - [4.1 BleDevice](#41-bledevice)
  - [4.2 EuiccDevice](#42-euiccdevice)
  - [4.3 ProfileInfo](#43-profileinfo)
  - [4.4 EuiccResult&lt;T&gt;](#44-euiccresultt)
- [5. 回调协议](#5-回调协议)
  - [5.1 BleScanCallback](#51-blescancallback)
  - [5.2 BleDeviceConnectionListener](#52-bledeviceconnectionlistener)
  - [5.3 BleUserAuthenticationListener](#53-bleuserauthenticationlistener)
- [6. API 参考](#6-api-参考)
  - [6.1 初始化](#61-初始化)
  - [6.2 BLE 扫描](#62-ble-扫描)
  - [6.3 设备连接与断开](#63-设备连接与断开)
  - [6.4 设备信息](#64-设备信息)
  - [6.5 设备重置](#65-设备重置)
  - [6.6 Profile 管理](#66-profile-管理)
  - [6.7 APN 管理](#67-apn-管理)
  - [6.8 APDU 设置](#68-apdu-设置)
- [7. 设备型号支持矩阵](#7-设备型号支持矩阵)
- [8. 监听器设置](#8-监听器设置)
- [9. 错误码说明](#9-错误码说明)
- [10. 完整对接示例](#10-完整对接示例)
- [11. 注意事项](#11-注意事项)

---

## 1. 概述

LinklanLPA 是一款基于 iOS BLE（CoreBluetooth）的 eSIM 设备管理 SDK，提供设备扫描、连接、Profile 下载/启用/禁用/删除、APN 配置等完整能力。


> **核心入口：** `LinklanLPA.shared`（单例，全局唯一实例）

---

## 2. 环境要求

| 项目 | 要求 |
|------|------|
| iOS 最低版本 | iOS 13.0+（推荐 iOS 15.0+ 以获得完整 async/await 支持） |
| Swift 版本 | Swift 5.5+ |
| BLE 支持 | 设备需支持 Bluetooth LE |
| 依赖框架 | CoreBluetooth、Foundation |
| Xcode 版本 | Xcode 13.0+ |

> **注意：** SDK 使用了 Swift Concurrency（`async`/`await`），调用异步方法需在 `Task` 或 `async` 上下文中执行。iOS 13 可通过 `@available` 检查在运行时降级处理。

---

## 3. 集成配置

### 3.1 权限配置

在 `Info.plist` 中声明以下蓝牙权限：

| Key | Value | 说明 |
|-----|-------|------|
| `NSBluetoothAlwaysUsage` | 应用需要蓝牙权限以连接 eSIM 设备 | 蓝牙使用说明（iOS 13+ 必需） |
| `NSBluetoothPeripheralUsage` | 应用需要蓝牙权限以连接 eSIM 设备 | 旧版蓝牙权限（iOS 12 及以下） |

如果需要后台蓝牙通信，在 `Info.plist` 中添加：

```xml
<key>UIBackgroundModes</key>
<array>
    <string>bluetooth-central</string>
</array>
```

> **注意：** iOS 不像 Android 需要运行时动态申请蓝牙权限，系统会在首次调用蓝牙 API 时自动弹出权限对话框。但必须在 `Info.plist` 中声明 usage description，否则应用会崩溃。

### 3.2 依赖配置

#### 方式一：CocoaPods

```ruby
# Podfile
target 'YourApp' do
  use_frameworks!
  pod 'LinklanEuiccIOS'
end
```

```bash
pod install
```

#### 方式二：Swift Package Manager (SPM)

在 Xcode 中：
1. File → Add Packages
2. 输入仓库地址
3. 选择版本规则 `Up to Next Major Version`，填入 `1.1.0`

或在 `Package.swift` 中：

```swift
dependencies: [
    .package(url: "https://github.com/huangzhangliang/LinklanEuicciOS.git", from: "1.1.0")
]
```

#### 方式三：手动集成（XCFramework）

将 `LinklanEuicc.xcframework` 拖入 Xcode 项目，在 Target → General → Frameworks, Libraries, and Embedded Content 中添加并设为 `Embed & Sign`。

---

## 4. 核心数据模型

### 4.1 BleDevice

BLE 扫描发现的设备信息，用于 `connectDevice` 连接入参。

```swift
public struct BleDevice {
    public let address: String              // 设备 MAC 地址（iOS 下为 UUID 字符串）
    public let name: String                 // 设备名称
    public let rssi: Int                    // 信号强度（dBm）
    public let model: String                // 设备型号标识
    public let scanRecord: Data?            // 扫描记录原始数据
    public let serviceUuids: [String]?      // 服务 UUID 列表
    public let txPower: Int?                // 发射功率（dBm）
    public let manufacturerData: [Int: Data]? // 厂商自定义数据（key 为 Company ID）
    public let lastSeen: Date               // 最后发现时间
}
```

> 该结构体实现了 `Equatable`、`Hashable`（基于 `address` 字段）和 `CustomStringConvertible`。

**便利方法：**

```swift
/// 是否包含指定服务 UUID
public func hasServiceUuid(_ uuid: String) -> Bool
```

### 4.2 EuiccDevice

已连接的 eSIM 设备信息，作为大部分操作的上下文对象。

```swift
public struct EuiccDevice {
    public var eid: String?                    // eUICC 标识符 (EID)
    public var imei: String?                   // 设备 IMEI
    public var sn: String?                     // 设备序列号
    public var name: String?                   // 设备名称
    public var model: String?                  // 设备型号标识
    public var deviceName: String?             // 蓝牙广播名称
    public var address: String?                // 蓝牙 MAC 地址
    public var vendor: String?                 // 设备厂商
    public var freeVolatileMemory: Int?        // 可用易失性内存（KB）
    public var freeNonVolatileMemory: Int?     // 可用非易失性内存（KB）
}
```

### 4.3 ProfileInfo

eSIM Profile（配置文件）信息。

```swift
public struct ProfileInfo {
    public var iccid: String?                  // ICCID（Profile 唯一标识）
    public var profileName: String?            // Profile 名称
    public var nickname: String?              // 用户自定义昵称
    public var profileClass: Int?             // Profile 类别（0=Test, 1=Provisioning, 2=Operational）
    public var serviceProviderName: String?    // 运营商名称
    public var policyRules: Int?              // 策略规则
    public var state: Int?                    // Profile 状态（0=禁用, 1=启用）
    public var mcc: String?                   // 移动国家码 (MCC)
    public var mnc: String?                   // 移动网络码 (MNC)
    public var imsi: String?                 // IMSI
}
```

**便利计算属性：**

| 属性 | 类型 | 说明 |
|------|------|------|
| `stateEnum` | `State` | Profile 状态枚举（`.disabled` / `.enabled` / `.unknown`） |
| `classEnum` | `Class` | Profile 类别枚举（`.test` / `.provisioning` / `.operational` / `.unknown`） |
| `isEnabled` | `Bool` | 是否已启用 |
| `isOperational` | `Bool` | 是否运营 Profile |
| `mccmnc` | `String?` | MCC + MNC 拼接（如 "46000"） |
| `displayName` | `String` | 显示名称（优先昵称 → Profile 名 → ICCID 后 8 位） |

> 该结构体实现了 `Equatable`、`Hashable`（基于 `iccid` 字段）和 `CustomStringConvertible`。

### 4.4 EuiccResult&lt;T&gt;

所有异步操作的统一返回包装类型。

```swift
public struct EuiccResult<T> {
    public var code: Int?          // 结果码（0 = 成功，1 = 失败）
    public var data: T?            // 返回数据
    public var title: String?     // 结果标题
    public var msg: String?       // 结果消息
    public var info: String?      // 附加信息

    /// 是否成功
    public var isSuccess: Bool { code == 0 }
}
```

**静态工厂方法：**

```swift
// 成功（无数据）
EuiccResult<Void>.success()

// 成功（带数据）
EuiccResult.success(data: T) -> EuiccResult<T>

// 失败
EuiccResult.failure(code: Int, msg: String) -> EuiccResult<Void>
EuiccResult.failure(code: Int, title: String?, msg: String?, info: String?) -> EuiccResult<T>
```

---

## 5. 回调协议

### 5.1 BleScanCallback

BLE 扫描回调协议，实现后传给 `startScan(callback:)`。

```swift
public protocol BleScanCallback {
    /// 发现新设备
    func onDeviceFound(device: BleDevice)

    /// 设备信号更新（已存在设备的 RSSI 变化）
    func onDeviceUpdated(device: BleDevice)

    /// 扫描状态变化
    func onScanStateChanged(isScanning: Bool)

    /// 扫描失败
    func onScanFailed(errorCode: Int, errorMsg: String)
}
```

> `onDeviceUpdated` 和 `onScanStateChanged` 提供了默认空实现，可按需覆写。

### 5.2 BleDeviceConnectionListener

蓝牙设备连接状态监听协议。

```swift
public protocol BleDeviceConnectionListener {
    /// 设备连接状态变化回调
    /// - Parameters:
    ///   - address: 设备 MAC 地址
    ///   - status: 状态值（"connected" / "disconnected" / "connecting" / "timeout" / "error"）
    func onStatusChange(address: String?, status: String)
}
```

### 5.3 BleUserAuthenticationListener

蓝牙用户认证监听协议。

```swift
public protocol BleUserAuthenticationListener {
    /// 提示用户进行认证
    /// - Parameter address: 设备 MAC 地址
    /// - Returns: 用户输入的密码
    func onPromptUserAuthentication(address: String?) -> String
}
```

> 该协议为同步回调，SDK 调用时会阻塞当前线程等待返回密码。如果涉及 UI 交互，需注意线程处理。

---

## 6. API 参考

### 6.1 初始化

#### `initSDK`

```swift
public func initSDK(debug: Bool = false)
```

**说明：** 初始化 SDK，设置调试模式开关。必须在调用任何其他 API 之前调用。内部会初始化 TY 客户端和 BLE 扫描器。

| 参数 | 类型 | 必填 | 默认值 | 说明 |
|------|------|:----:|:------:|------|
| debug | Bool | 否 | false | 是否开启调试日志 |

**示例：**

```swift
// 在 AppDelegate 或 App 入口处调用
LinklanLPA.shared.initSDK(debug: true)
```

> 建议在 `UIApplicationDelegate.application(_:didFinishLaunchingWithOptions:)` 中调用。

---

### 6.2 BLE 扫描

#### `startScan`

```swift
public func startScan(callback: BleScanCallback)
```

**说明：** 开始扫描 BLE 设备，扫描结果通过回调返回。

| 参数 | 类型 | 必填 | 说明 |
|------|------|:----:|------|
| callback | BleScanCallback | 是 | 扫描结果回调协议实现 |

#### `stopScan`

```swift
public func stopScan()
```

**说明：** 停止 BLE 扫描。

**示例：**

```swift
// 开始扫描
LinklanLPA.shared.startScan(callback: ScanController())

// 停止扫描
LinklanLPA.shared.stopScan()
```

```swift
// 协议实现示例
class ScanController: BleScanCallback {
    func onDeviceFound(device: BleDevice) {
        print("发现设备: \(device.name) (\(device.address)) model=\(device.model)")
    }

    func onScanFailed(errorCode: Int, errorMsg: String) {
        print("扫描失败: \(errorCode) - \(errorMsg)")
    }
}
```

---

### 6.3 设备连接与断开

#### `connectDevice`

```swift
public func connectDevice(device: BleDevice?) async -> EuiccResult<EuiccDevice>
```

**说明：** 连接 BLE 设备。根据 `device.model` 自动路由到对应客户端。连接成功后返回 `EuiccDevice` 对象（已填充 name 和 model）。

| 参数 | 类型 | 必填 | 说明 |
|------|------|:----:|------|
| device | BleDevice? | 否 | 扫描到的设备对象，`nil` 或 model 不匹配时返回 `code = 1` |

**返回：** `EuiccResult<EuiccDevice>`

#### `checkDeviceConnection`

```swift
public func checkDeviceConnection(device: EuiccDevice?) -> Bool
```

**说明：** 检查设备是否已连接（同步方法）。

| 参数 | 类型 | 必填 | 说明 |
|------|------|:----:|------|
| device | EuiccDevice? | 否 | 已连接的设备对象 |

**返回：** `Bool` — `true` 表示已连接

#### `disconnectDevice`

```swift
public func disconnectDevice(device: EuiccDevice?)
```

**说明：** 断开与设备的蓝牙连接（同步方法）。

**示例：**

```swift
Task {
    // 连接设备
    let result = await LinklanLPA.shared.connectDevice(device: bleDevice)
    if result.isSuccess {
        let euiccDevice = result.data!
        print("连接成功: \(euiccDevice.name ?? "")")
    }

    // 检查连接状态
    let connected = LinklanLPA.shared.checkDeviceConnection(euiccDevice)

    // 断开连接
    LinklanLPA.shared.disconnectDevice(euiccDevice)
}
```

---

### 6.4 设备信息

#### `getDeviceInfo`

```swift
public func getDeviceInfo(device: EuiccDevice) async -> EuiccResult<EuiccDevice>
```

**说明：** 获取设备的详细信息（EID、IMEI、SN、内存等）。需先连接设备。

| 参数 | 类型 | 必填 | 说明 |
|------|------|:----:|------|
| device | EuiccDevice | 是 | 已连接的设备对象，`address` 不能为空 |

**返回：** `EuiccResult<EuiccDevice>` — `data` 中包含完整的设备信息

**示例：**

```swift
Task {
    let result = await LinklanLPA.shared.getDeviceInfo(device: euiccDevice)
    if result.isSuccess {
        let info = result.data!
        print("EID: \(info.eid ?? "N/A")")
        print("IMEI: \(info.imei ?? "N/A")")
        print("SN: \(info.sn ?? "N/A")")
        print("可用易失内存: \(info.freeVolatileMemory ?? 0) KB")
    }
}
```

---

### 6.5 设备重置

#### `resetDevice`

```swift
public func resetDevice(device: EuiccDevice?, duration: UInt64 = 15000) async
```

**说明：** 重置设备
| 参数 | 类型 | 必填 | 默认值 | 说明 |
|------|------|:----:|:------:|------|
| device | EuiccDevice? | 否 | — | 目标设备 |
| duration | UInt64 | 否 | 15000 | 等待时长（毫秒），仅对 bs/ty 生效 |

---

### 6.6 Profile 管理

#### `getProfileInfoList`

```swift
public func getProfileInfoList(device: EuiccDevice?) async -> EuiccResult<[ProfileInfo]>
```

**说明：** 获取设备上所有 Profile 列表。

| 参数 | 类型 | 必填 | 说明 |
|------|------|:----:|------|
| device | EuiccDevice? | 否 | 已连接的设备对象 |

**返回：** `EuiccResult<[ProfileInfo]>`

#### `enableProfile`

```swift
public func enableProfile(device: EuiccDevice?, iccid: String?) async -> EuiccResult<String>
```

**说明：** 启用指定的 Profile。启用成功后，SDK 会重置设备：


| 参数 | 类型 | 必填 | 说明 |
|------|------|:----:|------|
| device | EuiccDevice? | 否 | 已连接的设备对象 |
| iccid | String? | 否 | 目标 Profile 的 ICCID |

**返回：** `EuiccResult<String>`

#### `disableProfile`

```swift
public func disableProfile(device: EuiccDevice?, iccid: String?) async -> EuiccResult<String>
```

**说明：** 禁用指定的 Profile。禁用后的重置策略与 `enableProfile` 一致。

#### `deleteProfile`

```swift
public func deleteProfile(device: EuiccDevice?, iccid: String?) async -> EuiccResult<String>
```

**说明：** 删除指定的 Profile。

> 删除操作完成后不会自动重置设备。

#### `downloadProfile`

```swift
public func downloadProfile(
    device: EuiccDevice?,
    activeCode: String?,
    confirmCode: String?
) async -> EuiccResult<String>
```

**说明：** 下载 Profile。

| 参数 | 类型 | 必填 | 说明 |
|------|------|:----:|------|
| device | EuiccDevice? | 否 | 已连接的设备对象 |
| activeCode | String? | 否 | 激活码，格式：`LPA:1$smdpServer$matchingId` |
| confirmCode | String? | 否 | 确认码（可选） |

**返回：** `EuiccResult<String>` — `data` 为下载的 Profile 的 ICCID

**示例：**

```swift
Task {
    // 获取 Profile 列表
    let listResult = await LinklanLPA.shared.getProfileInfoList(device: euiccDevice)
    if listResult.isSuccess {
        listResult.data!.forEach { profile in
            print("ICCID: \(profile.iccid ?? "")")
            print("运营商: \(profile.serviceProviderName ?? "")")
            print("状态: \(profile.isEnabled ? "已启用" : "已禁用")")
            print("显示名: \(profile.displayName)")
        }
    }

    // 下载 Profile
    let downloadResult = await LinklanLPA.shared.downloadProfile(
        device: euiccDevice,
        activeCode: "LPA:1$smdp.example.com$activationCode123",
        confirmCode: nil
    )
    if downloadResult.isSuccess {
        print("下载成功, ICCID: \(downloadResult.data ?? "")")

        // ty 型号下载后会自动启用，可跳过此步
        if euiccDevice.model != "ty" {
            _ = await LinklanLPA.shared.enableProfile(
                device: euiccDevice,
                iccid: downloadResult.data
            )
        }
    }
}
```



## 8. 监听器设置

SDK 提供以下可选监听器，通过 `LinklanLPA.shared` 的公共属性设置：

#### `bleDeviceConnectionListener`

```swift
public var bleDeviceConnectionListener: BleDeviceConnectionListener?
```

**说明：** 蓝牙设备连接状态变化监听器。

#### `userAuthenticationListener`

```swift
public var userAuthenticationListener: BleUserAuthenticationListener?
```

**说明：** 蓝牙用户认证监听器（同步回调，返回密码字符串）。

**示例：**

```swift
// 设置连接状态监听
LinklanLPA.shared.bleDeviceConnectionListener = ConnectionListener()

// 设置用户认证监听
LinklanLPA.shared.userAuthenticationListener = AuthListener()
```

```swift
class ConnectionListener: BleDeviceConnectionListener {
    func onStatusChange(address: String?, status: String) {
        print("蓝牙状态变化 - 地址: \(address ?? "unknown"), 状态: \(status)")
        switch status {
        case "connected":
            print("设备已连接")
        case "disconnected":
            print("设备已断开")
        case "connecting":
            print("正在连接...")
        case "timeout":
            print("连接超时")
        case "error":
            print("连接错误")
        default:
            break
        }
    }
}

class AuthListener: BleUserAuthenticationListener {
    func onPromptUserAuthentication(address: String?) -> String {
        // 在主线程展示 UI 获取用户输入的密码
        // 注意：此方法为同步调用，需在主线程弹出输入框并等待用户确认
        return "user_password"
    }
}
```

---

## 9. 错误码说明

| code | 含义 | 说明 |
|:----:|------|------|
| 0 | 成功 | 操作成功完成，`data` 中包含返回数据 |
| 1 | 失败 | 操作失败，可能原因：设备为空、model 不匹配、设备不支持该功能、地址为空 |

> 建议通过 `msg` 和 `info` 字段获取详细错误信息。使用 `isSuccess` 计算属性判断是否成功。

---

## 10. 完整对接示例

以下示例展示从初始化到扫描 → 连接 → 获取信息 → 下载 Profile → 启用的完整流程：

```swift
import LinklanEuicc
import CoreBluetooth

// MARK: - AppDelegate 初始化

@main
class AppDelegate: UIResponder, UIApplicationDelegate {
    func application(_ application: UIApplication,
                     didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // 1. 初始化 SDK
        LinklanLPA.shared.initSDK(debug: true)
        return true
    }
}

// MARK: - ViewController 扫描与连接

class EuiccViewController: UIViewController {

    private var bleDevice: BleDevice?
    private var euiccDevice: EuiccDevice?

    override func viewDidLoad() {
        super.viewDidLoad()
        setupListeners()
    }

    // 2. 设置监听器
    private func setupListeners() {
        LinklanLPA.shared.bleDeviceConnectionListener = ConnectionListener()
        LinklanLPA.shared.userAuthenticationListener = AuthListener()
    }

    // 3. 开始扫描
    @IBAction func startScanTapped() {
        LinklanLPA.shared.startScan(callback: ScanCallbackImpl(
            onFound: { [weak self] device in
                DispatchQueue.main.async {
                    // 筛选目标设备（按名称或 model）
                    if device.name.contains("Linklan") {
                        LinklanLPA.shared.stopScan()
                        self?.bleDevice = device
                        self?.connectAndManage(device: device)
                    }
                }
            }
        ))
    }

    // 4. 连接并管理设备
    private func connectAndManage(device: BleDevice) {
        Task {
            // 4.1 连接设备
            let connectResult = await LinklanLPA.shared.connectDevice(device: device)
            guard connectResult.isSuccess, let connectedDevice = connectResult.data else {
                print("连接失败: \(connectResult.msg ?? "")")
                return
            }
            self.euiccDevice = connectedDevice
            print("连接成功: \(connectedDevice.name ?? "")")

            // 4.2 获取设备信息
            let infoResult = await LinklanLPA.shared.getDeviceInfo(device: connectedDevice)
            if infoResult.isSuccess, let info = infoResult.data {
                print("EID: \(info.eid ?? "N/A")")
                print("IMEI: \(info.imei ?? "N/A")")
                print("SN: \(info.sn ?? "N/A")")
            }

            // 4.3 获取 Profile 列表
            let profilesResult = await LinklanLPA.shared.getProfileInfoList(device: connectedDevice)
            if profilesResult.isSuccess {
                profilesResult.data?.forEach { profile in
                    print("Profile: \(profile.displayName)")
                    print("  ICCID: \(profile.iccid ?? "")")
                    print("  运营商: \(profile.serviceProviderName ?? "")")
                    print("  状态: \(profile.isEnabled ? "已启用" : "已禁用")")
                    print("  类别: \(profile.classEnum)")
                }
            }

            // 4.4 下载新 Profile
            let downloadResult = await LinklanLPA.shared.downloadProfile(
                device: connectedDevice,
                activeCode: "LPA:1$smdp.example.com$activationCode123",
                confirmCode: nil
            )
            if downloadResult.isSuccess {
                print("下载成功, ICCID: \(downloadResult.data ?? "")")

                // 4.5 启用 Profile（ty 型号下载后会自动启用，可跳过）
                if connectedDevice.model != "ty" {
                    let enableResult = await LinklanLPA.shared.enableProfile(
                        device: connectedDevice,
                        iccid: downloadResult.data
                    )
                    if enableResult.isSuccess {
                        print("启用成功")
                    }
                }
            }

            // 4.6 完成后断开连接
            LinklanLPA.shared.disconnectDevice(connectedDevice)
        }
    }

    deinit {
        LinklanLPA.shared.stopScan()
        LinklanLPA.shared.disconnectDevice(euiccDevice)
    }
}

// MARK: - 扫描回调实现

class ScanCallbackImpl: BleScanCallback {
    let onFound: (BleDevice) -> Void

    init(onFound: @escaping (BleDevice) -> Void) {
        self.onFound = onFound
    }

    func onDeviceFound(device: BleDevice) {
        onFound(device)
    }

    func onScanFailed(errorCode: Int, errorMsg: String) {
        print("扫描失败: [\(errorCode)] \(errorMsg)")
    }
}

// MARK: - 连接状态监听实现

class ConnectionListener: BleDeviceConnectionListener {
    func onStatusChange(address: String?, status: String) {
        print("蓝牙状态: \(status) - \(address ?? "unknown")")
    }
}

// MARK: - 用户认证监听实现

class AuthListener: BleUserAuthenticationListener {
    func onPromptUserAuthentication(address: String?) -> String {
        // 实际项目中此处应弹出输入框获取用户密码
        return "default_password"
    }
}
```

---

## 11. 注意事项

1. **初始化顺序：** 必须先调用 `LinklanLPA.shared.initSDK()` 完成初始化，再使用其他任何 API。

2. **async/await 调用：** 带 `async` 修饰符的方法（`connectDevice`、`getDeviceInfo`、`getProfileInfoList`、`enableProfile`、`disableProfile`、`deleteProfile`、`downloadProfile`、`resetDevice`、`getAPN`、`setApdu`）必须在 `Task` 或 `async` 上下文中调用。不能直接在主线程同步调用。

3. **Info.plist 权限：** 必须在 `Info.plist` 中声明 `NSBluetoothAlwaysUsage`，否则首次调用蓝牙 API 时应用会崩溃。iOS 不会在编译期报错，只在运行时崩溃。

4. **后台模式：** 如需在应用退到后台后维持蓝牙连接，需在 `Info.plist` 中启用 `bluetooth-central` 后台模式。否则应用进入后台后蓝牙连接会断开。

5. **设备 model 路由：** SDK 根据 `BleDevice.model` 或 `EuiccDevice.model` 字段自动路由到对应厂商客户端，调用前确保该字段已正确赋值（扫描时由 SDK 自动填充）。

6. **自动重置行为：** `enableProfile`、`disableProfile`、`downloadProfile` 在操作成功后会自动触发设备重置。重置期间设备蓝牙可能短暂断开，需等待重置完成后再进行后续操作。

7. **蓝牙状态管理：** 在 ViewController 销毁时（`deinit`）调用 `stopScan()` 和 `disconnectDevice()` 释放资源，避免蓝牙泄漏。

8. **单例模式：** `LinklanLPA` 使用 `static let shared` 单例，私有 `init()`，外部无法创建新实例。所有 API 通过 `LinklanLPA.shared` 调用。

9. **同步回调注意：** `BleUserAuthenticationListener.onPromptUserAuthentication(address:)` 是同步回调，SDK 调用时会阻塞当前线程。如需弹出 UI 交互获取用户输入，注意线程切换，避免在主线程以外的线程操作 UI。

10. **iOS 地址格式差异：** iOS 的 `BleDevice.address` 为 UUID 字符串（非 MAC 地址），这是 CoreBluetooth 的限制。连接和后续操作使用此 UUID 作为设备标识。
