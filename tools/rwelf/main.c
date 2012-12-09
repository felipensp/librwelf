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

#include <rwelf.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, num_sections, num_symbols;
	Elf_Shdr sec;
	Elf_Sym sym;
	Elf_Phdr phdr;
	Elf_Dyn dyn;
	rwelf *elf = rwelf_open("./librwelf.so");
	
	if (!elf) {
		exit(1);
	}
	printf("Class:    %s\n", rwelf_class(elf));
	printf("Data:     %s\n", rwelf_data(elf));
	printf("Version:  %d\n", rwelf_version(elf));
	printf("Type:     %s\n", rwelf_type(elf));
	printf("Sections: %d\n", rwelf_num_sections(elf));
	printf("PHeaders: %d\n", rwelf_num_pheaders(elf));
	printf("Entry:    %p\n", rwelf_entry(elf));
	
	num_sections = rwelf_num_sections(elf);
	for (i = 0; i < num_sections; ++i) {		
		rwelf_get_section_by_num(elf, i, &sec);
		printf("Section: %s\n", rwelf_get_section_name(&sec));
	}
	
	num_symbols = rwelf_num_symbols(elf);
	for (i = 0; i < num_symbols; ++i) {
		rwelf_get_symbol_by_num(elf, i, &sym);
		printf("Symbol: %s\n", rwelf_get_symbol_name(&sym));
	}
	
	if (rwelf_get_section_by_name(elf, ".dynstr", &sec) != -1) {
		printf("Section %s found!\n", rwelf_get_section_name(&sec));
	}
	
	if (rwelf_get_symbol_by_name(elf, "rwelf_type", &sym) != -1) {
		printf("Symbol %s found, section %s!\n",
			rwelf_get_symbol_name(&sym),
			rwelf_get_symbol_section(&sym));
	}
	
	rwelf_get_pheader_by_num(elf, 2, &phdr);
	printf("%s found!\n", rwelf_get_pheader_type_name(&phdr));
	
	rwelf_get_dynamic_by_num(elf, 1, &dyn);
	printf("%s found!\n", rwelf_get_dynamic_tag_name(&dyn));

	rwelf_close(elf);

	return 0;
}
