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

static void inline _copy_shdr(const rwelf *elf, Elf_Shdr *shdr, size_t n)
{
	shdr->elf = elf;

	if (ELF_IS_32(elf)) {
		SHDR32(shdr) = SHDR32(elf) + n;
	} else if (ELF_IS_64(elf)) {
		SHDR64(shdr) = SHDR64(elf) + n;
	}
}

/**
 * rwelf_get_section_by_name(const rwelf *elf, const char *sname, Elf_Shdr *shdr)
 * Returns the number of the section when the section is found,
 * otherwise -1 is returned. When shdr is supplied, it will be filled with
 * reference to the section.
 */
int rwelf_get_section_by_name(const rwelf *elf, const char *sname, Elf_Shdr *shdr)
{
	int i;
	size_t len;

	assert(elf != NULL);
	assert(elf->shstrtab != NULL);
	assert(sname != NULL);

	len = strlen(sname);

	for (i = 0; i < RWELF_EHDR(elf, e_shnum); ++i) {
		const char *name = (char*)(elf->shstrtab + RWELF_SHDR(elf, sh_name, i));

		if (name && memcmp(name, sname, len+1) == 0) {
			if (shdr) {
				_copy_shdr(elf, shdr, i);
			}
			return i;
		}
	}
	return -1;
}

/**
 * rwelf_section_by_num(const rwelf *elf, size_t num, Elf_Shdr *shdr)
 * Finds the section by number and fills the shdr with the related
 * section
 */
void rwelf_get_section_by_num(const rwelf *elf,
	size_t num, Elf_Shdr *shdr)
{
	assert(elf != NULL);
	assert(elf->shstrtab != NULL);
	assert(RWELF_EHDR(elf, e_shnum) > num);

	if (shdr) {
		_copy_shdr(elf, shdr, num);
	}
}

/**
 * rwelf_get_section_name(const Elf_Shdr *shdr)
 * Returns the name of section
 */
const unsigned char *rwelf_get_section_name(const Elf_Shdr *shdr)
{
	assert(shdr != NULL);
	assert(shdr->elf != NULL);

	return shdr->elf->shstrtab + RWELF_SHDR_DATA(shdr, sh_name);
}

/**
 * rwelf_get_section_type(const Elf_Shdr*)
 * Returns the section type
 */
uint32_t rwelf_get_section_type(const Elf_Shdr *shdr)
{
	assert(shdr != NULL);
	assert(shdr->elf != NULL);

	return RWELF_SHDR_DATA(shdr, sh_type);
}

/**
 * rwelf_get_section_flags(const Elf_Shdr*)
 * Returns the section flags
 */
uint64_t rwelf_get_section_flags(const Elf_Shdr *shdr)
{
	assert(shdr != NULL);
	assert(shdr->elf != NULL);

	return RWELF_SHDR_DATA(shdr, sh_flags);
}

/**
 * rwelf_get_section_addr(const Elf_Shdr*)
 * Returns the section address if it appears in the memory image of
 * process
 */
uint64_t rwelf_get_section_addr(const Elf_Shdr *shdr)
{
	assert(shdr != NULL);
	assert(shdr->elf != NULL);

	return RWELF_SHDR_DATA(shdr, sh_addr);
}

/**
 * rwelf_get_section_size(const Elf_Shdr*)
 * Returns the section size in bytes
 */
uint64_t rwelf_get_section_size(const Elf_Shdr *shdr)
{
	assert(shdr != NULL);
	assert(shdr->elf != NULL);

	return RWELF_SHDR_DATA(shdr, sh_size);
}

/**
 * rwelf_get_num_entries(const Elf_Shdr*)
 * Returns the number of entries
 */
uint64_t rwelf_get_num_entries(const Elf_Shdr *shdr)
{
	assert(shdr != NULL);
	assert(shdr->elf != NULL);

	return RWELF_SHDR_DATA(shdr, sh_size) / RWELF_SHDR_DATA(shdr, sh_entsize);
}

