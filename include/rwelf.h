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

#ifndef RWELF_H
#define RWELF_H

#include <assert.h>
#include <elf.h>
#include <stdint.h>
#include <link.h>

#define LIBRWELF_VERSION 100
#define LIBRWELF_VERSION_STR  "0.1.0"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define RWELF_DATA(_elf, _mem, _class) (_elf)->_mem._class

#define EHDR32(_elf) RWELF_DATA(_elf, ehdr, _32)
#define EHDR64(_elf) RWELF_DATA(_elf, ehdr, _64)
#define SHDR32(_elf) RWELF_DATA(_elf, shdr, _32)
#define SHDR64(_elf) RWELF_DATA(_elf, shdr, _64)
#define PHDR32(_elf) RWELF_DATA(_elf, phdr, _32)
#define PHDR64(_elf) RWELF_DATA(_elf, phdr, _64)
#define SYM32(_elf)  RWELF_DATA(_elf,  sym, _32)
#define SYM64(_elf)  RWELF_DATA(_elf,  sym, _64)
#define DYN32(_elf)  RWELF_DATA(_elf,  dyn, _32)
#define DYN64(_elf)  RWELF_DATA(_elf,  dyn, _64)
#define DYNSYM32(_elf)  RWELF_DATA(_elf,  dynsym, _32)
#define DYNSYM64(_elf)  RWELF_DATA(_elf,  dynsym, _64)

#define ELF_IS_32(_elf) (_elf->class == ELFCLASS32)
#define ELF_IS_64(_elf) (_elf->class == ELFCLASS64)

#define RWELFN(_elf, _mem, _field, _n) \
	(ELF_IS_64(_elf) ? (RWELF_DATA(_elf, _mem, _64)+_n)->_field : (RWELF_DATA(_elf, _mem, _64)+_n)->_field)

#define RWELF(_elf, _mem, _field) RWELFN(_elf, _mem, _field, 0)
	
#define ELF_EHDR(_elf, _field)     RWELF(_elf,  ehdr, _field)
#define ELF_SHDR(_elf, _field, _n) RWELFN(_elf, shdr, _field, _n)
#define ELF_PHDR(_elf, _field, _n) RWELFN(_elf, phdr, _field, _n)
#define ELF_SYM(_elf,  _field, _n) RWELFN(_elf,  sym, _field, _n)
#define ELF_DYN(_elf, _field, _n)  RWELFN(_elf,  dyn, _field, _n)

#define SHDR_DATA(_shdr, _field) \
	(ELF_IS_64((_shdr)->elf) ? SHDR64(_shdr)->_field : SHDR32(_shdr)->_field)

#define SYM_DATA(_sym, _field) \
	(ELF_IS_64((_sym)->elf) ? SYM64(_sym)->_field : SYM32(_sym)->_field)
	
#define PHDR_DATA(_sym, _field) \
	(ELF_IS_64((_sym)->elf) ? PHDR64(_sym)->_field : PHDR32(_sym)->_field)
	
#define DYN_DATA(_dyn, _field) \
	(ELF_IS_64((_dyn)->elf) ? DYN64(_dyn)->_field : DYN32(_dyn)->_field)

typedef union {
	Elf32_Shdr *_32;
	Elf64_Shdr *_64;
} rwelf_shdr;

typedef union {
	Elf32_Phdr *_32;
	Elf64_Phdr *_64;
} rwelf_phdr;

typedef union {
	Elf32_Ehdr *_32;
	Elf64_Ehdr *_64;
} rwelf_ehdr;

typedef union {                   
	Elf32_Sym *_32;
	Elf64_Sym *_64;
} rwelf_sym;

typedef union {
	Elf32_Dyn *_32;
	Elf64_Dyn *_64;
} rwelf_dyn;

typedef struct {
	int fd;
	unsigned char *file;      /* Mapped memory of file */
	size_t size;              /* Size of the file */
	unsigned char class;      /* ELF class 32/64 bit */
	rwelf_ehdr ehdr;	
	rwelf_phdr phdr;	
	rwelf_shdr shdr;                 
	rwelf_sym   sym;          /* .symtab section */
	rwelf_dyn   dyn;          /* .dynamic section */
	rwelf_sym  dynsym;        /* .dynsym section */
	size_t nsyms;             /* Number of symbols on .symtab */
	size_t ndyns;             /* Number of entries on .dynamic */
	size_t ndynsyms;          /* Number of symbols on .dynsym */
	
	unsigned char *dynstrtab; /* Dynamic string table (.dynstr) */
	unsigned char *shstrtab;  /* Section name string table (.shstrtab) */
	unsigned char *symstrtab; /* Symbol name string table (.strtab) */
} rwelf;

/**
 * ElfN_[ESP]hdr class independent-version 
 */
typedef struct {
	const rwelf *elf;
	rwelf_shdr shdr;
} Elf_Shdr;

typedef struct {
	const rwelf *elf;
	rwelf_phdr phdr;
} Elf_Phdr;

typedef struct {
	const rwelf *elf;
	rwelf_ehdr ehdr;
} Elf_Ehdr;

typedef struct {
	const rwelf *elf;
	rwelf_sym sym;
} Elf_Sym;

typedef struct {
	const rwelf *elf;
	rwelf_dyn dyn;
} Elf_Dyn;

/**
 * Functions for handling internal rwelf data
 */
extern rwelf *rwelf_open(const char*);
extern void rwelf_close(rwelf*);

/**
 * ElfN_Ehdr related functions 
 */
extern const char *rwelf_class(const rwelf*);
extern const char *rwelf_data(const rwelf*);
extern uint32_t rwelf_version(const rwelf*);
extern const char *rwelf_type(const rwelf*);
extern uint16_t rwelf_num_sections(const rwelf*);
extern uint16_t rwelf_num_pheaders(const rwelf*);
extern uint16_t rwelf_num_symbols(const rwelf*);
extern uint64_t rwelf_entry(const rwelf*);

/**
 * Elf_Shdr related functions
 */
extern void rwelf_get_section_by_num(const rwelf*, size_t, Elf_Shdr*);
extern int rwelf_get_section_by_name(const rwelf*, const char*, Elf_Shdr*);
extern const unsigned char *rwelf_get_section_name(const Elf_Shdr*);
extern uint32_t rwelf_get_section_type(const Elf_Shdr*);
extern uint64_t rwelf_get_section_flags(const Elf_Shdr*);
extern uint64_t rwelf_get_section_addr(const Elf_Shdr*);
extern uint64_t rwelf_get_section_size(const Elf_Shdr*);

/**
 * Elf_Phdr related functions
 */
extern void rwelf_get_pheader_by_num(const rwelf*, size_t, Elf_Phdr*);
extern uint32_t rwelf_get_pheader_type(const Elf_Phdr*);
extern uint32_t rwelf_get_pheader_flags(const Elf_Phdr*);
extern uint64_t rwelf_get_pheader_vaddr(const Elf_Phdr*);
extern const char *rwelf_get_pheader_type_name(const Elf_Phdr*);

/**
 * Elf_Sym related functions
 */
extern void rwelf_get_symbol_by_num(const rwelf*, size_t, Elf_Sym*);
extern int rwelf_get_symbol_by_name(const rwelf*, const char*, Elf_Sym*);
extern const unsigned char *rwelf_get_symbol_name(const Elf_Sym*);
extern const unsigned char *rwelf_get_symbol_section(const Elf_Sym*);
extern uint64_t rwelf_get_symbol_size(const Elf_Sym*);
extern uint64_t rwelf_get_symbol_value(const Elf_Sym*);
extern uint16_t rwelf_num_dyn_symbols(const rwelf*);

extern void rwelf_get_dyn_symbol_by_num(const rwelf*, size_t, Elf_Sym*);
extern const unsigned char *rwelf_get_dyn_symbol_name(const Elf_Sym*);

/**
 * Elf_Dyn related functions
 */
extern void rwelf_get_dynamic_by_num(const rwelf*, size_t, Elf_Dyn*);
extern int64_t rwelf_get_dynamic_tag(const Elf_Dyn*);
extern const char *rwelf_get_dynamic_tag_name(const Elf_Dyn*);
extern const unsigned char *rwelf_get_dynamic_strval(const Elf_Dyn*);
extern int rwelf_get_dynamic_by_tag(const rwelf*, int64_t, Elf_Dyn*);

#endif /* RWELF_H */
