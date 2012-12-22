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

/**
 * Helper to access librwelf's struct member
 */
#define RWELF_DATA(_elf, _mem, _class) (_elf)->_mem._class

#define EHDR32(_elf)   RWELF_DATA(_elf, ehdr, _32)
#define EHDR64(_elf)   RWELF_DATA(_elf, ehdr, _64)
#define SHDR32(_elf)   RWELF_DATA(_elf, shdr, _32)
#define SHDR64(_elf)   RWELF_DATA(_elf, shdr, _64)
#define PHDR32(_elf)   RWELF_DATA(_elf, phdr, _32)
#define PHDR64(_elf)   RWELF_DATA(_elf, phdr, _64)
#define SYM32(_elf)    RWELF_DATA(_elf,  sym, _32)
#define SYM64(_elf)    RWELF_DATA(_elf,  sym, _64)
#define DYN32(_elf)    RWELF_DATA(_elf,  dyn, _32)
#define DYN64(_elf)    RWELF_DATA(_elf,  dyn, _64)
#define DYNSYM32(_elf) RWELF_DATA(_elf,  dynsym, _32)
#define DYNSYM64(_elf) RWELF_DATA(_elf,  dynsym, _64)
#define RELA32(_elf)   RWELF_DATA(_elf,  rela, _32)
#define RELA64(_elf)   RWELF_DATA(_elf,  rela, _64)

#define ELF_IS_32(_elf) (_elf->class == ELFCLASS32)
#define ELF_IS_64(_elf) (_elf->class == ELFCLASS64)

/**
 * Helper to access ELF data through rwelf pointer
 */
#define RWELFN(_elf, _type, _field, _n) \
	(ELF_IS_64(_elf) ? (_type##64(_elf)+_n)->_field : (_type##32(_elf)+_n)->_field)

#define RWELF(_elf, _mem, _field) RWELFN(_elf, _mem, _field, 0)
	
#define ELF_EHDR(_elf, _field)     RWELF(_elf,  EHDR, _field)
#define ELF_SHDR(_elf, _field, _n) RWELFN(_elf, SHDR, _field, _n)
#define ELF_PHDR(_elf, _field, _n) RWELFN(_elf, PHDR, _field, _n)
#define ELF_SYM(_elf, _field, _n)  RWELFN(_elf, SYM,  _field, _n)
#define ELF_DYN(_elf, _field, _n)  RWELFN(_elf, DYN,  _field, _n)

/**
 * Helper to access Elf_(Shr, Ehdr, ...) member
 */
#define RWELF2(_var, _type, _field) \
	(ELF_IS_64((_var)->elf) ? _type##64(_var)->_field : _type##32(_var)->_field)

#define EHDR_DATA(_ehdr, _field) RWELF2(_ehdr, EHDR, _field)
#define SHDR_DATA(_shdr, _field) RWELF2(_shdr, SHDR, _field)
#define SYM_DATA(_sym, _field)   RWELF2(_sym,  SYM,  _field)
#define PHDR_DATA(_phdr, _field) RWELF2(_phdr, PHDR, _field)
#define DYN_DATA(_dyn, _field)   RWELF2(_dyn,  DYN,  _field)
#define RELA_DATA(_rela, _field) RWELF2(_rela, RELA, _field)

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

typedef union {
	Elf32_Rela *_32;
	Elf64_Rela *_64;
} rwelf_rela;

typedef struct {
	int fd;
	unsigned char *file;      /* Mapped memory of file */
	size_t size;              /* Size of the file */
	unsigned char class;      /* ELF class 32/64 bit */
	rwelf_ehdr ehdr;	
	rwelf_phdr phdr;	
	rwelf_shdr shdr;                 
	rwelf_sym  sym;           /* .symtab section */
	rwelf_dyn  dyn;           /* .dynamic section */
	rwelf_sym  dynsym;        /* .dynsym section */
	size_t nsyms;             /* Number of symbols on .symtab */
	size_t ndyns;             /* Number of entries on .dynamic */
	size_t ndynsyms;          /* Number of symbols on .dynsym */
	
	unsigned char *dynstr;    /* Dynamic string table (.dynstr) */
	unsigned char *shstrtab;  /* Section name string table (.shstrtab) */
	unsigned char *strtab;    /* Symbol name string table (.strtab) */
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

typedef struct {
	const rwelf *elf;
	rwelf_rela rela;
} Elf_Rela;

/**
 * Functions for handling internal rwelf data
 */
extern rwelf *rwelf_open(const char*);
extern void rwelf_close(rwelf*);
extern uint16_t rwelf_num_symbols(const rwelf*);
extern void rwelf_get_header(const rwelf*, Elf_Ehdr*);

/**
 * ElfN_Ehdr related functions 
 */
extern const char *rwelf_class(const Elf_Ehdr*);
extern const char *rwelf_data(const Elf_Ehdr*);
extern uint32_t rwelf_version(const Elf_Ehdr*);
extern const char *rwelf_type(const Elf_Ehdr*);
extern uint16_t rwelf_num_sections(const Elf_Ehdr*);
extern uint16_t rwelf_num_pheaders(const Elf_Ehdr*);
extern uint64_t rwelf_entry(const Elf_Ehdr*);

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
extern uint64_t rwelf_get_num_entries(const Elf_Shdr*);

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

/**
 * Elf_Rela related functions
 */
extern void rwelf_get_rela_by_num(const Elf_Shdr*, size_t, Elf_Rela*);
extern uint64_t rwelf_get_rela_offset(const Elf_Rela*);
extern uint64_t rwelf_get_rela_info(const Elf_Rela*);
extern int64_t rwelf_get_rela_addend(const Elf_Rela*);
extern uint64_t rwelf_get_rela_type(const Elf_Rela*);
extern const unsigned char *rwelf_get_rela_symbol(const Elf_Rela*);

#endif /* RWELF_H */
