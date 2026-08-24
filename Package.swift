// swift-tools-version:5.7
import PackageDescription

let package = Package(
    name: "LinklanEuiccIOS",
    platforms: [.iOS(.v13)],
    products: [
        .library(
            name: "LinklanEuiccIOS",
            targets: ["LinklanEuicc", "eSIMManagerSDK", "TYLPA"]
        )
    ],
    dependencies: [
        .package(url: "https://github.com/Moya/Moya.git", from: "15.0.0"),
        .package(url: "https://github.com/ReactiveX/RxSwift.git", from: "6.5.0"),
        .package(url: "https://github.com/ashleymills/GestureRecognizerClosures.git", from: "5.0.0"),
    ],
    targets: [
        .binaryTarget(
            name: "LinklanEuicc",
            path: "Frameworks/LinklanEuicc.xcframework"
        ),
        .binaryTarget(
            name: "eSIMManagerSDK",
            path: "Frameworks/eSIMManagerSDK.xcframework"
        ),
        .binaryTarget(
            name: "TYLPA",
            path: "Frameworks/TYLPA.xcframework"
        )
    ]
)
