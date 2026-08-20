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
            targets: ["LinklanEuicc", "eSIMManagerSDK", "TYLPA"]
        ),
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
        ),
    ]
)
