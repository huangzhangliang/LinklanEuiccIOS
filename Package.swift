// swift-tools-version:5.7
import PackageDescription

let package = Package(
    name: "LinklanEuiccIOS",
    platforms: [
        .iOS(.v13)
    ],
    products: [
        .library(
            name: "LinklanEuiccIOS",
            targets: ["LinklanEuiccIOS"]
        ),
    ],
    dependencies: [
        .package(path: "Frameworks/eSIMManagerSDK.xcframework"),
        .package(path: "Frameworks/TYLPA.framework")
    ],
    targets: [
        .binaryTarget(
            name: "LinklanEuiccIOS",
            path: "Frameworks/LinklanEuicc.framework"
        )
    ]
)