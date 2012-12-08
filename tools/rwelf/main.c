#include <rwelf.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, num_sections, num_symbols;
	Elf_Shdr dynstr;
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
		Elf_Shdr sec;
		
		rwelf_get_section_by_num(elf, i, &sec);
		printf("Section: %s\n", rwelf_get_section_name(&sec));
	}
	
	num_symbols = rwelf_num_symbols(elf);
	for (i = 0; i < num_symbols; ++i) {
		printf("Symbol: %s\n", rwelf_symbol_name(elf, i));
	}
	
	if (rwelf_get_section_by_name(elf, ".dynstr", &dynstr)) {
		printf("%s found!\n", rwelf_get_section_name(&dynstr));
	}

	rwelf_close(elf);

	return 0;
}
