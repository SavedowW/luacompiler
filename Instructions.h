#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#define NOP 0x0
#define GETSTATIC 0xb2
#define PUTSTATIC 0xb3
#define GETFIELD 0xb4
#define PUTFIELD 0xb5
#define INVOKEVIRTUAL 0xb6
#define INVOKESPECIAL 0xb7
#define INVOKESTATIC 0xb8
#define ALOAD_0 0x2a
#define ALOAD_1 0x2b
#define ALOAD_2 0x2c
#define ALOAD_3 0x2d
#define NEW 0xbb
#define DUP 0x59
#define LDC_W 0x13
#define LDC2_W 0x14
#define ARETURN 0xb0
#define RETURN 0xb1
#define POP 0x57
#define POP2 0x58
#define ICONST_M1 0x2
#define ICONST_0 0x3
#define ICONST_1 0x4
#define ICONST_2 0x5
#define ICONST_3 0x6
#define ICONST_4 0x7
#define ICONST_5 0x8
#define IFEQ 0x99
#define IFNE 0x9a
#define IFLT 0x9b
#define IFGE 0x9c
#define IFGT 0x9d
#define IFLE 0x9e
#define GOTO 0xa7
#define GOTO_W 0xc8


#endif