#ifndef __LUCHO_H__
#define __LUCHO_H__

#include <mach-o/loader.h>
#include <stdio.h>
#include <stdlib.h>

// colors
#define RED_BOLD "\033[1;31m"
#define WHITE_BOLD "\033[1;37m"
#define RESET "\033[0m"

// macros
#define ALLOC(type, count) (type*)malloc(sizeof(type) * (count))

// CONSTANT DEFINITION START
/*
 * After MacOS X 10.1 when a new load command is added that is required to be
 * understood by the dynamic linker for the image to execute properly the
 * LC_REQ_DYLD bit will be or'ed into the load command constant.  If the dynamic
 * linker sees such a load command it it does not understand will issue a
 * "unknown load command required for execution" error and refuse to use the
 * image.  Other load commands without this bit that are not understood will
 * simply be ignored.
 */
#define LC_REQ_DYLD 0x80000000

/* Constants for the cmd field of all load commands, the type */
#define LC_SEGMENT 0x1 /* segment of this file to be mapped */
#define LC_SYMTAB 0x2 /* link-edit stab symbol table info */
#define LC_SYMSEG 0x3 /* link-edit gdb symbol table info (obsolete) */
#define LC_THREAD 0x4 /* thread */
#define LC_UNIXTHREAD 0x5 /* unix thread (includes a stack) */
#define LC_LOADFVMLIB 0x6 /* load a specified fixed VM shared library */
#define LC_IDFVMLIB 0x7 /* fixed VM shared library identification */
#define LC_IDENT 0x8 /* object identification info (obsolete) */
#define LC_FVMFILE 0x9 /* fixed VM file inclusion (internal use) */
#define LC_PREPAGE 0xa /* prepage command (internal use) */
#define LC_DYSYMTAB 0xb /* dynamic link-edit symbol table info */
#define LC_LOAD_DYLIB 0xc /* load a dynamically linked shared library */
#define LC_ID_DYLIB 0xd /* dynamically linked shared lib ident */
#define LC_LOAD_DYLINKER 0xe /* load a dynamic linker */
#define LC_ID_DYLINKER 0xf /* dynamic linker identification */
#define LC_PREBOUND_DYLIB 0x10 /* modules prebound for a dynamically */
/*  linked shared library */
#define LC_ROUTINES 0x11 /* image routines */
#define LC_SUB_FRAMEWORK 0x12 /* sub framework */
#define LC_SUB_UMBRELLA 0x13 /* sub umbrella */
#define LC_SUB_CLIENT 0x14 /* sub client */
#define LC_SUB_LIBRARY 0x15 /* sub library */
#define LC_TWOLEVEL_HINTS 0x16 /* two-level namespace lookup hints */
#define LC_PREBIND_CKSUM 0x17 /* prebind checksum */

// END CONSTANTS DEFINITIONS

struct nlist_64_t {
    uint32_t n_strx; /* index into the string table */
    uint8_t n_type; /* type flag, see below */
    uint8_t n_sect; /* section number or NO_SECT */
    uint16_t n_desc; /* see <mach-o/stab.h> */
    uint64_t n_value; /* value of this symbol (or stab offset) */
};

struct dysymtab_command_t {
    uint32_t ilocalsym; /* index to local symbols */
    uint32_t nlocalsym; /* number of local symbols */
    uint32_t iextdefsym; /* index to externally defined symbols */
    uint32_t nextdefsym; /* number of externally defined symbols */
    uint32_t iundefsym; /* index to undefined symbols */
    uint32_t nundefsym; /* number of undefined symbols */
    uint32_t tocoff; /* file offset to table of contents */
    uint32_t ntoc; /* number of entries in table of contents */
    uint32_t modtaboff; /* file offset to module table */
    uint32_t nmodtab; /* number of module table entries */
    uint32_t extrefsymoff; /* offset to referenced symbol table */
    uint32_t nextrefsyms; /* number of referenced symbol table entries */
    uint32_t indirectsymoff; /* file offset to the indirect symbol table */
    uint32_t nindirectsyms; /* number of indirect symbol table entries */
    uint32_t extreloff; /* offset to external relocation entries */
    uint32_t nextrel; /* number of external relocation entries */
    uint32_t locreloff; /* offset to local relocation entries */
    uint32_t nlocrel; /* number of local relocation entries */
};

struct symtab_command_t {
    uint32_t symoff; /* symbol table offset */
    uint32_t nsyms; /* number of symbol table entries */
    uint32_t stroff; /* string table offset */
    uint32_t strsize; /* string table size in bytes */
    char* string_table;
    struct nlist_64_t* symbol_table;
};

struct section_64_t { /* for 64-bit architectures */
    char sectname[16]; /* name of this section */
    char segname[16]; /* segment this section goes in */
    uint64_t addr; /* memory address of this section */
    uint64_t size; /* size in bytes of this section */
    uint32_t offset; /* file offset of this section */
    uint32_t align; /* section alignment (power of 2) */
    uint32_t reloff; /* file offset of relocation entries */
    uint32_t nreloc; /* number of relocation entries */
    uint32_t flags; /* flags (section type and attributes)*/
    uint32_t reserved1; /* reserved (for offset or index) */
    uint32_t reserved2; /* reserved (for count or sizeof) */
    uint32_t reserved3; /* reserved */
};

typedef int vm_prot_t;

struct segment_command_64_t {
    char segname[16]; /* segment name */
    uint64_t vmaddr; /* memory address of this segment */
    uint64_t vmsize; /* memory size of this segment */
    uint64_t fileoff; /* file offset of this segment */
    uint64_t filesize; /* amount to map from the file */
    vm_prot_t maxprot; /* maximum VM protection */
    vm_prot_t initprot; /* initial VM protection */
    uint32_t nsects; /* number of sections in segment */
    uint32_t flags; /* flags */
    struct section_64_t* sections;
};

struct build_version_command_t {
    uint32_t platform; /* platform */
    uint32_t minos; /* X.Y.Z is encoded in nibbles xxxx.yy.zz */
    uint32_t sdk; /* X.Y.Z is encoded in nibbles xxxx.yy.zz */
    uint32_t ntools; /* number of tool entries following this */
};

struct load_command_t {
    uint32_t cmd;
    uint32_t cmd_size;
    union {
        struct segment_command_64_t cmd_seg_64;
        struct build_version_command_t cmd_build_version;
        struct symtab_command_t cmd_symtab;
        struct dysymtab_command_t cmd_dysymtab;
    };
};

struct mach_object_file_t {
    uint32_t magic;
    uint32_t cpu_type;
    uint32_t cpu_subtype;
    uint32_t file_type;
    uint32_t number_of_load_commands;
    uint32_t size_of_load_commands;
    uint32_t flags;
    uint32_t reserved;
    struct load_command_t* commands;
};

// END CONSTANT DECLARATION

struct mach_object_file_t* parse_file(const char* file_path);
void pretty_print(struct mach_object_file_t* file);

#endif
