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

void rwelf_get_rela_by_num(const Elf_Shdr *shdr, size_t n,
	Elf_Rela *rela)
{
	assert(shdr != NULL);

	if (!rela) {
		return;
	}
	rela->elf = shdr->elf;

	if (ELF_IS_32(shdr->elf)) {
		RELA32(rela) = (Elf32_Rela*)(shdr->elf->file +
			RWELF_SHDR_DATA(shdr, sh_offset));
		RELA32(rela) += n;
	} else {
		RELA64(rela) = (Elf64_Rela*)(shdr->elf->file +
			RWELF_SHDR_DATA(shdr, sh_offset));
		RELA64(rela) += n;
	}
}

/**
 * rwelf_get_rela_offset(const Elf_Rela*)
 * Returns the location at which to apply the relocation action
 */
uint64_t rwelf_get_rela_offset(const Elf_Rela *rela)
{
	assert(rela != NULL);

	return RWELF_RELA_DATA(rela, r_offset);
}

/**
 * rwelf_get_rela_info(const Elf_Rela*)
 * Returns the value which gives both the symbol index and type of
 * relocation
 */
uint64_t rwelf_get_rela_info(const Elf_Rela *rela)
{
	assert(rela != NULL);

	return RWELF_RELA_DATA(rela, r_info);
}

/**
 * rwelf_get_rela_addend(const Elf_Rela*)
 * Returns the constant addend used to compute to be stored in the
 * relocatable field
 */
int64_t rwelf_get_rela_addend(const Elf_Rela *rela)
{
	assert(rela != NULL);

	return RWELF_RELA_DATA(rela, r_addend);
}

/**
 * rwelf_get_rela_type(const Elf_Rela*)
 * Returns the relocation type
 */
uint64_t rwelf_get_rela_type(const Elf_Rela *rela)
{
	assert(rela != NULL);

	return ELF_IS_64(rela->elf) ?
		ELF64_R_TYPE(RWELF_RELA_DATA(rela, r_info)) :
		ELF32_R_TYPE(RWELF_RELA_DATA(rela, r_info));
}

/**
 * rwelf_get_rela_symbol(const Elf_Rela*)
 * Returns the symbol name related to the relocation
 */
const unsigned char *rwelf_get_rela_symbol(const Elf_Rela *rela)
{
	Elf_Sym sym;

	assert(rela != NULL);
	assert(rela->elf != NULL);

	/* Read the symbol from .dynsym section + r_info */
	rwelf_get_dyn_symbol_by_num(rela->elf,
		(ELF_IS_64(rela->elf) ?
			ELF64_R_SYM(RWELF_RELA_DATA(rela, r_info)) :
			ELF32_R_SYM(RWELF_RELA_DATA(rela, r_info))),
		&sym);

	/* Get the name from .dynstr */
	return rwelf_get_dyn_symbol_name(&sym);
}
