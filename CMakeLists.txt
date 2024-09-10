cmake_minimum_required(VERSION 3.10) # Устанавливает минимальную версию CMake, необходимую для этого проекта (в данном случае 3.10).

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
# Создает переменные PATCH_VERSION и PROJECT_VESRION для управления версиями проекта.

project(helloworld VERSION ${PROJECT_VESRION}) # Определяет имя проекта helloworld и использует переменную PROJECT_VESRION для указания версии проекта.

find_package(Boost COMPONENTS unit_test_framework REQUIRED) 
# Ищет Boost в вашей системе. 
#   - COMPONENTS unit_test_framework: Указывает, что вам нужна компонента unit_test_framework из Boost.
#   - REQUIRED: Если Boost не найден, CMake прекратит работу с ошибкой.

# Указание пути к Boost
# set(Boost_DIR "C:/Users/Sopha/Downloads/boost_1_86_0")
# find_package(Boost COMPONENTS unit_test_framework REQUIRED PATHS ${Boost_DIR})

configure_file(version.h.in version.h) # Создает файл version.h из шаблона version.h.in, заменяя в нем специальные переменные, например, имя проекта и версию.

add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp)
add_executable(test_version test_version.cpp)
# add_executable: Создает исполняемые файлы helloworld_cli и test_version.
# add_library: Создает статическую или динамическую библиотеку helloworld (в данном случае, вероятно, статическую).

set_target_properties(helloworld_cli helloworld test_version PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
#  set_target_properties: Устанавливает свойства для целей helloworld_cli, helloworld, и test_version:
# - CXX_STANDARD 14: Указывает, что используется стандарт C++14.
# - CXX_STANDARD_REQUIRED ON: Требует использование стандартного C++14.

target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)
# Добавляет директорию CMAKE_BINARY_DIR (директория, в которой CMake создает свои файлы) в список директорий, которые следует искать при включении заголовочных файлов в helloworld.

set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)
# set_target_properties: Устанавливает свойства для test_version:
# - COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK: Определяет макрос BOOST_TEST_DYN_LINK, чтобы Boost.Test использовал динамическую связь.
# - INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}: Добавляет директорию с заголовочными файлами Boost в список директорий, которые следует искать при включении заголовочных файлов в test_version.

target_link_libraries(helloworld_cli PRIVATE
    helloworld
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    helloworld
)
# target_link_libraries: Соединяет цели с необходимыми библиотеками:
#   - helloworld_cli связывается с helloworld.
#   - test_version связывается с Boost_LIBRARIES (библиотеки Boost.Test) и helloworld.


if (MSVC) # Устанавливает флаги компиляции в зависимости от компилятора
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
    target_compile_options(helloworld PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(helloworld_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS helloworld_cli RUNTIME DESTINATION bin) # Устанавливает исполняемые файлы в директорию bin после успешной сборки.
# Настройка для создания пакетов с помощью CPack

# Установка пакета DEB для Linux
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR DEB) # Настраивает CPack для создания пакетов DEB (Linux).
endif()

# Установка пакета ZIP для macOS
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CPACK_GENERATOR ZIP) # Настройка CPack для создания пакетов ZIP (macOS)
endif()

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing() # Включает тестирование

add_test(test_version test_version) # Добавляет тест test_version
