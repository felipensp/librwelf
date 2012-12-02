librwelf
========

Library for reading and writing on ELF files


Example:

```C
#include <rwelf.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, num_sections;
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
		printf("Section [%s]\n", rwelf_section_name(elf, i));
	}

	rwelf_close(elf);

	return 0;
}
```
