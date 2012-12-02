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
 * rwelf_class(rwelf)
 * Returns the architecture of the binary
 */
char *rwelf_class(const rwelf *elf) 
{
	assert(elf != NULL);
	
	switch (elf->header->e_ident[EI_CLASS]) {
		case ELFCLASS32:    return "ELF32";
		case ELFCLASS64:	return "ELF64";	
		case ELFCLASSNONE:
		default:			return NULL;
	}
}

/**
 * rwelf_version(rwelf)
 * Returns the data encoding of the processor-specific data in the file
 */
char *rwelf_version(const rwelf *elf)
{
	assert(elf != NULL);
	
	switch (elf->header->e_ident[EI_VERSION]) {
		case ELFDATA2LSB:	return "2's complement, little-endian";
		case ELFDATA2MSB:	return "2's complement, big-endian";
		case ELFDATANONE:
		default:			return NULL;
	}
}
