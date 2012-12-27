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

static void inline _copy_dyn(const rwelf *elf, Elf_Dyn *dyn, size_t n)
{
	dyn->elf = elf;

	if (ELF_IS_32(elf)) {
		DYN32(dyn) = DYN32(elf) + n;
	} else if (ELF_IS_64(elf)) {
		DYN64(dyn) = DYN64(elf) + n;
	}
}

/**
 * rwelf_get_dynamic_by_num(const rwelf*, size_t, Elf_Dyn*)
 * Finds the dynamic entry by specified number and fill the out param with
 * the entry
 */
void rwelf_get_dynamic_by_num(const rwelf *elf, size_t num, Elf_Dyn *dyn)
{
	assert(elf != NULL);

	if (dyn) {
		_copy_dyn(elf, dyn, num);
	}
}

/**
 * rwelf_get_dynamic_by_tag(const rwelf*, int64_t, Elf_Dyn*)
 * Finds the .dynamic entry by tag and fill the out param with it when
 * found and return its position, otherwise -1 is returned
 */
int rwelf_get_dynamic_by_tag(const rwelf *elf, int64_t tag, Elf_Dyn *dyn)
{
	int i;

	assert(elf != NULL);

	for (i = 0; i < elf->ndyns; ++i) {
		if (RWELF_DYN(elf, d_tag, i) == tag) {
			if (dyn) {
				_copy_dyn(elf, dyn, i);
			}
			return i;
		}
	}
	return -1;
}

/**
 * rwelf_get_dynamic_strval(const Elf_Dyn*)
 * Returns the string value related to .dynamic entry
 */
const unsigned char *rwelf_get_dynamic_strval(const Elf_Dyn *dyn)
{
	assert(dyn != NULL);
	assert(dyn->elf != NULL);
	assert(dyn->elf->dynstr != NULL);

	switch (RWELF_DYN_DATA(dyn, d_tag)) {
		case DT_SONAME:  /* Shared library name */
		case DT_NEEDED:  /* Needed library */
		case DT_RPATH:   /* RPATH */
		case DT_RUNPATH: /* Library search path */
			return dyn->elf->dynstr + RWELF_DYN_DATA(dyn, d_un.d_val);
		default:
			return NULL;
	}
}

/**
 * rwelf_get_dynamic_tag(const Elf_Dyn*)
 * Returns the .dynamic entry's tag
 */
int64_t rwelf_get_dynamic_tag(const Elf_Dyn *dyn)
{
	assert(dyn != NULL);
	assert(dyn->elf != NULL);

	return RWELF_DYN_DATA(dyn, d_tag);
}

/**
 * rwelf_get_dynamic_tag_name(const Elf_Dyn *dyn)
 * Returns the .dynamic entry's tag as string
 */
const char *rwelf_get_dynamic_tag_name(const Elf_Dyn *dyn)
{
	assert(dyn != NULL);
	assert(dyn->elf != NULL);

#define CASE(x) case x: return #x
	switch (RWELF_DYN_DATA(dyn, d_tag)) {
		CASE(DT_NULL);
		CASE(DT_NEEDED);
		CASE(DT_PLTRELSZ);
		CASE(DT_PLTGOT);
		CASE(DT_HASH);
		CASE(DT_STRTAB);
		CASE(DT_SYMTAB);
		CASE(DT_RELA);
		CASE(DT_RELASZ);
		CASE(DT_RELAENT);
		CASE(DT_STRSZ);
		CASE(DT_SYMENT);
		CASE(DT_INIT);
		CASE(DT_FINI);
		CASE(DT_SONAME);
		CASE(DT_RPATH);
		CASE(DT_SYMBOLIC);
		CASE(DT_REL);
		CASE(DT_RELSZ);
		CASE(DT_RELENT);
		CASE(DT_PLTREL);
		CASE(DT_DEBUG);
		CASE(DT_TEXTREL);
		CASE(DT_JMPREL);
		CASE(DT_BIND_NOW);
		CASE(DT_RUNPATH);
		CASE(DT_LOPROC);
		CASE(DT_HIPROC);
		default:
			return "UNKNOWN";
	}
#undef CASE
}
