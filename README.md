librwelf
========

Library for reading and writing on ELF files


Example:

```C
#include <rwelf.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, num_sections, num_symbols;
	Elf_Shdr sec;
	Elf_Sym sym;
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

	rwelf_close(elf);

	return 0;
}
```
