file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/iso ${CMAKE_BINARY_DIR}/iso/boot ${CMAKE_BINARY_DIR}/iso/boot/grub)

set(GRUB_CFG   "set timeout=0 \n
				set default=0 \n
				menuentry \"${PROJECT_NAME}\" { \n
				multiboot2 /boot/${PROJECT_NAME}.bin \n
				boot \n }\n")

message(${CMAKE_BINARY_DIR})
file(WRITE ${CMAKE_BINARY_DIR}/iso/boot/grub/grub.cfg "${GRUB_CFG}")

find_program(MAKE_RESCUE grub-mkrescue REQUIRED)

add_custom_target(
	iso # Make target
	COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin ${CMAKE_BINARY_DIR}/iso/boot/${PROJECT_NAME}.bin
	DEPENDS ${PROJECT_NAME}.bin
)

add_custom_command(
    COMMAND ${MAKE_RESCUE}
	ARGS -o ${PROJECT_NAME}.iso ${CMAKE_BINARY_DIR}/iso
	TARGET iso
)