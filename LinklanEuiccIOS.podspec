
#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint linklan_euicc.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'LinklanEuiccIOS'
  s.version          = '1.0.1'
  s.summary          = 'A Flutter plugin for Linklan eUICC SDK'
  s.description      = <<-DESC
A Flutter plugin that wraps the Linklan eUICC iOS SDK.
                       DESC
  s.homepage         = 'https://github.com/huangzhangliang/LinklanEuiccIOS'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'huangzhangliang' => '82770536@qq.com' }
  s.source       = {:git => "https://github.com/huangzhangliang/LinklanEuiccIOS.git", :tag => s.version}

#  s.source_files  = "Classes", "Classes/**/*.{h,m}", "Classes/**/*.swift"
#  s.exclude_files = "Classes/Exclude"
  s.vendored_frameworks ='eSIMManagerSDK.xcframework'

end
