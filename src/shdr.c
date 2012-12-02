#include "rwelf.h"

/**
 * rwelf_section_name(rwelf *elf, size_t shnum)
 * Returns the name of the specified section
 */
const unsigned char *rwelf_section_name(const rwelf *elf, size_t shnum)
{
	assert(elf != NULL);
	assert(elf->sstrtab != NULL);
	assert(elf->shdr != NULL);
	
	return elf->sstrtab + elf->shdr[shnum].sh_name;
}
