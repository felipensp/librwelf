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
	Elf_Ehdr ehdr;
	Elf_Shdr sec;
	Elf_Sym sym;
	Elf_Phdr phdr;
	Elf_Dyn dyn;
	rwelf *elf = rwelf_open("./librwelf.so");
	
	if (!elf) {
		exit(1);
	}
	rwelf_get_header(elf, &ehdr);
	
	printf("Class:    %s\n", rwelf_class(&ehdr));
	printf("Data:     %s\n", rwelf_data(&ehdr));
	printf("Version:  %d\n", rwelf_version(&ehdr));
	printf("Type:     %s\n", rwelf_type(&ehdr));
	printf("Sections: %d\n", rwelf_num_sections(&ehdr));
	printf("PHeaders: %d\n", rwelf_num_pheaders(&ehdr));
	printf("Entry:    %p\n", rwelf_entry(&ehdr));
	
	num_sections = rwelf_num_sections(&ehdr);
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

	rwelf_close(elf);

	return 0;
}
```
