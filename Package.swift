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
            targets: ["LinklanEuicc"]
        ),
    ],
    targets: [
        .binaryTarget(
            name: "LinklanEuicc",
            path: "Frameworks/LinklanEuicc.framework"
        ),
        .binaryTarget(
            name: "eSIMManagerSDK",
            path: "Frameworks/eSIMManagerSDK.xcframework"
        ),
        .binaryTarget(
            name: "TYLPA",
            path: "Frameworks/TYLPA.framework"
        ),
    ]
)