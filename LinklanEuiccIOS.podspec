Pod::Spec.new do |s|
  s.name             = 'LinklanEuiccIOS'
  s.version          = '1.0.4'
  s.summary          = 'Linklan eUICC iOS SDK'
  s.description      = <<-DESC
Linklan eUICC iOS SDK, including necessary frameworks for eSIM management.
                       DESC
  s.homepage         = 'https://github.com/huangzhangliang/LinklanEuiccIOS'
  s.license          = { :type => 'MIT' }
  s.author           = { 'huangzhangliang' => '82770536@qq.com' }
  s.source           = { :git => 'https://github.com/huangzhangliang/LinklanEuiccIOS.git', :tag => s.version.to_s }
  # ✅ 只指定 iOS 平台
  s.platform         = :ios, '13.0'
  s.swift_version    = '5.0'

  s.vendored_frameworks = [
    'Frameworks/LinklanEuicc.framework',
    'Frameworks/eSIMManagerSDK.xcframework',
    'Frameworks/TYLPA.framework'
  ]

  s.frameworks = 'CoreBluetooth', 'Foundation', 'UIKit'

  s.pod_target_xcconfig = {
    'DEFINES_MODULE' => 'YES',
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64 x86_64 i386'
  }
end