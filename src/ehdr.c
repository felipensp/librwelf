/**
 * rwelf
 * Copyright (c) 2012-2013 Felipe Pena <felipensp(at)gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "rwelf.h"

/**
 * rwelf_class(const rwelf *)
 * Returns the architecture of the binary
 */
const char *rwelf_class(const rwelf *elf) 
{
	assert(elf != NULL);
	
	switch (ELF_EHDR(elf, e_ident)[EI_CLASS]) {
		case ELFCLASS32:	return "ELF32";
		case ELFCLASS64:	return "ELF64";	
		case ELFCLASSNONE:
		default:			return NULL;
	}
}

/**
 * rwelf_version(const rwelf *)
 * Returns the version number of ELF specification
 */
uint32_t rwelf_version(const rwelf *elf)
{
	assert(elf != NULL);
	
	switch (ELF_EHDR(elf, e_ident)[EI_VERSION]) {
		case EV_CURRENT:	return EV_CURRENT;
		case EV_NONE:
		default:			return 0;
	}
}

/**
 * rwelf_data(const rwelf *)
 * Returns the data encoding of the processor-specific data in the file
 */
const char *rwelf_data(const rwelf *elf)
{
	assert(elf != NULL);
	
	switch (ELF_EHDR(elf, e_ident)[EI_DATA]) {
		case ELFDATA2LSB:	return "2's complement, little-endian";
		case ELFDATA2MSB:	return "2's complement, big-endian";
		case ELFDATANONE:
		default:			return NULL;
	}
}

/**
 * rwelf_type(const rwelf *)
 * Returns the object file type
 */
const char *rwelf_type(const rwelf *elf)
{
	assert(elf != NULL);
	
	switch (ELF_EHDR(elf, e_type)) {
		case ET_REL:	return "REL (relocatable file)";
		case ET_EXEC:	return "EXEC (executable file)";
		case ET_DYN:	return "DYN (shared object)";
		case ET_CORE:	return "CORE (core file)";
		case ET_NONE:
		default:		return NULL;
	}
}

/**
 * rwelf_num_sections(const rwelf *elf)
 * Returns the number of entries on sections header table
 */
uint16_t rwelf_num_sections(const rwelf *elf)
{
	assert(elf != NULL);
	
	return ELF_EHDR(elf, e_shnum);
}

/**
 * rwelf_num_pheader(const rwelf *elf)
 * Returns the number of entries on program header table
 */
uint16_t rwelf_num_pheaders(const rwelf *elf)
{
	assert(elf != NULL);

	return ELF_EHDR(elf, e_phnum);
}

/**
 * rwelf_num_symbols(const rwelf*)
 * Returns the number of symbols in the symbol table
 */
uint16_t rwelf_num_symbols(const rwelf *elf)
{
	assert(elf != NULL);
	
	return elf->nsyms;
}

/**
 * rwelf_num_dyn_symbols(const rwelf*)
 * Returns the number of dynamic symbols in the .dynsym section
 */
uint16_t rwelf_num_dyn_symbols(const rwelf *elf)
{
	assert(elf != NULL);
	
	return elf->ndynsyms;
}

/**
 * rwelf_entry(const rwelf *elf)
 * Returns the virtual address of entry point
 */ 
uint64_t rwelf_entry(const rwelf *elf)
{
	assert(elf != NULL);
	
	return ELF_EHDR(elf, e_entry);
}
