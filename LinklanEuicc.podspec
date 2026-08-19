
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
  s.source_files     = 'Frameworks/*'

  s.requires_arc = true

  s.platform         = :ios, '13.0'
  s.swift_version    = '5.0'
end
