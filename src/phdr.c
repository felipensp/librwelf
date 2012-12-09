#include "rwelf.h"

static void inline _copy_phdr(const rwelf *elf, Elf_Phdr *phdr, size_t n)
{
	phdr->elf = elf;
	
	if (ELF_IS_32(elf)) {
		PHDR32(phdr) = PHDR32(elf) + n;
	} else if (ELF_IS_64(elf)) {
		PHDR64(phdr) = PHDR64(elf) + n;
	}
}

void rwelf_get_pheader_by_num(const rwelf *elf, size_t num, Elf_Phdr *phdr)
{
	assert(elf != NULL);
	
	if (phdr) {
		_copy_phdr(elf, phdr, num);
	}
}
