#pragma once

/* Section permissions */
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

typedef struct elf64_pheader {
    unsigned int p_type;
    unsigned int p_flags;
    unsigned long p_offset;
    unsigned long p_vaddr;
    unsigned long p_paddr;
    unsigned long p_filesz;
    unsigned long p_memsz;
    unsigned long p_align;
} elf64_pheader_t;

typedef struct elf64_sheader {
    unsigned int sh_name;
    unsigned int sh_type;
    unsigned long sh_flags;
    unsigned long sh_addr;
    unsigned long sh_offset;
    unsigned long sh_size;
    unsigned int sh_link;
    unsigned int sh_info;
    unsigned long sh_addralign;
    unsigned long sh_entsize;
} elf64_sheader_t;
