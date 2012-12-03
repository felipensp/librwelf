#include "rwelf.h"
#include <stdio.h>

/**
 * rwelf_symbol_name(const rwelf*, size_t)
 * Returns the symbol name for an specific symbol
 */
const unsigned char *rwelf_symbol_name(const rwelf *elf, size_t num)
{
	assert(elf != NULL);
	
	if (elf->sym.symtab == NULL || elf->sym.nsyms < num
		|| elf->sym.symtab[num].st_name == 0) {
		return NULL;
	}
	
	return elf->symstrtab + elf->sym.symtab[num].st_name;
}

/**
 * rwelf_num_symbols(const rwelf*)
 * Returns the number of symbols in the symbol table
 */
size_t rwelf_num_symbols(const rwelf *elf)
{
	assert(elf != NULL);
	
	return elf->sym.nsyms;
}
