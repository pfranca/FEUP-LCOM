#include "i8042.h"


.text

.global _asm_handler
_asm_handler:
		xor %eax,%eax
		xor %ecx,%ecx

ciclo: 	inb $STAT_REG,%al
		test %al , %cl
		jnz ciclo
		inb $OUT_BUF, %al

		ret

.end _asm_handler
