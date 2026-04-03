依赖与安装（使用 vcpkg 推荐）

本项目依赖：
- OpenCV (>=4.5)
- Tesseract OCR (>=4.1)
- leptonica（作为 Tesseract 的依赖）

推荐使用 vcpkg 管理依赖：
1. 克隆并安装 vcpkg：
   git clone https://github.com/microsoft/vcpkg.git
   ./vcpkg/bootstrap-vcpkg.sh   （Linux/macOS）
   .\vcpkg\bootstrap-vcpkg.bat （Windows）

2. 安装依赖包（示例）：
   ./vcpkg/vcpkg install opencv[tesseract]:x64-windows
   ./vcpkg/vcpkg install tesseract:x64-windows
   或在 Linux/macOS （示例）：
   ./vcpkg/vcpkg install opencv tesseract

3. 将 vcpkg 集成到 CMake：
   在构建时传入 -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

构建示例：
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

注意事项：
- Windows 上可能需要预先安装 Visual Studio（含 C++ 工具）。
- 若使用系统包管理器（apt / brew）安装，请确保 OpenCV 与 Tesseract 能被 CMake 找到。
- 若你需要我也能把 vcpkg.json 或者工具链配置添加到仓库，请回复我。

以上。