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
static void _show_elf_header(const Elf_Ehdr *ehdr)
{	
	printf("Class:    %s\n", rwelf_class(ehdr));
	printf("Data:     %s\n", rwelf_data(ehdr));
	printf("Version:  %d\n", rwelf_version(ehdr));
	printf("Type:     %s\n", rwelf_type(ehdr));
	printf("Sections: %d\n", rwelf_num_sections(ehdr));
	printf("PHeaders: %d\n", rwelf_num_pheaders(ehdr));
	printf("Entry:    %p\n", (void *) rwelf_entry(ehdr));
}

/**
 * Displays the ELF sections information (-S option)
 */
static void _show_elf_sections(const Elf_Ehdr *ehdr)
{
	const rwelf *elf = ehdr->elf;
	Elf_Shdr sec;
	int i, num_sections;
	
	num_sections = rwelf_num_sections(ehdr);
	
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
	Elf_Sym sym;
	int i, num_symbols;
	
	num_symbols = rwelf_num_symbols(elf);
	for (i = 0; i < num_symbols; ++i) {
		rwelf_get_symbol_by_num(elf, i, &sym);
		printf("Symbol: %s\n", rwelf_get_symbol_name(&sym));
	}
}

/**
 * Displays the Rela information
 */
static void _show_elf_rela(const Elf_Shdr *shdr, size_t n)
{
	int i;
	
	for (i = 0; i < n; ++i) {
		Elf_Rela rela;
		
		rwelf_get_rela_by_num(shdr, i, &rela);
		
		printf("Offset: %012lx | Info: %012lx | Addend: %012lx | Sym: %s\n", 
			rwelf_get_rela_offset(&rela),
			rwelf_get_rela_info(&rela),
			rwelf_get_rela_addend(&rela),
			rwelf_get_rela_symbol(&rela));
	}
}

/**
 * Displays the ELF relocations (-r option)
 */
static void _show_elf_relocations(const Elf_Ehdr *ehdr)
{
	const rwelf *elf = ehdr->elf;
	int i, num_sections;
	
	num_sections = rwelf_num_sections(ehdr);
	
	for (i = 0; i < num_sections; ++i) {
		Elf_Shdr sec;
		size_t n;
			
		rwelf_get_section_by_num(elf, i, &sec);		
		
		switch (rwelf_get_section_type(&sec)) {
			case SHT_REL:
			case SHT_RELA:
				n = rwelf_get_num_entries(&sec);
				printf("Relocation entries: %d\n",(int) n);
					
				_show_elf_rela(&sec, n);
				break;
		}
	}
}

/**
 * Displays the program header information (-l option)
 */
static void _show_elf_pheaders(const Elf_Ehdr *ehdr)
{
	const rwelf *elf = ehdr->elf;
	int i, num_phdrs;
	
	num_phdrs = rwelf_num_pheaders(ehdr);
	
	for (i = 0; i < num_phdrs; ++i) {
		Elf_Phdr phdr;
				
		rwelf_get_pheader_by_num(elf, i, &phdr);
	
		printf("Type: %s\n", rwelf_get_pheader_type_name(&phdr));
	}
}

int main(int argc, char **argv)
{
	int action, c, i;
	const char *file;
	Elf_Ehdr ehdr;
	rwelf *elf;
	
	while ((c = getopt(argc, argv, "h:l:S:s:r:")) != -1) {
		switch (c) {
			case 'h': /* Header */
			case 'l': /* Program header */
			case 'r': /* Relocation */
			case 'S': /* Sections */
			case 's': /* Symbol table */
				file = optarg;
				action = c;
				break;
			default:
				break;
		}		
	}
	
	if (!file || !c) {
	  printf("You have to specific options and an ELF file.\n");
	  printf("Eg. rwelf -h /bin/ls\n");
	  return 0;
	}
	
	if (!(elf = rwelf_open(file))) {
		exit(1);
	}
	rwelf_get_header(elf, &ehdr);

	switch (action) {
		case 'l':
			_show_elf_pheaders(&ehdr);
			break;
		case 'h':
			_show_elf_header(&ehdr);
			break;
		case 'r':
			_show_elf_relocations(&ehdr);
			break;
		case 'S':
			_show_elf_sections(&ehdr);
			break;
		case 's':
			_show_elf_symbols(elf);
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
