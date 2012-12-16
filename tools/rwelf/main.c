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
#include <getopt.h>

/**
 * Displays the ELF header information (-h option)
 */
static void _show_elf_header(const rwelf *elf)
{	
	Elf_Ehdr ehdr;

	rwelf_get_header(elf, &ehdr);
	
	printf("Class:    %s\n", rwelf_class(&ehdr));
	printf("Data:     %s\n", rwelf_data(&ehdr));
	printf("Version:  %d\n", rwelf_version(&ehdr));
	printf("Type:     %s\n", rwelf_type(&ehdr));
	printf("Sections: %d\n", rwelf_num_sections(&ehdr));
	printf("PHeaders: %d\n", rwelf_num_pheaders(&ehdr));
	printf("Entry:    %p\n", rwelf_entry(&ehdr));
}

/**
 * Displays the ELF sections information (-S option)
 */
static void _show_elf_sections(const rwelf *elf)
{
	Elf_Shdr sec;
	Elf_Ehdr ehdr;
	int i, num_sections;

	rwelf_get_header(elf, &ehdr);
	
	num_sections = rwelf_num_sections(&ehdr);
	
    for (i = 0; i < num_sections; ++i) {
        rwelf_get_section_by_num(elf, i, &sec);
        printf("Section: %s\n", rwelf_get_section_name(&sec));
    }    
}

/**
 * Displays the ELF symbols (-s option)
 */
static void _show_elf_symbols(const rwelf *elf)
{
	Elf_Ehdr ehdr;
	Elf_Sym sym;
	int i, num_symbols;
	
	rwelf_get_header(elf, &ehdr);
	
	num_symbols = rwelf_num_symbols(elf);
	for (i = 0; i < num_symbols; ++i) {
		rwelf_get_symbol_by_num(elf, i, &sym);
		printf("Symbol: %s\n", rwelf_get_symbol_name(&sym));
	}
}

/**
 * Displays the ELF relocations (-r option)
 */
static void _show_elf_relocations(const rwelf *elf)
{
	Elf_Ehdr ehdr;
	int i, num_sections;
	
	rwelf_get_header(elf, &ehdr);
	
	num_sections = rwelf_num_sections(&ehdr);
	
	for (i = 0; i < num_sections; ++i) {
		Elf_Shdr sec;
			
		rwelf_get_section_by_num(elf, i, &sec);		
		
		switch (rwelf_get_section_type(&sec)) {
			case SHT_REL:
			case SHT_RELA:
				printf("Relocation entries: %d\n",
					rwelf_get_num_relocs(&sec));
				break;
		}
	}
}

int main(int argc, char **argv)
{
	int action, c, i;
	const char *file;
	Elf_Phdr phdr;
	Elf_Dyn dyn;
	rwelf *elf;
	
	while ((c = getopt(argc, argv, "h:S:s:r:")) != -1) {
		switch (c) {
			case 'h':
			case 'S':
			case 's':
			case 'r':
				file = optarg;
				action = c;
				break;
			default:
				break;
		}		
	}
	
	if (!file) {
		return 0;
	}
	
	if (!(elf = rwelf_open(file))) {
		exit(1);
	}

	switch (action) {
		case 'h':
			_show_elf_header(elf);
			break;
		case 'S':
			_show_elf_sections(elf);
			break;
		case 's':
			_show_elf_symbols(elf);
			break;
		case 'r':
			_show_elf_relocations(elf);
			break;
	}
	
	rwelf_close(elf);	
	
	/*

	
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
	
	if (rwelf_get_dynamic_by_tag(elf, DT_SONAME, &dyn) != -1) {
		printf("%s found, %s!\n",
			rwelf_get_dynamic_tag_name(&dyn),
			rwelf_get_dynamic_strval(&dyn));
	}
	
	num_symbols = rwelf_num_dyn_symbols(elf);	
	for (i = 0; i < num_symbols; ++i) {
		rwelf_get_dyn_symbol_by_num(elf, i, &sym);
		printf("%s [%s]\n",
			rwelf_get_dyn_symbol_name(&sym),
			rwelf_get_symbol_section(&sym));
	}

	
*/
	return 0;
}
