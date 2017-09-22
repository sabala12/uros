cmake_minimum_required(VERSION 3.8)

PROJECT (uros ASM C CXX)
SET (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
INCLUDE (CheckCXXCompilerFlag)
INCLUDE (profile_detection)

# Load architecture specific setting
LOAD_PROFILE ("x86_64")

# Add files to project
FILE (GLOB LIBC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}    "src/libc/*.cpp" "src/libc/*.h")
FILE (GLOB DRIVERS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}     "src/drivers/*.cpp" "src/drivers/*.h")
FILE (GLOB KERNEL_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}      "src/kernel/*.cpp" "src/kernel/*.h")
FILE (GLOB UTILS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}       "src/utils/*.cpp" "src/utils/*.h")

SET  (KERNEL_SOURCES ${DRIVERS_SRC} ${KERNEL_SRC} ${UTILS_SRC})
SET  (SOURCE_FILES ${LIBC_SOURCES} ${KERNEL_SOURCES} ${ISA_ASM_FILES} ${ISA_CXX_FILES})
LIST (SORT SOURCE_FILES)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
)

# Set comelation flags
SET (CXX_FLAGS                      "${ISA_CXX_FLAGS} -Wno-error -fpermissive -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -c")
SET (CMAKE_CXX_FLAGS                "${CXX_FLAGS}")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g -D_DEBUG ${CXX_FLAGS}")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${CXX_FLAGS}")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG ${CXX_FLAGS}")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${CXX_FLAGS}")
SET (CMAKE_ASM_FLAGS                "-felf64")
SET (CMAKE_LINK_FLAGS               "-n --gc-sections -T ${PLATFORM_LAYOUT}")

# Locate toolchain
SET (CROSS_GCC "/home/eran/Dev/cross/x86_64/bin")
find_program(GCC_AR x86_64-elf-ar               REQUIRED PATHS ${CROSS_GCC})
find_program(GCC_LD x86_64-elf-ld               REQUIRED PATHS ${CROSS_GCC})
find_program(GCC_NM x86_64-elf-nm               REQUIRED PATHS ${CROSS_GCC})
find_program(GCC x86_64-elf-gcc                 REQUIRED PATHS ${CROSS_GCC})
find_program(GPP x86_64-elf-g++                 REQUIRED PATHS ${CROSS_GCC})
find_program(GCC_OBJDUMP x86_64-elf-objdump     REQUIRED PATHS ${CROSS_GCC})
find_program(GCC_RANLIB x86_64-elf-ranlib       REQUIRED PATHS ${CROSS_GCC})

# Set toolchain
SET (CMAKE_AR                           "${GCC_AR}")
SET (CMAKE_LINKER                       "${GCC_LD}")
SET (CMAKE_NM                           "${GCC_NM}")
SET (CMAKE_OBJDUMP                      "${GCC_OBJDUMP}")
SET (CMAKE_RANLIB                       "${GCC_RANLIB}")
SET (CMAKE_C_COMPILER                   "${GCC}")
SET (CMAKE_CXX_COMPILER                 "${GPP}")

# Setup nasm compiler
SET (CMAKE_ASM_NASM_COMPILE_OBJECT      "<CMAKE_ASM_NASM_COMPILER> ${CMAKE_ASM_FLAGS} -o <OBJECT> <SOURCE>")
# Setup linker flags
SET (CMAKE_CXX_LINK_EXECUTABLE          "ld ${CMAKE_LINK_FLAGS} -o ${PROJECT_NAME}.bin <OBJECTS>")

# Create target
ADD_EXECUTABLE (${PROJECT_NAME}.bin ${SOURCE_FILES})

INCLUDE (make_iso)
