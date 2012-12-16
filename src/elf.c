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
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/**
 * Finds the string table used for ElfN_Sym and ElfN_Shdr
 */
static void inline _find_str_tables(rwelf *elf)
{
	Elf_Shdr shdr;
	
	/* String table for section names (.shstrtab) */
	elf->shstrtab = elf->file +
		ELF_SHDR(elf, sh_offset, ELF_EHDR(elf, e_shstrndx));

	/* Symbol table */
	if (rwelf_get_section_by_name(elf, ".symtab", &shdr) != -1) {
		if (ELF_IS_64(elf)) {
			SYM64(elf) = (Elf64_Sym*)(elf->file + 
				SHDR_DATA(&shdr, sh_offset));
		} else {
			SYM32(elf) = (Elf32_Sym*)(elf->file +
				SHDR_DATA(&shdr, sh_offset));
		}
		elf->nsyms = rwelf_get_num_entries(&shdr);
	}
	
	/* Symbol name string table */
	if (rwelf_get_section_by_name(elf, ".strtab", &shdr) != -1) {
		elf->strtab = elf->file + SHDR_DATA(&shdr, sh_offset);
	}
	
	/* Dynamic string table */
	if (rwelf_get_section_by_name(elf, ".dynstr", &shdr) != -1) {
		elf->dynstr = elf->file + SHDR_DATA(&shdr, sh_offset);
	}
	
	/* Dynamic symbol table */
	if (rwelf_get_section_by_name(elf, ".dynsym", &shdr) != -1) {
		if (ELF_IS_32(elf)) {
			DYNSYM32(elf) = (Elf32_Sym*)(elf->file +
				SHDR_DATA(&shdr, sh_offset));
		} else {
			DYNSYM64(elf) = (Elf64_Sym*)(elf->file +
				SHDR_DATA(&shdr, sh_offset));
		}
		elf->ndynsyms = rwelf_get_num_entries(&shdr);
	}
	
	/* Dynamic section */
	if (rwelf_get_section_by_name(elf, ".dynamic", &shdr) != -1) {
		if (ELF_IS_32(elf)) {
			DYN32(elf) = (Elf32_Dyn*)(elf->file + 
				SHDR_DATA(&shdr, sh_offset));
		} else {
			DYN64(elf) = (Elf64_Dyn*)(elf->file +
				SHDR_DATA(&shdr, sh_offset));
		}
		elf->ndyns = rwelf_get_num_entries(&shdr);
	}
}

/**
 * Prepares internal data according to ELF's class
 */
static int _prepare_internal_data(rwelf *elf)
{
	elf->class = elf->file[EI_CLASS];
	
	if (ELF_IS_32(elf)) {
		EHDR32(elf) = (Elf32_Ehdr*) elf->file;
		PHDR32(elf) = (Elf32_Phdr*) (elf->file + EHDR32(elf)->e_phoff);
		SHDR32(elf) = (Elf32_Shdr*) (elf->file + EHDR32(elf)->e_shoff);
	} else if (ELF_IS_64(elf)) {
		EHDR64(elf) = (Elf64_Ehdr*) elf->file;
		PHDR64(elf) = (Elf64_Phdr*) (elf->file + EHDR64(elf)->e_phoff);
		SHDR64(elf) = (Elf64_Shdr*) (elf->file + EHDR64(elf)->e_shoff);
	} else {
		return 0;
	}

	_find_str_tables(elf);
	
	return 1;
}

/**
 * rwelf_open(const char*)
 * Opens a ELF file for reading/writing
 */
rwelf *rwelf_open(const char *fname)
{
	unsigned char *mem;
	struct stat st;
	int fd;
	rwelf *elf;

	if ((fd = open(fname, O_RDONLY)) == -1) {
		return NULL;
	}
	
	fstat(fd, &st);

	mem = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	
	if (mem == MAP_FAILED || memcmp(mem, ELFMAG, SELFMAG) != 0) {
		close(fd);
		return NULL;
	}
	
	elf = calloc(1, sizeof(rwelf));
	
	assert(elf != NULL);
	
	elf->file = mem;
	elf->fd   = fd;	
	elf->size = st.st_size;

	if (_prepare_internal_data(elf)) {
		return elf;		
	}
	
	rwelf_close(elf);
	
	return NULL;
}

/**
 * rwelf_get_header(const rwelf*, Elf_Ehdr*)
 * Fill the out param with the ELF header
 */
void rwelf_get_header(const rwelf *elf, Elf_Ehdr *ehdr)
{
	assert(elf != NULL);
	
	ehdr->elf = elf;

	if (ELF_IS_32(elf)) {
		EHDR32(ehdr) = EHDR32(elf);
	} else {
		EHDR64(ehdr) = EHDR64(elf);
	}
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
 * rwelf_close(rwelf *elf)
 * Closes fd and unmap memory related to internal rwelf data
 */
void rwelf_close(rwelf *elf)
{
	assert(elf != NULL);

	if (!elf->file) {
		return;
	}

	munmap(elf->file, elf->size);
	close(elf->fd);	
	free(elf);
}
