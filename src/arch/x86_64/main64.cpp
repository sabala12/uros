#if defined(__linux__)
#error "__linux__ is defined, you are not using a cross-compiler."
#endif

#include <utils/fmt.h>
#include <drivers/vga.h>
#include <arch/x86_64/multiboot_header.h>
#include <arch/x86_64/idt.h>

extern u64 p4_table;

#define bootloader_sect_start(name)	print(#name":")

void print_bootloader_info(void* addr)
{
	struct multiboot_tag *tag;
	for (tag = (struct multiboot_tag *) ((u8*)addr + 8);
	     tag->type != MULTIBOOT_TAG_TYPE_END;
	     tag = (struct multiboot_tag *) ((unsigned char*) tag + ((tag->size + 7) & ~7)))
	{
		switch (tag->type)
		{
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				struct multiboot_tag_string* cmd;
				bootloader_sect_start(command line);

				cmd = (struct multiboot_tag_string*)tag;
				print("cmd=%s",		cmd->string);
				print("");
				break;
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				struct multiboot_tag_string* name;
				bootloader_sect_start(boot load name);

				name = (struct multiboot_tag_string*)tag;
				print("name=%s",	name->string);
				print("");
				break;
			case MULTIBOOT_TAG_TYPE_MODULE:
				struct multiboot_tag_module* module;
				bootloader_sect_start(module);

				module = (struct multiboot_tag_module*)tag;
				print("start=0x%x",	module->mod_start);
				print("end=0x%x",	module->mod_end);
				print("cmd=%s",		module->cmdline);
				print("");
				break;
			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
				struct multiboot_tag_basic_meminfo* mem_info;
				bootloader_sect_start(memory);

				mem_info = (struct multiboot_tag_basic_meminfo*)tag;
				print("mem_lower=%dKB", mem_info->mem_lower);
				print("mem_upper=%dKB", mem_info->mem_upper);
				print("");
				break;
			case MULTIBOOT_TAG_TYPE_BOOTDEV:
				struct multiboot_tag_bootdev* boot_dev;
				bootloader_sect_start(boot device);

				boot_dev = (struct multiboot_tag_bootdev*)tag;
				print("device=0x%x",	boot_dev->biosdev);
				print("slice=0x%x",	boot_dev->slice);
				print("part=0x%x",	boot_dev->part);
				print("");
				break;
			case MULTIBOOT_TAG_TYPE_MMAP: {
				break;
				struct multiboot_tag_mmap* mmap;
				bootloader_sect_start(mmap);

				mmap = (struct multiboot_tag_mmap*)tag;
				multiboot_memory_map_t *sect;
				for (sect = mmap->entries; 
				     (multiboot_uint8_t *) sect < (multiboot_uint8_t *) mmap + mmap->size;
				     sect = (multiboot_memory_map_t *) ((unsigned long) sect + mmap->entry_size)) {

					print("base_addr=0x%x%x",	(unsigned) (sect->addr >> 32), (unsigned) (sect->addr & 0xffffffff));
					print("length=0x%x%x",		(unsigned) (sect->len  >> 32), (unsigned) (sect->len  & 0xffffffff));
					print("type=0x%x",		(unsigned) (sect->type));
				}
				print("");
				break;
			}
		}
	}

}

extern "C" void main64(multiboot_header_tag* mbt)
{
	vga_init();

	print("********************************** UROS **********************************");
	print_bootloader_info(mbt);

	idt_init();
	/* Test interrupts work */
	open_interrupts();
	__asm__ __volatile__("int $2");
	close_interrupts();
}
