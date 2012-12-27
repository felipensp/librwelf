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

static void inline _copy_phdr(const rwelf *elf, Elf_Phdr *phdr, size_t n)
{
	phdr->elf = elf;

	if (ELF_IS_32(elf)) {
		PHDR32(phdr) = PHDR32(elf) + n;
	} else if (ELF_IS_64(elf)) {
		PHDR64(phdr) = PHDR64(elf) + n;
	}
}

/**
 * rwelf_get_pheader_by_num(const rwelf*, size_t, Elf_Phdr*)
 * Finds a program header by number and fill the out param with the related
 * program header
 */
void rwelf_get_pheader_by_num(const rwelf *elf, size_t num, Elf_Phdr *phdr)
{
	assert(elf != NULL);

	if (phdr) {
		_copy_phdr(elf, phdr, num);
	}
}

/**
 * rwelf_get_pheader_type(const Elf_Phdr*)
 * Returns the program header type
 */
uint32_t rwelf_get_pheader_type(const Elf_Phdr *phdr)
{
	assert(phdr != NULL);
	assert(phdr->elf != NULL);

	return RWELF_PHDR_DATA(phdr, p_type);
}

/**
 * rwelf_get_pheader_flags(const Elf_Phdr*)
 * Returns the program header flags
 */
uint32_t rwelf_get_pheader_flags(const Elf_Phdr *phdr)
{
	assert(phdr != NULL);
	assert(phdr->elf != NULL);

	return RWELF_PHDR_DATA(phdr, p_flags);
}

/**
 * rwelf_get_pheader_vaddr(const Elf_Phdr*)
 * Returns the program header virtual address
 */
uint64_t rwelf_get_pheader_vaddr(const Elf_Phdr *phdr)
{
	assert(phdr != NULL);
	assert(phdr->elf != NULL);

	return RWELF_PHDR_DATA(phdr, p_vaddr);
}

/**
 * rwelf_get_pheader_type_name(const Elf_Phdr*)
 * Returns the program header type as string
 */
const char *rwelf_get_pheader_type_name(const Elf_Phdr *phdr)
{
	assert(phdr != NULL);
	assert(phdr->elf != NULL);

#define CASE(x) case x: return #x
	switch (RWELF_PHDR_DATA(phdr, p_type)) {
		CASE(PT_NULL);
		CASE(PT_LOAD);
		CASE(PT_DYNAMIC);
		CASE(PT_INTERP);
		CASE(PT_NOTE);
		CASE(PT_SHLIB);
		CASE(PT_PHDR);
		CASE(PT_LOPROC);
		CASE(PT_HIPROC);
		CASE(PT_GNU_STACK);
		default:
			return "UNKNOWN";
	}
#undef CASE
}
