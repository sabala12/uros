FUNCTION(LOAD_PROFILE ISA)
    enable_language(ASM_NASM)
    if(CMAKE_ASM_NASM_COMPILER_LOADED)
        SET  (CAN_USE_ASSEMBLER TRUE)
        FILE (GLOB_RECURSE ISA_ASM_FILES "src/arch/${ISA}/boot/*.asm")
        SET  (ISA_ASM_FILES "${ISA_ASM_FILES}" PARENT_SCOPE)
    endif(CMAKE_ASM_NASM_COMPILER_LOADED)

    FILE (GLOB ISA_CXX_FILES "src/arch/${ISA}/*.cpp"
                             "src/arch/${ISA}/*.h"
			     "src/arch/${ISA}/core/*.cpp"
			     "src/arch/${ISA}/core/*.h")

    SET  (ISA_CXX_FILES "${ISA_CXX_FILES}" PARENT_SCOPE)

    SET  (PLATFORM_LAYOUT ${CMAKE_SOURCE_DIR}/src/arch/${ISA}/boot/layout.ld PARENT_SCOPE)

    # TODO load flags from ISA dir
    SET (ISA_SRCS ${ISA_SRCS} PARENT_SCOPE)
	SET (ISA_CXX_FLAGS "" PARENT_SCOPE)
	SET (ISA_ASM_FLAGS "" PARENT_SCOPE)
	SET (ISA_LINK_FLAGS "" PARENT_SCOPE)
ENDFUNCTION(LOAD_PROFILE)
