cmake_minimum_required(VERSION 3.14)
project(ClubeDoLivro CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v5.0.0)   # pode conferir se há versão mais nova no GitHub
FetchContent_MakeAvailable(ftxui)

add_executable(clube_ui src/main_ui.cpp)
target_include_directories(clube_ui PRIVATE include)
target_link_libraries(clube_ui PRIVATE ftxui::screen ftxui::dom ftxui::component)