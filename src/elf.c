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

/**
 * rwelf_open(const char*)
 * Opens a ELF file for reading/writing
 */
rwelf *rwelf_open(const char *fname)
{
	unsigned char *mem;
	struct stat st;
	int fd;
	rwelf *obj;

	if ((fd = open(fname, O_RDONLY)) == -1) {
		return NULL;
	}
	
	fstat(fd, &st);

	mem = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	
	if (mem == MAP_FAILED) {
		close(fd);
		return NULL;
	}
	
	obj = malloc(sizeof(rwelf));
	obj->file = mem;
	obj->fd   = fd;	
	obj->size = st.st_size;
	
	obj->header  = (ElfW(Ehdr)*) obj->file;
	obj->pheader = (ElfW(Phdr)*) (obj->file + obj->header->e_phoff);
	obj->sheader = (ElfW(Shdr)*) (obj->file + obj->header->e_shoff);
	obj->sstrtab = (unsigned char*) (obj->file + 
		obj->sheader[obj->header->e_shstrndx].sh_offset);
	
	return obj;
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
