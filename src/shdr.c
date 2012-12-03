#include "rwelf.h"

/**
 * rwelf_section_name(rwelf *elf, size_t shnum)
 * Returns the name of the specified section
 */
const unsigned char *rwelf_section_name(const rwelf *elf, size_t shnum)
{
	assert(elf != NULL);
	assert(elf->shstrtab != NULL);
	assert(elf->shdr != NULL);
	
	return elf->shstrtab + elf->shdr[shnum].sh_name;
}
