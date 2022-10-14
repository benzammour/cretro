cmake_minimum_required(VERSION 3.11)

set(PRODUCT_NAME cretro)
string(TOUPPER ${PRODUCT_NAME} MSG_PREFIX)

project(
    ${PRODUCT_NAME}
    LANGUAGES C
)

set(CMAKE_C_COMPILER gcc)

set(INSTALL_PATH ~/.local/bin/${PRODUCT_NAME})

add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wunused
    -Wshadow
    -Wcast-align
    -Wconversion
    -Wlogical-op
    -Wdouble-promotion
    -fstack-protector
    -Wstack-protector
    -Wredundant-decls
    -Wpacked
    -Wmissing-declarations
    -Wmissing-prototypes
    -Wstrict-prototypes
    -Wno-aggressive-loop-optimizations
    -Wjump-misses-init
    -Wwrite-strings
    -Wcast-qual
    -Wundef
    -Wunsafe-loop-optimizations
    -Wfloat-equal
    -Wtrampolines
    -Warray-bounds=2
    -Wsuggest-attribute=cold
    -Wsuggest-attribute=malloc
    -Wsuggest-attribute=pure
    -Wsuggest-attribute=const
    -Wsuggest-attribute=noreturn
    -Wsuggest-attribute=format
    -Wstrict-overflow=4
    -Wswitch-enum
    -Wswitch-default
    -Wmissing-include-dirs
    -Wformat=2
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR CMAKE_BUILD_TYPE STREQUAL "debug")
    message("[${MSG_PREFIX}] Using -g")
    add_compile_options(-g)
else()
    message("[${MSG_PREFIX}] Using -O3")
    add_compile_options(-O3)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message("[${MSG_PREFIX}] Using clang")
    add_compile_options(
        -Weverything
        -Wno-format-nonliteral
        -ferror-limit=0
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message("[${MSG_PREFIX}] Using gcc")
    add_compile_options(
        ### WARNINGS
        # Standard GCC warnings: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
        -Werror
        -Wlogical-op
        -Wjump-misses-init
        -Wunsafe-loop-optimizations
        -Wtrampolines
        -Wsuggest-attribute=cold
        -Wsuggest-attribute=malloc
        -Wsuggest-attribute=pure
        -Wsuggest-attribute=const
        -Wsuggest-attribute=noreturn
        -Wsuggest-attribute=format
        -Wformat=1
        -fmax-errors=0
    )
endif()

include_directories(src)
set(SOURCE_FILES
    src/cli.c
    src/cpu.c
    src/cretro.c
    src/lcd.c
    src/logging.c
    src/sound.c
    src/timer.c
)

add_executable(
    ${PRODUCT_NAME}
    ${SOURCE_FILES}
)
set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 17)

target_link_libraries(${PRODUCT_NAME} PUBLIC -lm -lSDL2 -pthread)

add_custom_target(install-bin COMMAND /bin/sh -c "cp ${PRODUCT_NAME} ${INSTALL_PATH}\; echo 'Installed ${PRODUCT_NAME} at ${INSTALL_PATH}'" DEPENDS ${PRODUCT_NAME})
add_custom_target(uninstall-bin COMMAND /bin/sh -c "if [[ -f ${INSTALL_PATH} ]]\; then rm ${INSTALL_PATH}\; echo 'Removing ${PRODUCT_NAME} from ${INSTALL_PATH}'\; else echo 'Nothing to uninstall.'\; fi")

