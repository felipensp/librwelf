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
#include <string.h>

static void inline _copy_sym(int is_dynamic, const rwelf *elf,
	Elf_Sym *sym, size_t n)
{
	sym->elf = elf;

	if (ELF_IS_32(elf)) {
		SYM32(sym) = (is_dynamic ? DYNSYM32(elf) : SYM32(elf)) + n;
	} else if (ELF_IS_64(elf)) {
		SYM64(sym) = (is_dynamic ? DYNSYM64(elf) : SYM64(elf)) + n;
	}
}

/* .symtab symbols */

/**
 * rwelf_get_symbol_by_num(const rwelf*, size_t, Elf_Sym*)
 * Finds the symbol by number and fill the sym param with related symbol
 */
void rwelf_get_symbol_by_num(const rwelf *elf, size_t num, Elf_Sym *sym)
{
	assert(elf != NULL);
	assert(elf->nsyms > num);

	if (sym) {
		_copy_sym(0, elf, sym, num);
	}
}

/**
 * rwelf_get_symbol_by_name(const rwelf *elf, const char *sname, Elf_Sym *sym)
 * Returns the position of the symbol if found, otherwise -1 is returned
 */
int rwelf_get_symbol_by_name(const rwelf *elf, const char *sname,
	Elf_Sym *sym)
{
	int i;

	assert(elf != NULL);
	assert(elf->strtab != NULL);
	assert(sname != NULL);

	for (i = 0; i < elf->nsyms; ++i) {
		const char *name = (char*)(elf->strtab + RWELF_SYM(elf, st_name, i));

		if (name && memcmp(name, sname, strlen(name)+1) == 0) {
			if (sym) {
				_copy_sym(0, elf, sym, i);
			}
			return i;
		}
	}

	return -1;
}

/**
 * rwelf_get_symbol_name(const Elf_Sym*)
 * Returns the symbol name for an specific symbol
 */
const unsigned char *rwelf_get_symbol_name(const Elf_Sym *sym)
{
	assert(sym != NULL);
	assert(sym->elf != NULL);

	return sym->elf->strtab + RWELF_SYM_DATA(sym, st_name);
}

/**
 * rewlf_get_symbol_section(const Elf_Sym*)
 * Returns the section name related to the symbol
 */
const unsigned char *rwelf_get_symbol_section(const Elf_Sym *sym)
{
	Elf_Shdr shdr;

	assert(sym != NULL);
	assert(sym->elf != NULL);

	switch (RWELF_SYM_DATA(sym, st_shndx)) {
		case SHN_ABS:
		case SHN_COMMON:
		case SHN_UNDEF:
			return NULL;
		default:
			rwelf_get_section_by_num(sym->elf, RWELF_SYM_DATA(sym, st_shndx), &shdr);
			break;
	}

	return rwelf_get_section_name(&shdr);
}

/**
 * rwelf_get_symbol_size(const Elf_Sym*)
 * Returns the symbol size
 */
uint64_t rwelf_get_symbol_size(const Elf_Sym *sym)
{
	assert(sym != NULL);
	assert(sym->elf != NULL);

	return RWELF_SYM_DATA(sym, st_size);
}

/**
 * rwelf_get_symbol_value(const Elf_Sym*)
 * Returns the symbol value
 */
uint64_t rwelf_get_symbol_value(const Elf_Sym *sym)
{
	assert(sym != NULL);
	assert(sym->elf != NULL);

	return RWELF_SYM_DATA(sym, st_value);
}

/* .dynsym symbols */

/**
 * rwelf_get_dyn_symbol_by_num()
 */
void rwelf_get_dyn_symbol_by_num(const rwelf *elf, size_t n, Elf_Sym *sym)
{
	assert(elf != NULL);
	assert(elf->dynstr != NULL);

	sym->elf = elf;
	_copy_sym(1, elf, sym, n);
}

/**
 * rwelf_get_symbol_name(const Elf_Sym*)
 * Returns the symbol name for an specific symbol
 */
const unsigned char *rwelf_get_dyn_symbol_name(const Elf_Sym *sym)
{
	assert(sym != NULL);
	assert(sym->elf != NULL);
	assert(sym->elf->dynstr != NULL);

	return sym->elf->dynstr + RWELF_SYM_DATA(sym, st_name);
}
