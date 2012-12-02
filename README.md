librwelf
========

Library for reading and writing on ELF files


Example:

```C
#include <rwelf.h>
#include <stdio.h>

int main(int argc, char **argv) {
	rwelf *elf = rwelf_open("./librwelf.so");
	
	if (elf) {
		printf("Class: %s\n", rwelf_class(elf));
		
		rwelf_close(elf);
	}

	return 0;
}
```
