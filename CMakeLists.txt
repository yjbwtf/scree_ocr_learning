cmake_minimum_required(VERSION 3.16)
project(screen_ocr_learning VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Allow out-of-source builds
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

find_package(OpenCV REQUIRED)
# Try to find Tesseract (vcpkg or system-installed). If your CMake doesn't provide a Tesseract target,
# you can set Tesseract_LIBRARIES and Tesseract_INCLUDE_DIRS manually or install via vcpkg.
find_package(Tesseract CONFIG QUIET)
if(NOT TARGET Tesseract::libtesseract)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(TESS tesseract)
  if(TESS_FOUND)
    set(Tesseract_LIBRARIES ${TESS_LIBRARIES})
    set(Tesseract_INCLUDE_DIRS ${TESS_INCLUDE_DIRS})
  endif()
endif()

add_executable(screen_ocr_app
  src/main.cpp
  src/screen_capture_gdi.cpp
  src/detector.cpp
  src/overlay_window.cpp
  src/input_simulator.cpp
)

target_include_directories(screen_ocr_app PRIVATE ${OpenCV_INCLUDE_DIRS})
if(Tesseract_INCLUDE_DIRS)
  target_include_directories(screen_ocr_app PRIVATE ${Tesseract_INCLUDE_DIRS})
endif()

target_link_libraries(screen_ocr_app PRIVATE ${OpenCV_LIBS})
if(TARGET Tesseract::libtesseract)
  target_link_libraries(screen_ocr_app PRIVATE Tesseract::libtesseract)
elseif(Tesseract_LIBRARIES)
  target_link_libraries(screen_ocr_app PRIVATE ${Tesseract_LIBRARIES})
endif()

# Compiler warnings and useful defines for Windows
if(MSVC)
  target_compile_options(screen_ocr_app PRIVATE /W4 /permissive-)
else()
  target_compile_options(screen_ocr_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")

# Install target (optional)
install(TARGETS screen_ocr_app RUNTIME DESTINATION bin)