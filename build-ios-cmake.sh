#!/bin/bash
#
# Filename:     build-ios.sh
# Simple iOS build script for FreeType
#

LIBNAME=libfreetype
pwd=$(pwd)

# 读取参数
BUILD_TYPE=${1:-Release}
XCODE_NAME=${2:-"Xcode"}
IOS_SDK_VERSION=${3:-"18.5"}

buildit_cmake()
{
    ios_platform=$1
    arch=$2
    desc=$3

    echo "Building $desc..."

    # Determine platform name for output directory
    if [[ "$ios_platform" == "OS" ]]; then
        PLATFORM_NAME="iPhoneOS"
    else
        PLATFORM_NAME="iPhoneSimulator"
    fi

    BUILD_DIR="$pwd/build_${arch}_${PLATFORM_NAME}"
    rm -rf "$BUILD_DIR"
    export CXXFLAGS="-std=c++23"

    cmake -S . -B "$BUILD_DIR" -G Xcode \
        -DCMAKE_TOOLCHAIN_FILE="$pwd/builds/cmake/iOS.cmake" \
        -DIOS_PLATFORM="$ios_platform" \
        -DCMAKE_IOS_DEVELOPER_ROOT="/Applications/$XCODE_NAME.app/Contents/Developer/Platforms/$PLATFORM_NAME.platform/Developer" \
        -DCMAKE_IOS_SDK_ROOT="/Applications/$XCODE_NAME.app/Contents/Developer/Platforms/$PLATFORM_NAME.platform/Developer/SDKs/$PLATFORM_NAME.sdk" \
        -DCMAKE_OSX_ARCHITECTURES="$arch" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_INSTALL_PREFIX="$pwd/output/$arch/$PLATFORM_NAME" \
        -DFT_DISABLE_ZLIB=TRUE \
        -DFT_DISABLE_BZIP2=TRUE \
        -DFT_DISABLE_PNG=TRUE \
        -DFT_DISABLE_HARFBUZZ=TRUE \
        -DFT_DISABLE_BROTLI=TRUE || exit 1

    cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" || exit 1
    cmake --build "$BUILD_DIR" --target install --config "$BUILD_TYPE" || exit 1

    echo "✅ $desc completed"
}

# Clean
rm -rf build_* output/

# Build
buildit_cmake "OS" "arm64" "iOS Device (ARM64)"
buildit_cmake "SIMULATOR" "arm64" "iOS Simulator (ARM64)"
buildit_cmake "SIMULATOR" "x86_64" "iOS Simulator (x86_64)"

# Merge libraries
echo "Creating universal library..."
DEVICE_LIB="$pwd/output/arm64/iPhoneOS/lib/$LIBNAME.a"
SIM_LIB="$pwd/output/x86_64/iPhoneSimulator/lib/$LIBNAME.a"

LIPO=$(xcrun -sdk iphoneos -find lipo)
mkdir -p "$pwd/output"

if [ -f "$DEVICE_LIB" ] && [ -f "$SIM_LIB" ]; then
    $LIPO -create "$DEVICE_LIB" "$SIM_LIB" -output "$pwd/output/$LIBNAME.a"
    echo "✅ Universal library created: $pwd/output/$LIBNAME.a"
    lipo -info "$pwd/output/$LIBNAME.a"
else
    echo "❌ Libraries not found"
    exit 1
fi

echo "🎉 Build completed!"