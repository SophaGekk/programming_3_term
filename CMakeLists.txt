cmake_minimum_required(VERSION 3.10)
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version") 
set(PROJECT_VESRION 0.0.${PATCH_VERSION}) 

project(lab4 VERSION ${PROJECT_VESRION})
add_executable(lab4 main.cpp)

set(CMAKE_CXX_STANDARD 11) # Устанавливаем стандарт C++11
set(CXX_STANDARD_REQUIRED ON) 

# Установка файлов для инсталляции 
install(TARGETS lab4 DESTINATION bin) 

set(CPACK_PROJECT_NAME lab4) 

set(CPACK_GENERATOR "ZIP;DEB") 
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}") 

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}") 
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}") 

set(CPACK_PACKAGE_CONTACT example@example.com) 

include(CPack) 

# enable_testing() # Включает тестирование 

set(CPACK_PROJECT_NAME ${PROJECT_NAME}) 
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})