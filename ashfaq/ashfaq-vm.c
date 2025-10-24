typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned char    dwfenc;
typedef unsigned int    dword;
typedef unsigned long    qword;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;
typedef struct eh_frame_hdr eh_frame_hdr, *Peh_frame_hdr;

struct eh_frame_hdr {
    byte eh_frame_hdr_version; // Exception Handler Frame Header Version
    dwfenc eh_frame_pointer_encoding; // Exception Handler Frame Pointer Encoding
    dwfenc eh_frame_desc_entry_count_encoding; // Encoding of # of Exception Handler FDEs
    dwfenc eh_frame_table_encoding; // Exception Handler Table Encoding
};

typedef struct NoteGnuPropertyElement_4 NoteGnuPropertyElement_4, *PNoteGnuPropertyElement_4;

struct NoteGnuPropertyElement_4 {
    dword prType;
    dword prDatasz;
    byte data[4];
};

typedef struct fde_table_entry fde_table_entry, *Pfde_table_entry;

struct fde_table_entry {
    dword initial_loc; // Initial Location
    dword data_loc; // Data location
};

typedef void _IO_lock_t;

typedef struct _IO_marker _IO_marker, *P_IO_marker;

typedef struct _IO_FILE _IO_FILE, *P_IO_FILE;

typedef long __off_t;

typedef long __off64_t;

typedef ulong size_t;

struct _IO_FILE {
    int _flags;
    char *_IO_read_ptr;
    char *_IO_read_end;
    char *_IO_read_base;
    char *_IO_write_base;
    char *_IO_write_ptr;
    char *_IO_write_end;
    char *_IO_buf_base;
    char *_IO_buf_end;
    char *_IO_save_base;
    char *_IO_backup_base;
    char *_IO_save_end;
    struct _IO_marker *_markers;
    struct _IO_FILE *_chain;
    int _fileno;
    int _flags2;
    __off_t _old_offset;
    ushort _cur_column;
    char _vtable_offset;
    char _shortbuf[1];
    _IO_lock_t *_lock;
    __off64_t _offset;
    void *__pad1;
    void *__pad2;
    void *__pad3;
    void *__pad4;
    size_t __pad5;
    int _mode;
    char _unused2[20];
};

struct _IO_marker {
    struct _IO_marker *_next;
    struct _IO_FILE *_sbuf;
    int _pos;
};

typedef struct _IO_FILE FILE;

typedef long __ssize_t;

typedef __ssize_t ssize_t;

typedef enum Elf_ProgramHeaderType {
    PT_NULL=0,
    PT_LOAD=1,
    PT_DYNAMIC=2,
    PT_INTERP=3,
    PT_NOTE=4,
    PT_SHLIB=5,
    PT_PHDR=6,
    PT_TLS=7,
    PT_GNU_EH_FRAME=1685382480,
    PT_GNU_STACK=1685382481,
    PT_GNU_RELRO=1685382482
} Elf_ProgramHeaderType;

typedef struct Elf64_Dyn Elf64_Dyn, *PElf64_Dyn;

typedef enum Elf64_DynTag {
    DT_NULL=0,
    DT_NEEDED=1,
    DT_PLTRELSZ=2,
    DT_PLTGOT=3,
    DT_HASH=4,
    DT_STRTAB=5,
    DT_SYMTAB=6,
    DT_RELA=7,
    DT_RELASZ=8,
    DT_RELAENT=9,
    DT_STRSZ=10,
    DT_SYMENT=11,
    DT_INIT=12,
    DT_FINI=13,
    DT_SONAME=14,
    DT_RPATH=15,
    DT_SYMBOLIC=16,
    DT_REL=17,
    DT_RELSZ=18,
    DT_RELENT=19,
    DT_PLTREL=20,
    DT_DEBUG=21,
    DT_TEXTREL=22,
    DT_JMPREL=23,
    DT_BIND_NOW=24,
    DT_INIT_ARRAY=25,
    DT_FINI_ARRAY=26,
    DT_INIT_ARRAYSZ=27,
    DT_FINI_ARRAYSZ=28,
    DT_RUNPATH=29,
    DT_FLAGS=30,
    DT_PREINIT_ARRAY=32,
    DT_PREINIT_ARRAYSZ=33,
    DT_RELRSZ=35,
    DT_RELR=36,
    DT_RELRENT=37,
    DT_ANDROID_REL=1610612751,
    DT_ANDROID_RELSZ=1610612752,
    DT_ANDROID_RELA=1610612753,
    DT_ANDROID_RELASZ=1610612754,
    DT_ANDROID_RELR=1879040000,
    DT_ANDROID_RELRSZ=1879040001,
    DT_ANDROID_RELRENT=1879040003,
    DT_GNU_PRELINKED=1879047669,
    DT_GNU_CONFLICTSZ=1879047670,
    DT_GNU_LIBLISTSZ=1879047671,
    DT_CHECKSUM=1879047672,
    DT_PLTPADSZ=1879047673,
    DT_MOVEENT=1879047674,
    DT_MOVESZ=1879047675,
    DT_FEATURE_1=1879047676,
    DT_POSFLAG_1=1879047677,
    DT_SYMINSZ=1879047678,
    DT_SYMINENT=1879047679,
    DT_GNU_XHASH=1879047924,
    DT_GNU_HASH=1879047925,
    DT_TLSDESC_PLT=1879047926,
    DT_TLSDESC_GOT=1879047927,
    DT_GNU_CONFLICT=1879047928,
    DT_GNU_LIBLIST=1879047929,
    DT_CONFIG=1879047930,
    DT_DEPAUDIT=1879047931,
    DT_AUDIT=1879047932,
    DT_PLTPAD=1879047933,
    DT_MOVETAB=1879047934,
    DT_SYMINFO=1879047935,
    DT_VERSYM=1879048176,
    DT_RELACOUNT=1879048185,
    DT_RELCOUNT=1879048186,
    DT_FLAGS_1=1879048187,
    DT_VERDEF=1879048188,
    DT_VERDEFNUM=1879048189,
    DT_VERNEED=1879048190,
    DT_VERNEEDNUM=1879048191,
    DT_AUXILIARY=2147483645,
    DT_FILTER=2147483647
} Elf64_DynTag;

struct Elf64_Dyn {
    enum Elf64_DynTag d_tag;
    qword d_val;
};

typedef struct Elf64_Rela Elf64_Rela, *PElf64_Rela;

struct Elf64_Rela {
    qword r_offset; // location to apply the relocation action
    qword r_info; // the symbol table index and the type of relocation
    qword r_addend; // a constant addend used to compute the relocatable field value
};

typedef struct Elf64_Shdr Elf64_Shdr, *PElf64_Shdr;

typedef enum Elf_SectionHeaderType {
    SHT_NULL=0,
    SHT_PROGBITS=1,
    SHT_SYMTAB=2,
    SHT_STRTAB=3,
    SHT_RELA=4,
    SHT_HASH=5,
    SHT_DYNAMIC=6,
    SHT_NOTE=7,
    SHT_NOBITS=8,
    SHT_REL=9,
    SHT_SHLIB=10,
    SHT_DYNSYM=11,
    SHT_INIT_ARRAY=14,
    SHT_FINI_ARRAY=15,
    SHT_PREINIT_ARRAY=16,
    SHT_GROUP=17,
    SHT_SYMTAB_SHNDX=18,
    SHT_ANDROID_REL=1610612737,
    SHT_ANDROID_RELA=1610612738,
    SHT_GNU_ATTRIBUTES=1879048181,
    SHT_GNU_HASH=1879048182,
    SHT_GNU_LIBLIST=1879048183,
    SHT_CHECKSUM=1879048184,
    SHT_SUNW_move=1879048186,
    SHT_SUNW_COMDAT=1879048187,
    SHT_SUNW_syminfo=1879048188,
    SHT_GNU_verdef=1879048189,
    SHT_GNU_verneed=1879048190,
    SHT_GNU_versym=1879048191
} Elf_SectionHeaderType;

struct Elf64_Shdr {
    dword sh_name;
    enum Elf_SectionHeaderType sh_type;
    qword sh_flags;
    qword sh_addr;
    qword sh_offset;
    qword sh_size;
    dword sh_link;
    dword sh_info;
    qword sh_addralign;
    qword sh_entsize;
};

typedef struct GnuBuildId GnuBuildId, *PGnuBuildId;

struct GnuBuildId {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
    byte hash[20];
};

typedef struct NoteGnuProperty_4 NoteGnuProperty_4, *PNoteGnuProperty_4;

struct NoteGnuProperty_4 {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
};

typedef struct Elf64_Phdr Elf64_Phdr, *PElf64_Phdr;

struct Elf64_Phdr {
    enum Elf_ProgramHeaderType p_type;
    dword p_flags;
    qword p_offset;
    qword p_vaddr;
    qword p_paddr;
    qword p_filesz;
    qword p_memsz;
    qword p_align;
};

typedef struct Elf64_Ehdr Elf64_Ehdr, *PElf64_Ehdr;

struct Elf64_Ehdr {
    byte e_ident_magic_num;
    char e_ident_magic_str[3];
    byte e_ident_class;
    byte e_ident_data;
    byte e_ident_version;
    byte e_ident_osabi;
    byte e_ident_abiversion;
    byte e_ident_pad[7];
    word e_type;
    word e_machine;
    dword e_version;
    qword e_entry;
    qword e_phoff;
    qword e_shoff;
    dword e_flags;
    word e_ehsize;
    word e_phentsize;
    word e_phnum;
    word e_shentsize;
    word e_shnum;
    word e_shstrndx;
};

typedef struct Elf64_Sym Elf64_Sym, *PElf64_Sym;

struct Elf64_Sym {
    dword st_name;
    byte st_info;
    byte st_other;
    word st_shndx;
    qword st_value;
    qword st_size;
};

typedef struct ElfNote_4_96 ElfNote_4_96, *PElfNote_4_96;

struct ElfNote_4_96 {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
    byte description[96]; // Blob value
};

typedef struct NoteAbiTag NoteAbiTag, *PNoteAbiTag;

struct NoteAbiTag {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
    dword abiType; // 0 == Linux
    dword requiredKernelVersion[3]; // Major.minor.patch
};




void _DT_INIT(void)

{
  __gmon_start__();
  return;
}



void FUN_00101020(void)

{
  (*(code *)(undefined *)0x0)();
  return;
}



void FUN_00101120(void)

{
  __cxa_finalize();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int putchar(int __c)

{
  int iVar1;
  
  iVar1 = putchar(__c);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int puts(char *__s)

{
  int iVar1;
  
  iVar1 = puts(__s);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

size_t fread(void *__ptr,size_t __size,size_t __n,FILE *__stream)

{
  size_t sVar1;
  
  sVar1 = fread(__ptr,__size,__n,__stream);
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ssize_t write(int __fd,void *__buf,size_t __n)

{
  ssize_t sVar1;
  
  sVar1 = write(__fd,__buf,__n);
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int fclose(FILE *__stream)

{
  int iVar1;
  
  iVar1 = fclose(__stream);
  return iVar1;
}



void __stack_chk_fail(void)

{
                    // WARNING: Subroutine does not return
  __stack_chk_fail();
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int printf(char *__format,...)

{
  int iVar1;
  
  iVar1 = printf(__format);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void * memset(void *__s,int __c,size_t __n)

{
  void *pvVar1;
  
  pvVar1 = memset(__s,__c,__n);
  return pvVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

size_t strcspn(char *__s,char *__reject)

{
  size_t sVar1;
  
  sVar1 = strcspn(__s,__reject);
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ssize_t read(int __fd,void *__buf,size_t __nbytes)

{
  ssize_t sVar1;
  
  sVar1 = read(__fd,__buf,__nbytes);
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int fprintf(FILE *__stream,char *__format,...)

{
  int iVar1;
  
  iVar1 = fprintf(__stream,__format);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void * memcpy(void *__dest,void *__src,size_t __n)

{
  void *pvVar1;
  
  pvVar1 = memcpy(__dest,__src,__n);
  return pvVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int setvbuf(FILE *__stream,char *__buf,int __modes,size_t __n)

{
  int iVar1;
  
  iVar1 = setvbuf(__stream,__buf,__modes,__n);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

FILE * fopen(char *__filename,char *__modes)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(__filename,__modes);
  return pFVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void exit(int __status)

{
                    // WARNING: Subroutine does not return
  exit(__status);
}



void processEntry entry(undefined8 param_1,undefined8 param_2)

{
  undefined auStack_8 [8];
  
  __libc_start_main(FUN_001032b0,param_2,&stack0x00000008,0,0,param_1,auStack_8);
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}



// WARNING: Removing unreachable block (ram,0x00101263)
// WARNING: Removing unreachable block (ram,0x0010126f)

void FUN_00101250(void)

{
  return;
}



// WARNING: Removing unreachable block (ram,0x001012a4)
// WARNING: Removing unreachable block (ram,0x001012b0)

void FUN_00101280(void)

{
  return;
}



void _FINI_0(void)

{
  if (DAT_00106048 != '\0') {
    return;
  }
  FUN_00101120(PTR_LOOP_00106008);
  FUN_00101250();
  DAT_00106048 = 1;
  return;
}



void _INIT_0(void)

{
  FUN_00101280();
  return;
}



void _INIT_1(void)

{
  setvbuf(stdin,(char *)0x0,0,0);
  setvbuf(stdout,(char *)0x0,2,0);
  setvbuf(stderr,(char *)0x0,2,0);
  return;
}



undefined4 FUN_0010136e(byte param_1)

{
  undefined4 uVar1;
  
  if ((param_1 < 9) && (param_1 != 0)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



void FUN_00101398(long param_1,byte param_2,byte param_3,char param_4)

{
  char cVar1;
  
  cVar1 = FUN_0010136e(param_3);
  if (cVar1 != '\x01') {
    printf("[\x1b[35m%s\x1b[0m] ","ERROR");
    printf("invalid src register: R%d\n",(ulong)param_3);
                    // WARNING: Subroutine does not return
    exit(-1);
  }
  cVar1 = FUN_0010136e(param_2);
  if (cVar1 != '\x01') {
    printf("[\x1b[35m%s\x1b[0m] ","ERROR");
    printf("invalid dst register: R%d\n",(ulong)param_2);
                    // WARNING: Subroutine does not return
    exit(-1);
  }
  *(undefined4 *)(param_1 + (long)(int)(param_2 - 1) * 4) =
       *(undefined4 *)(param_1 + (long)(int)(param_3 - 1) * 4);
  if (param_4 == '\0') {
    *(undefined4 *)(param_1 + (long)(int)(param_3 - 1) * 4) = 0;
  }
  return;
}



void FUN_0010149d(undefined4 param_1,byte param_2,char param_3,long param_4)

{
  uint uVar1;
  undefined4 local_10;
  undefined4 local_c;
  
  *(undefined4 *)(param_4 + 0x202c + (ulong)*(uint *)(param_4 + 0x232c)) = param_1;
  *(byte *)(param_4 + 0x202c + (ulong)(*(int *)(param_4 + 0x232c) + 4)) = param_2;
  if (param_3 == '\x01') {
    local_10 = 0;
    local_c = *(int *)(param_4 + 0x232c) + 5;
    for (; local_10 < (int)(uint)param_2; local_10 = local_10 + 1) {
      uVar1 = *(uint *)(param_4 + 0x20);
      *(uint *)(param_4 + 0x20) = uVar1 + 1;
      *(undefined *)(param_4 + 0x202c + (long)local_c) =
           *(undefined *)(param_4 + 0x2c + (ulong)uVar1);
      local_c = local_c + 1;
    }
  }
  else {
    memcpy((void *)(param_4 + (ulong)*(uint *)(param_4 + 0x232c) + 0x2031),
           (void *)(param_4 + 0x222c),(ulong)param_2);
  }
  *(uint *)(param_4 + 0x232c) = (uint)param_2 + *(int *)(param_4 + 0x232c) + 6;
  return;
}



int FUN_001015ad(int param_1,long param_2)

{
  byte bVar1;
  uint local_14;
  
  local_14 = 0;
  while( true ) {
    if (*(uint *)(param_2 + 0x232c) <= local_14) {
      return -0x11110001;
    }
    bVar1 = *(byte *)(param_2 + 0x202c + (ulong)(local_14 + 4));
    if (*(int *)(param_2 + (ulong)local_14 + 0x202c) == param_1) break;
    if (bVar1 == 0) {
      return -0x11110001;
    }
    local_14 = local_14 + bVar1 + 6;
  }
  return local_14 + 4;
}



void FUN_00101646(undefined4 param_1,long param_2)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = FUN_001015ad(param_1,param_2);
  if (uVar2 == 0xeeeeffff) {
    *(undefined4 *)(param_2 + 0x1c) = 0xeeeeffff;
  }
  else {
    bVar1 = *(byte *)(param_2 + 0x202c + (ulong)uVar2);
    *(byte *)(param_2 + 0x232b) = bVar1;
    memcpy((void *)(param_2 + 0x222c),(void *)(param_2 + 0x202c + (ulong)(uVar2 + 1)),(ulong)bVar1);
  }
  return;
}



void FUN_001016d9(undefined8 param_1,undefined8 param_2,char param_3)

{
  undefined *puVar1;
  char *pcVar2;
  
  if (param_3 == '\0') {
    pcVar2 = "UNSET";
    puVar1 = &DAT_00104063;
  }
  else {
    pcVar2 = "SET";
    puVar1 = &DAT_0010405d;
  }
  printf("%s%s%s=%s%s%s  ",param_2,param_1,&DAT_00104069,puVar1,pcVar2,&DAT_00104069);
  return;
}



void FUN_00101760(long param_1)

{
  int local_c;
  
  printf("\n%s%s==== Register Dump ====%s\n",&DAT_00104084,&DAT_0010407e,&DAT_00104069);
  for (local_c = 0; local_c < 8; local_c = local_c + 1) {
    printf(&DAT_001040b6,&DAT_001040b0,(ulong)(local_c + 1),&DAT_00104069,&DAT_0010405d,
           (ulong)*(uint *)(param_1 + (long)local_c * 4),&DAT_00104069);
  }
  putchar(10);
  printf("\x1b[35mPC\x1b[0m=\x1b[37m0x%04X%s  ",(ulong)*(uint *)(param_1 + 0x20),&DAT_00104069);
  printf("\x1b[31mSP\x1b[0m=\x1b[37m0x%04X%s  ",(ulong)*(uint *)(param_1 + 0x24),&DAT_00104069);
  printf("\x1b[34mBP\x1b[0m=\x1b[37m0x%04X%s\n",(ulong)*(uint *)(param_1 + 0x28),&DAT_00104069);
  FUN_001016d9(&DAT_00104124,&DAT_001040b0,*(undefined *)(param_1 + 0x2334));
  FUN_001016d9(&DAT_00104127,&DAT_0010407e,*(undefined *)(param_1 + 0x2335));
  FUN_001016d9(&DAT_00104130,&DAT_0010412a,*(undefined *)(param_1 + 0x2336));
  printf("%sCMP_RESULT%s=%s%d%s\n",&DAT_00104139,&DAT_00104069,&DAT_00104133,
         (ulong)*(uint *)(param_1 + 0x2330),&DAT_00104069);
  return;
}



void FUN_00101946(long param_1)

{
  uint uVar1;
  byte bVar2;
  byte local_16;
  uint local_14;
  uint local_10;
  
  printf("\n%s%s==== Stored Strings ====%s\n",&DAT_00104084,&DAT_00104139,&DAT_00104069);
  if (*(int *)(param_1 + 0x232c) == 0) {
    printf("%s[!] No strings stored%s\n",&DAT_001040b0,&DAT_00104069);
    printf("%s=========================%s\n\n",&DAT_00104139,&DAT_00104069);
  }
  else {
    local_10 = 0;
    local_14 = 0;
    while (local_14 < *(uint *)(param_1 + 0x232c)) {
      uVar1 = local_14 + 5;
      bVar2 = *(byte *)(param_1 + 0x202c + (ulong)(local_14 + 4));
      if (bVar2 == 0) break;
      printf("%s[%02d]%s UID=%s0x%X%s  SIZE=%s0x%X%s\n",&DAT_0010407e,(ulong)local_10,&DAT_00104069,
             &DAT_001040b0,(ulong)*(uint *)(param_1 + (ulong)local_14 + 0x202c),&DAT_00104069,
             &DAT_001040b0,(ulong)bVar2,&DAT_00104069);
      printf("     %sCONTENT:%s %s\n",&DAT_0010405d,&DAT_00104069,(ulong)uVar1 + param_1 + 0x202c);
      printf("     %s    HEX:%s ",&DAT_0010412a,&DAT_00104069);
      for (local_16 = 0; local_16 < bVar2; local_16 = local_16 + 1) {
        printf("%02X ",(ulong)*(byte *)(param_1 + 0x202c + (ulong)(local_16 + uVar1)));
      }
      puts("\n");
      local_10 = local_10 + 1;
      local_14 = uVar1 + bVar2 + 1;
    }
    printf("%s=========================%s\n",&DAT_00104139,&DAT_00104069);
  }
  return;
}



void FUN_00101ba6(long param_1)

{
  uint uVar1;
  uint local_10;
  
  printf("\n%s%s==== Stack Dump ====%s\n",&DAT_00104084,&DAT_0010407e,&DAT_00104069);
  printf("%sSP%s: %s0x%04X%s | %sBP%s: %s0x%04X%s\n",&DAT_00104063,&DAT_00104069,&DAT_00104133,
         (ulong)*(uint *)(param_1 + 0x24),&DAT_00104069,&DAT_0010412a,&DAT_00104069,&DAT_00104133,
         (ulong)*(uint *)(param_1 + 0x28),&DAT_00104069);
  if (*(int *)(param_1 + 0x24) == 0x1000) {
    printf("%s[!] Stack is empty%s\n",&DAT_001040b0,&DAT_00104069);
    printf("%s====================%s\n",&DAT_0010407e,&DAT_00104069);
  }
  else {
    printf("%s  %-8s | %-12s | %-10s%s\n",&DAT_00104084,"OFFSET","DWORD (HEX)","DWORD (DEC)",
           &DAT_00104069);
    printf("%s  %-8s-+-%-12s-+-%-10s%s\n",&DAT_0010407e,"--------","------------","----------",
           &DAT_00104069);
    for (local_10 = *(uint *)(param_1 + 0x24); local_10 < 0x1000; local_10 = local_10 + 4) {
      uVar1 = *(uint *)(param_1 + (ulong)local_10 + 0x102c);
      printf("  0x%04X   |  0x%08X   |  %-10u\n",(ulong)local_10,(ulong)uVar1,(ulong)uVar1);
    }
    printf("\n%s====================%s\n\n",&DAT_0010407e,&DAT_00104069);
  }
  return;
}



void FUN_00101dba(uint *param_1)

{
  ssize_t sVar1;
  size_t sVar2;
  
  if ((char)param_1[1] == '\0') {
    param_1[7] = 0xeeeeffff;
  }
  else {
    sVar1 = read(0,param_1 + 0x88b,(long)(int)((param_1[1] & 0xff) - 1));
    sVar2 = strcspn((char *)(param_1 + 0x88b),"\n");
    *(undefined *)((long)param_1 + sVar2 + 0x222c) = 0;
    *(char *)((long)param_1 + 0x232b) = (char)sVar1 + -2;
    *param_1 = (uint)(byte)((char)sVar1 - 1);
  }
  return;
}



void FUN_00101e56(long param_1)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  ssize_t sVar4;
  undefined local_15;
  
  local_15 = (byte)*(undefined4 *)(param_1 + 8);
  cVar2 = (char)*(undefined4 *)(param_1 + 0xc);
  if ((cVar2 == '\0') || (cVar2 == '\x01')) {
    if (cVar2 == '\x01') {
      write(1,(void *)(param_1 + 0x222c),(ulong)local_15);
    }
    else {
      uVar3 = FUN_001015ad(*(undefined4 *)(param_1 + 4),param_1);
      if (uVar3 == 0xeeeeffff) {
        *(undefined4 *)(param_1 + 0x1c) = 0xeeeeffff;
      }
      else {
        bVar1 = *(byte *)(param_1 + 0x202c + (ulong)uVar3);
        if ((bVar1 < local_15) || (local_15 == 0)) {
          local_15 = bVar1;
        }
        sVar4 = write(1,(void *)(param_1 + 0x202c + (ulong)(uVar3 + 1)),(ulong)local_15);
        if ((byte)sVar4 != local_15) {
          *(undefined4 *)(param_1 + 0x1c) = 0xeeeeffff;
        }
      }
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x1c) = 0xeeeeffff;
  }
  return;
}



void FUN_00101f96(long param_1)

{
  FUN_00101646(*(undefined4 *)(param_1 + 4),param_1);
  return;
}



void FUN_00101fbe(void *param_1)

{
  memset(param_1,0,0x2338);
  *(undefined4 *)((long)param_1 + 0x28) = 0x1000;
  *(undefined4 *)((long)param_1 + 0x24) = *(undefined4 *)((long)param_1 + 0x28);
  return;
}



void FUN_00102000(undefined8 param_1)

{
  FUN_00101fbe(param_1);
  return;
}



void FUN_0010201f(short param_1,long param_2)

{
  ushort uVar1;
  bool bVar2;
  
  uVar1 = *(ushort *)(param_2 + (ulong)*(uint *)(param_2 + 0x20) + 0x2c);
  *(int *)(param_2 + 0x20) = *(int *)(param_2 + 0x20) + 2;
  bVar2 = false;
  switch(param_1) {
  case -0x6660:
    bVar2 = true;
    break;
  case -0x665f:
  case -0x665e:
    bVar2 = (bool)*(char *)(param_2 + 0x2334) == (param_1 == -0x665f);
    break;
  case -0x665d:
    if ((*(char *)(param_2 + 0x2334) == '\0') &&
       (*(char *)(param_2 + 0x2335) == *(char *)(param_2 + 0x2336))) {
      bVar2 = true;
    }
    else {
      bVar2 = false;
    }
    break;
  case -0x665c:
    bVar2 = *(char *)(param_2 + 0x2335) == *(char *)(param_2 + 0x2336);
    break;
  case -0x665b:
    bVar2 = *(char *)(param_2 + 0x2335) != *(char *)(param_2 + 0x2336);
    break;
  case -0x665a:
    if ((*(char *)(param_2 + 0x2334) == '\x01') ||
       (*(char *)(param_2 + 0x2335) != *(char *)(param_2 + 0x2336))) {
      bVar2 = true;
    }
    else {
      bVar2 = false;
    }
  }
  if (bVar2) {
    *(uint *)(param_2 + 0x20) = (uint)uVar1;
  }
  return;
}



void FUN_00102199(uint *param_1)

{
  undefined uVar1;
  byte bVar2;
  ushort uVar3;
  undefined4 uVar4;
  long lVar5;
  char cVar6;
  int iVar7;
  uint uVar8;
  long in_FS_OFFSET;
  byte local_31;
  byte local_30;
  uint local_2c;
  
  lVar5 = *(long *)(in_FS_OFFSET + 0x28);
LAB_00103285:
  do {
    if (0xfef < param_1[8]) {
LAB_0010329a:
      if (lVar5 == *(long *)(in_FS_OFFSET + 0x28)) {
        return;
      }
                    // WARNING: Subroutine does not return
      __stack_chk_fail();
    }
    uVar3 = *(ushort *)((long)param_1 + (ulong)param_1[8] + 0x2c);
    param_1[8] = param_1[8] + 2;
    if (uVar3 != 0xffff) {
      if (uVar3 == 0xdead) goto LAB_0010329a;
      if ((uVar3 < 0xdeae) && (uVar3 < 0x99a7)) {
        if (0x999f < uVar3) {
          FUN_0010201f(uVar3,param_1);
          goto LAB_00103285;
        }
        if (uVar3 == 0x9900) {
          uVar8 = param_1[8];
          param_1[8] = uVar8 + 1;
          bVar2 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
          cVar6 = FUN_0010136e(bVar2);
          if (cVar6 != '\x01') {
            printf("[\x1b[35m%s\x1b[0m] ","ERROR");
            printf("invalid dst register: R%d\n",(ulong)bVar2);
                    // WARNING: Subroutine does not return
            exit(-1);
          }
          local_2c = *(uint *)((long)param_1 + (ulong)param_1[8] + 0x2c);
          param_1[8] = param_1[8] + 4;
          uVar8 = param_1[8];
          param_1[8] = uVar8 + 1;
          if (*(char *)((long)param_1 + (ulong)uVar8 + 0x2c) != '\0') {
            cVar6 = FUN_0010136e((byte)local_2c);
            if (cVar6 != '\x01') {
              printf("[\x1b[35m%s\x1b[0m] ","ERROR");
              printf("invalid src register: R%d\n",(ulong)(byte)local_2c);
                    // WARNING: Subroutine does not return
              exit(-1);
            }
            local_2c = param_1[(int)((local_2c & 0xff) - 1)];
          }
          uVar8 = param_1[(int)(bVar2 - 1)];
          param_1[0x8cc] = uVar8 - local_2c;
          *(bool *)(param_1 + 0x8cd) = uVar8 - local_2c == 0;
          *(undefined *)((long)param_1 + 0x2335) = 0;
          *(bool *)((long)param_1 + 0x2336) = param_1[(int)(bVar2 - 1)] < local_2c;
          goto LAB_00103285;
        }
        if ((uVar3 < 0x9901) && (uVar3 < 0xf0a)) {
          if (0xf07 < uVar3) {
            uVar8 = param_1[8];
            param_1[8] = uVar8 + 1;
            bVar2 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
            cVar6 = FUN_0010136e(bVar2);
            if (cVar6 != '\x01') {
              printf("[\x1b[35m%s\x1b[0m] ","ERROR");
              printf("invalid input register: R%d\n",(ulong)bVar2);
                    // WARNING: Subroutine does not return
              exit(-1);
            }
            uVar8 = param_1[8];
            param_1[8] = uVar8 + 1;
            local_30 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
            uVar8 = param_1[8];
            param_1[8] = uVar8 + 1;
            if (*(char *)((long)param_1 + (ulong)uVar8 + 0x2c) != '\0') {
              cVar6 = FUN_0010136e(local_30);
              if (cVar6 != '\x01') {
                printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                printf("invalid src register: R%d\n",(ulong)local_30);
                    // WARNING: Subroutine does not return
                exit(-1);
              }
              local_30 = (byte)param_1[(int)(local_30 - 1)];
            }
            if (uVar3 == 0xf08) {
              param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] << (local_30 & 0x1f);
            }
            else {
              param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] >> (local_30 & 0x1f);
            }
            goto LAB_00103285;
          }
          if (uVar3 < 0xf08) {
            if (0xeff < uVar3) {
              uVar8 = param_1[8];
              param_1[8] = uVar8 + 1;
              bVar2 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
              cVar6 = FUN_0010136e(bVar2);
              if (cVar6 != '\x01') {
                printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                printf("invalid dst register: R%d\n",(ulong)bVar2);
                    // WARNING: Subroutine does not return
                exit(-1);
              }
              local_2c = *(uint *)((long)param_1 + (ulong)param_1[8] + 0x2c);
              param_1[8] = param_1[8] + 4;
              uVar8 = param_1[8];
              param_1[8] = uVar8 + 1;
              if (*(char *)((long)param_1 + (ulong)uVar8 + 0x2c) != '\0') {
                cVar6 = FUN_0010136e((byte)local_2c);
                if (cVar6 != '\x01') {
                  printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                  printf("invalid src register: R%d\n",(ulong)(byte)local_2c);
                    // WARNING: Subroutine does not return
                  exit(-1);
                }
                local_2c = param_1[(int)((local_2c & 0xff) - 1)];
              }
              if (uVar3 == 0xf00) {
                param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] + local_2c;
              }
              else if (uVar3 == 0xf01) {
                param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] - local_2c;
              }
              else if (uVar3 == 0xf02) {
                param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] | local_2c;
              }
              else if (uVar3 == 0xf03) {
                param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] & local_2c;
              }
              else if (uVar3 == 0xf04) {
                param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] ^ local_2c;
              }
              else if (uVar3 == 0xf05) {
                param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] * local_2c;
              }
              else if (uVar3 == 0xf07) {
                param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] % local_2c;
              }
              else if (uVar3 == 0xf06) {
                if (local_2c == 0) {
                  printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                  printf("ZeroDivisionException");
                    // WARNING: Subroutine does not return
                  exit(-1);
                }
                param_1[(int)(bVar2 - 1)] = param_1[(int)(bVar2 - 1)] / local_2c;
              }
              goto LAB_00103285;
            }
            if (uVar3 == 0xd02) {
              uVar8 = param_1[8];
              param_1[8] = uVar8 + 1;
              bVar2 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
              if (param_1[9] < 0x1000) {
                cVar6 = FUN_0010136e(bVar2);
                if (cVar6 != '\x01') {
                  printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                  printf("invalid dst register: R%d\n",(ulong)bVar2);
                    // WARNING: Subroutine does not return
                  exit(-1);
                }
                param_1[(int)(bVar2 - 1)] = *(uint *)((long)param_1 + (ulong)param_1[9] + 0x102c);
              }
              else {
                param_1[7] = 0xeeeeffff;
              }
              goto LAB_00103285;
            }
            if (uVar3 < 0xd03) {
              if (uVar3 == 0xd01) {
                uVar8 = param_1[8];
                param_1[8] = uVar8 + 1;
                bVar2 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
                cVar6 = FUN_0010136e(bVar2);
                if (cVar6 != '\x01') {
                  printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                  printf("invalid src register: R%d\n",(ulong)bVar2);
                    // WARNING: Subroutine does not return
                  exit(-1);
                }
                if (0x1000 < param_1[9] + 4) {
                  printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                  puts("OOM: stack underflow");
                    // WARNING: Subroutine does not return
                  exit(-1);
                }
                param_1[(int)(bVar2 - 1)] = *(uint *)((long)param_1 + (ulong)param_1[9] + 0x102c);
                param_1[9] = param_1[9] + 4;
                goto LAB_00103285;
              }
              if (uVar3 < 0xd02) {
                if (uVar3 == 0xd00) {
                  local_2c = *(uint *)((long)param_1 + (ulong)param_1[8] + 0x2c);
                  param_1[8] = param_1[8] + 4;
                  uVar8 = param_1[8];
                  param_1[8] = uVar8 + 1;
                  if (*(char *)((long)param_1 + (ulong)uVar8 + 0x2c) != '\0') {
                    cVar6 = FUN_0010136e((byte)local_2c);
                    if (cVar6 != '\x01') {
                      printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                      printf("invalid src register: R%d\n",(ulong)(byte)local_2c);
                    // WARNING: Subroutine does not return
                      exit(-1);
                    }
                    local_2c = param_1[(int)((local_2c & 0xff) - 1)];
                  }
                  if (param_1[9] < 4) {
                    printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                    puts("OOM: stack overflow");
                    // WARNING: Subroutine does not return
                    exit(-1);
                  }
                  param_1[9] = param_1[9] - 4;
                  *(uint *)((long)param_1 + (ulong)param_1[9] + 0x102c) = local_2c;
                  goto LAB_00103285;
                }
                if (uVar3 < 0xd01) {
                  uVar8 = (uint)uVar3;
                  if (uVar8 == 0xc02) {
                    uVar8 = param_1[8];
                    param_1[8] = uVar8 + 1;
                    bVar2 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
                    cVar6 = FUN_0010136e(bVar2);
                    if (cVar6 != '\x01') {
                      printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                      printf("invalid dst register: R%d\n",(ulong)bVar2);
                    // WARNING: Subroutine does not return
                      exit(-1);
                    }
                    uVar8 = *(uint *)((long)param_1 + (ulong)param_1[8] + 0x2c);
                    param_1[8] = param_1[8] + 4;
                    param_1[(int)(bVar2 - 1)] = uVar8;
                    goto LAB_00103285;
                  }
                  if (uVar8 < 0xc03) {
                    if (uVar8 < 0x5b4) {
                      if (0x59f < uVar8) {
                        switch(uVar8) {
                        case 0x5a0:
                          uVar8 = param_1[8];
                          param_1[8] = uVar8 + 1;
                          uVar1 = *(undefined *)((long)param_1 + (ulong)uVar8 + 0x2c);
                          uVar4 = *(undefined4 *)((long)param_1 + (ulong)param_1[8] + 0x2c);
                          param_1[8] = param_1[8] + 4;
                          iVar7 = FUN_001015ad(uVar4,param_1);
                          if (iVar7 == -0x11110001) {
                            FUN_0010149d(uVar4,uVar1,1,param_1);
                          }
                          else {
                            param_1[7] = 0xeeeeffff;
                          }
                          goto LAB_00103285;
                        case 0x5b0:
                          uVar4 = *(undefined4 *)((long)param_1 + (ulong)param_1[8] + 0x2c);
                          param_1[8] = param_1[8] + 4;
                          FUN_00101646(uVar4,param_1);
                          goto LAB_00103285;
                        case 0x5b1:
                          uVar4 = *(undefined4 *)((long)param_1 + (ulong)param_1[8] + 0x2c);
                          param_1[8] = param_1[8] + 4;
                          iVar7 = FUN_001015ad(uVar4,param_1);
                          if (iVar7 == -0x11110001) {
                            if (*(char *)((long)param_1 + 0x232b) == '\0') {
                              param_1[7] = 0xeeeeffff;
                            }
                            else {
                              FUN_0010149d(uVar4,*(char *)((long)param_1 + 0x232b),0,param_1);
                            }
                          }
                          else {
                            param_1[7] = 0xeeeeffff;
                          }
                          goto LAB_00103285;
                        case 0x5b2:
                          uVar8 = param_1[8];
                          param_1[8] = uVar8 + 1;
                          local_31 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
                          uVar8 = param_1[8];
                          param_1[8] = uVar8 + 1;
                          if (*(char *)((long)param_1 + (ulong)uVar8 + 0x2c) != '\0') {
                            cVar6 = FUN_0010136e(local_31);
                            if (cVar6 != '\x01') {
                              printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                              printf("invalid src register: R%d\n",(ulong)local_31);
                    // WARNING: Subroutine does not return
                              exit(-1);
                            }
                            local_31 = (byte)param_1[(int)(local_31 - 1)];
                          }
                          if (*(byte *)((long)param_1 + 0x232b) < local_31) {
                            param_1[7] = 0xeeeeffff;
                          }
                          else {
                            *param_1 = (uint)*(byte *)((long)param_1 +
                                                      (long)(int)(uint)local_31 + 0x222c);
                          }
                          goto LAB_00103285;
                        case 0x5b3:
                          uVar4 = *(undefined4 *)((long)param_1 + (ulong)param_1[8] + 0x2c);
                          param_1[8] = param_1[8] + 4;
                          uVar8 = param_1[8];
                          param_1[8] = uVar8 + 1;
                          local_31 = *(byte *)((long)param_1 + (ulong)uVar8 + 0x2c);
                          uVar8 = param_1[8];
                          param_1[8] = uVar8 + 1;
                          cVar6 = *(char *)((long)param_1 + (ulong)uVar8 + 0x2c);
                          uVar8 = FUN_001015ad(uVar4,param_1);
                          if (uVar8 == 0xeeeeffff) {
                            param_1[7] = 0xeeeeffff;
                          }
                          else {
                            if (cVar6 != '\0') {
                              cVar6 = FUN_0010136e(local_31);
                              if (cVar6 != '\x01') {
                                printf("[\x1b[35m%s\x1b[0m] ","ERROR");
                                printf("invalid src register: R%d\n",(ulong)local_31);
                    // WARNING: Subroutine does not return
                                exit(-1);
                              }
                              local_31 = (byte)param_1[(int)(local_31 - 1)];
                            }
                            if (*(byte *)((long)param_1 + (ulong)uVar8 + 0x202c) < local_31) {
                              param_1[7] = 0xeeeeffff;
                            }
                            else {
                              *param_1 = (uint)*(byte *)((long)param_1 +
                                                        (ulong)(uVar8 + local_31 + 1) + 0x202c);
                            }
                          }
                          goto LAB_00103285;
                        }
                      }
                    }
                    else if (uVar8 - 0xc00 < 2) {
                      uVar8 = param_1[8];
                      param_1[8] = uVar8 + 1;
                      uVar1 = *(undefined *)((long)param_1 + (ulong)uVar8 + 0x2c);
                      uVar8 = param_1[8];
                      param_1[8] = uVar8 + 1;
                      FUN_00101398(param_1,uVar1,*(undefined *)((long)param_1 + (ulong)uVar8 + 0x2c)
                                   ,uVar3 != 0xc00);
                      goto LAB_00103285;
                    }
                  }
                }
              }
            }
          }
        }
      }
      printf("[\x1b[35m%s\x1b[0m] ","ERROR");
      printf("Invalid opcode: 0x%02X at PC: 0x%02X\n",(ulong)uVar3,(ulong)param_1[8]);
                    // WARNING: Subroutine does not return
      exit(-1);
    }
    uVar8 = *param_1;
    if (uVar8 < 7) {
      switch(uVar8) {
      case 1:
        FUN_00101dba(param_1);
        goto LAB_00103285;
      case 2:
        FUN_00101e56(param_1);
        goto LAB_00103285;
      case 3:
        FUN_00101f96(param_1);
        goto LAB_00103285;
      case 4:
        FUN_00101760(param_1);
        goto LAB_00103285;
      case 5:
        FUN_00101ba6(param_1);
        goto LAB_00103285;
      case 6:
        FUN_00101946(param_1);
        goto LAB_00103285;
      }
    }
    else if (uVar8 == 0xff) {
                    // WARNING: Subroutine does not return
      exit(0);
    }
    param_1[7] = 0xeeeeffff;
  } while( true );
}



undefined8 FUN_001032b0(int param_1,undefined8 *param_2)

{
  FILE *__stream;
  size_t sVar1;
  long in_FS_OFFSET;
  undefined local_2348 [44];
  undefined local_231c [8972];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_1 != 2) {
    fprintf(stderr,"No arguments provided!\nUsage: %s <program.ashfaq>",*param_2);
                    // WARNING: Subroutine does not return
    exit(1);
  }
  __stream = fopen((char *)param_2[1],"r");
  if (__stream == (FILE *)0x0) {
    printf("[\x1b[35m%s\x1b[0m] ","ERROR");
    printf("Failed to open file %s!\n",param_2[1]);
                    // WARNING: Subroutine does not return
    exit(-1);
  }
  FUN_00101fbe(local_2348);
  sVar1 = fread(local_231c,1,0x1000,__stream);
  fclose(__stream);
  if (sVar1 == 0) {
    printf("[\x1b[35m%s\x1b[0m] ","ERROR");
    puts("Empty or invalid program file!");
                    // WARNING: Subroutine does not return
    exit(-1);
  }
  FUN_00102199(local_2348);
  FUN_00102000(local_2348);
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    // WARNING: Subroutine does not return
    __stack_chk_fail();
  }
  return 0;
}



void _DT_FINI(void)

{
  return;
}


