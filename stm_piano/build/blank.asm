
build/blank.elf:     file format elf32-littlearm

Sections:
Idx Name              Size      VMA       LMA       File off  Algn  Flags
  0 .isr_vector       000000c0  08000000  08000000  00010000  2**0  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .text             000002ac  080000c0  080000c0  000100c0  2**2  CONTENTS, ALLOC, LOAD, READONLY, CODE
  2 .rodata           00000004  0800036c  0800036c  0001036c  2**2  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .init_array       00000008  08000370  08000370  00010370  2**2  CONTENTS, ALLOC, LOAD, DATA
  4 .fini_array       00000004  08000378  08000378  00010378  2**2  CONTENTS, ALLOC, LOAD, DATA
  5 .data             0000042c  20000000  0800037c  00020000  2**3  CONTENTS, ALLOC, LOAD, DATA
  6 .bss              00000020  2000042c  080007a8  0002042c  2**2  ALLOC
  7 ._user_heap_stack 00000604  2000044c  080007a8  0002044c  2**0  ALLOC
  8 .ARM.attributes   00000028  00000000  00000000  0002042c  2**0  CONTENTS, READONLY
  9 .debug_info       00000360  00000000  00000000  00020454  2**0  CONTENTS, READONLY, DEBUGGING, OCTETS
 10 .debug_abbrev     00000182  00000000  00000000  000207b4  2**0  CONTENTS, READONLY, DEBUGGING, OCTETS
 11 .debug_loc        00000140  00000000  00000000  00020936  2**0  CONTENTS, READONLY, DEBUGGING, OCTETS
 12 .debug_aranges    00000080  00000000  00000000  00020a76  2**0  CONTENTS, READONLY, DEBUGGING, OCTETS
 13 .debug_ranges     00000050  00000000  00000000  00020af6  2**0  CONTENTS, READONLY, DEBUGGING, OCTETS
 14 .debug_line       00000278  00000000  00000000  00020b46  2**0  CONTENTS, READONLY, DEBUGGING, OCTETS
 15 .debug_str        0000027d  00000000  00000000  00020dbe  2**0  CONTENTS, READONLY, DEBUGGING, OCTETS
 16 .comment          00000049  00000000  00000000  0002103b  2**0  CONTENTS, READONLY
 17 .debug_frame      000002b8  00000000  00000000  00021084  2**2  CONTENTS, READONLY, DEBUGGING, OCTETS

Disassembly of section .text:

080000c0 <__do_global_dtors_aux>:
 80000c0:	b510      	push	{r4, lr}
 80000c2:	4c06      	ldr	r4, [pc, #24]	; (80000dc <__do_global_dtors_aux+0x1c>)
 80000c4:	7823      	ldrb	r3, [r4, #0]
 80000c6:	2b00      	cmp	r3, #0
 80000c8:	d107      	bne.n	80000da <__do_global_dtors_aux+0x1a>
 80000ca:	4b05      	ldr	r3, [pc, #20]	; (80000e0 <__do_global_dtors_aux+0x20>)
 80000cc:	2b00      	cmp	r3, #0
 80000ce:	d002      	beq.n	80000d6 <__do_global_dtors_aux+0x16>
 80000d0:	4804      	ldr	r0, [pc, #16]	; (80000e4 <__do_global_dtors_aux+0x24>)
 80000d2:	e000      	b.n	80000d6 <__do_global_dtors_aux+0x16>
 80000d4:	bf00      	nop
 80000d6:	2301      	movs	r3, #1
 80000d8:	7023      	strb	r3, [r4, #0]
 80000da:	bd10      	pop	{r4, pc}
 80000dc:	2000042c 	.word	0x2000042c
 80000e0:	00000000 	.word	0x00000000
 80000e4:	08000354 	.word	0x08000354

080000e8 <frame_dummy>:
 80000e8:	4b04      	ldr	r3, [pc, #16]	; (80000fc <frame_dummy+0x14>)
 80000ea:	b510      	push	{r4, lr}
 80000ec:	2b00      	cmp	r3, #0
 80000ee:	d003      	beq.n	80000f8 <frame_dummy+0x10>
 80000f0:	4903      	ldr	r1, [pc, #12]	; (8000100 <frame_dummy+0x18>)
 80000f2:	4804      	ldr	r0, [pc, #16]	; (8000104 <frame_dummy+0x1c>)
 80000f4:	e000      	b.n	80000f8 <frame_dummy+0x10>
 80000f6:	bf00      	nop
 80000f8:	bd10      	pop	{r4, pc}
 80000fa:	46c0      	nop			; (mov r8, r8)
 80000fc:	00000000 	.word	0x00000000
 8000100:	20000430 	.word	0x20000430
 8000104:	08000354 	.word	0x08000354

08000108 <Reset_Handler>:
 8000108:	480d      	ldr	r0, [pc, #52]	; (8000140 <LoopForever+0x2>)
 800010a:	4685      	mov	sp, r0
 800010c:	480d      	ldr	r0, [pc, #52]	; (8000144 <LoopForever+0x6>)
 800010e:	490e      	ldr	r1, [pc, #56]	; (8000148 <LoopForever+0xa>)
 8000110:	4a0e      	ldr	r2, [pc, #56]	; (800014c <LoopForever+0xe>)
 8000112:	2300      	movs	r3, #0
 8000114:	e002      	b.n	800011c <LoopCopyDataInit>

08000116 <CopyDataInit>:
 8000116:	58d4      	ldr	r4, [r2, r3]
 8000118:	50c4      	str	r4, [r0, r3]
 800011a:	3304      	adds	r3, #4

0800011c <LoopCopyDataInit>:
 800011c:	18c4      	adds	r4, r0, r3
 800011e:	428c      	cmp	r4, r1
 8000120:	d3f9      	bcc.n	8000116 <CopyDataInit>
 8000122:	4a0b      	ldr	r2, [pc, #44]	; (8000150 <LoopForever+0x12>)
 8000124:	4c0b      	ldr	r4, [pc, #44]	; (8000154 <LoopForever+0x16>)
 8000126:	2300      	movs	r3, #0
 8000128:	e001      	b.n	800012e <LoopFillZerobss>

0800012a <FillZerobss>:
 800012a:	6013      	str	r3, [r2, #0]
 800012c:	3204      	adds	r2, #4

0800012e <LoopFillZerobss>:
 800012e:	42a2      	cmp	r2, r4
 8000130:	d3fb      	bcc.n	800012a <FillZerobss>
 8000132:	f000 f815 	bl	8000160 <SystemInit>
 8000136:	f000 f869 	bl	800020c <__libc_init_array>
 800013a:	f000 f80e 	bl	800015a <main>

0800013e <LoopForever>:
 800013e:	e7fe      	b.n	800013e <LoopForever>
 8000140:	20002000 	.word	0x20002000
 8000144:	20000000 	.word	0x20000000
 8000148:	2000042c 	.word	0x2000042c
 800014c:	0800037c 	.word	0x0800037c
 8000150:	2000042c 	.word	0x2000042c
 8000154:	2000044c 	.word	0x2000044c

08000158 <ADC1_COMP_IRQHandler>:
 8000158:	e7fe      	b.n	8000158 <ADC1_COMP_IRQHandler>

0800015a <main>:
 800015a:	b580      	push	{r7, lr}
 800015c:	af00      	add	r7, sp, #0
 800015e:	e7fe      	b.n	800015e <main+0x4>

08000160 <SystemInit>:
 8000160:	b580      	push	{r7, lr}
 8000162:	af00      	add	r7, sp, #0
 8000164:	4b1a      	ldr	r3, [pc, #104]	; (80001d0 <SystemInit+0x70>)
 8000166:	681a      	ldr	r2, [r3, #0]
 8000168:	4b19      	ldr	r3, [pc, #100]	; (80001d0 <SystemInit+0x70>)
 800016a:	2101      	movs	r1, #1
 800016c:	430a      	orrs	r2, r1
 800016e:	601a      	str	r2, [r3, #0]
 8000170:	4b17      	ldr	r3, [pc, #92]	; (80001d0 <SystemInit+0x70>)
 8000172:	685a      	ldr	r2, [r3, #4]
 8000174:	4b16      	ldr	r3, [pc, #88]	; (80001d0 <SystemInit+0x70>)
 8000176:	4917      	ldr	r1, [pc, #92]	; (80001d4 <SystemInit+0x74>)
 8000178:	400a      	ands	r2, r1
 800017a:	605a      	str	r2, [r3, #4]
 800017c:	4b14      	ldr	r3, [pc, #80]	; (80001d0 <SystemInit+0x70>)
 800017e:	681a      	ldr	r2, [r3, #0]
 8000180:	4b13      	ldr	r3, [pc, #76]	; (80001d0 <SystemInit+0x70>)
 8000182:	4915      	ldr	r1, [pc, #84]	; (80001d8 <SystemInit+0x78>)
 8000184:	400a      	ands	r2, r1
 8000186:	601a      	str	r2, [r3, #0]
 8000188:	4b11      	ldr	r3, [pc, #68]	; (80001d0 <SystemInit+0x70>)
 800018a:	681a      	ldr	r2, [r3, #0]
 800018c:	4b10      	ldr	r3, [pc, #64]	; (80001d0 <SystemInit+0x70>)
 800018e:	4913      	ldr	r1, [pc, #76]	; (80001dc <SystemInit+0x7c>)
 8000190:	400a      	ands	r2, r1
 8000192:	601a      	str	r2, [r3, #0]
 8000194:	4b0e      	ldr	r3, [pc, #56]	; (80001d0 <SystemInit+0x70>)
 8000196:	685a      	ldr	r2, [r3, #4]
 8000198:	4b0d      	ldr	r3, [pc, #52]	; (80001d0 <SystemInit+0x70>)
 800019a:	4911      	ldr	r1, [pc, #68]	; (80001e0 <SystemInit+0x80>)
 800019c:	400a      	ands	r2, r1
 800019e:	605a      	str	r2, [r3, #4]
 80001a0:	4b0b      	ldr	r3, [pc, #44]	; (80001d0 <SystemInit+0x70>)
 80001a2:	6ada      	ldr	r2, [r3, #44]	; 0x2c
 80001a4:	4b0a      	ldr	r3, [pc, #40]	; (80001d0 <SystemInit+0x70>)
 80001a6:	210f      	movs	r1, #15
 80001a8:	438a      	bics	r2, r1
 80001aa:	62da      	str	r2, [r3, #44]	; 0x2c
 80001ac:	4b08      	ldr	r3, [pc, #32]	; (80001d0 <SystemInit+0x70>)
 80001ae:	6b1a      	ldr	r2, [r3, #48]	; 0x30
 80001b0:	4b07      	ldr	r3, [pc, #28]	; (80001d0 <SystemInit+0x70>)
 80001b2:	490c      	ldr	r1, [pc, #48]	; (80001e4 <SystemInit+0x84>)
 80001b4:	400a      	ands	r2, r1
 80001b6:	631a      	str	r2, [r3, #48]	; 0x30
 80001b8:	4b05      	ldr	r3, [pc, #20]	; (80001d0 <SystemInit+0x70>)
 80001ba:	6b5a      	ldr	r2, [r3, #52]	; 0x34
 80001bc:	4b04      	ldr	r3, [pc, #16]	; (80001d0 <SystemInit+0x70>)
 80001be:	2101      	movs	r1, #1
 80001c0:	438a      	bics	r2, r1
 80001c2:	635a      	str	r2, [r3, #52]	; 0x34
 80001c4:	4b02      	ldr	r3, [pc, #8]	; (80001d0 <SystemInit+0x70>)
 80001c6:	2200      	movs	r2, #0
 80001c8:	609a      	str	r2, [r3, #8]
 80001ca:	46c0      	nop			; (mov r8, r8)
 80001cc:	46bd      	mov	sp, r7
 80001ce:	bd80      	pop	{r7, pc}
 80001d0:	40021000 	.word	0x40021000
 80001d4:	f8ffb80c 	.word	0xf8ffb80c
 80001d8:	fef6ffff 	.word	0xfef6ffff
 80001dc:	fffbffff 	.word	0xfffbffff
 80001e0:	ffc0ffff 	.word	0xffc0ffff
 80001e4:	fffffeac 	.word	0xfffffeac

080001e8 <NMI_Handler>:
 80001e8:	b580      	push	{r7, lr}
 80001ea:	af00      	add	r7, sp, #0
 80001ec:	46c0      	nop			; (mov r8, r8)
 80001ee:	46bd      	mov	sp, r7
 80001f0:	bd80      	pop	{r7, pc}

080001f2 <HardFault_Handler>:
 80001f2:	b580      	push	{r7, lr}
 80001f4:	af00      	add	r7, sp, #0
 80001f6:	e7fe      	b.n	80001f6 <HardFault_Handler+0x4>

080001f8 <SVC_Handler>:
 80001f8:	b580      	push	{r7, lr}
 80001fa:	af00      	add	r7, sp, #0
 80001fc:	46c0      	nop			; (mov r8, r8)
 80001fe:	46bd      	mov	sp, r7
 8000200:	bd80      	pop	{r7, pc}

08000202 <PendSV_Handler>:
 8000202:	b580      	push	{r7, lr}
 8000204:	af00      	add	r7, sp, #0
 8000206:	46c0      	nop			; (mov r8, r8)
 8000208:	46bd      	mov	sp, r7
 800020a:	bd80      	pop	{r7, pc}

0800020c <__libc_init_array>:
 800020c:	b570      	push	{r4, r5, r6, lr}
 800020e:	4d0c      	ldr	r5, [pc, #48]	; (8000240 <__libc_init_array+0x34>)
 8000210:	4e0c      	ldr	r6, [pc, #48]	; (8000244 <__libc_init_array+0x38>)
 8000212:	1b76      	subs	r6, r6, r5
 8000214:	10b6      	asrs	r6, r6, #2
 8000216:	d005      	beq.n	8000224 <__libc_init_array+0x18>
 8000218:	2400      	movs	r4, #0
 800021a:	cd08      	ldmia	r5!, {r3}
 800021c:	3401      	adds	r4, #1
 800021e:	4798      	blx	r3
 8000220:	42a6      	cmp	r6, r4
 8000222:	d1fa      	bne.n	800021a <__libc_init_array+0xe>
 8000224:	f000 f896 	bl	8000354 <_init>
 8000228:	4d07      	ldr	r5, [pc, #28]	; (8000248 <__libc_init_array+0x3c>)
 800022a:	4e08      	ldr	r6, [pc, #32]	; (800024c <__libc_init_array+0x40>)
 800022c:	1b76      	subs	r6, r6, r5
 800022e:	10b6      	asrs	r6, r6, #2
 8000230:	d005      	beq.n	800023e <__libc_init_array+0x32>
 8000232:	2400      	movs	r4, #0
 8000234:	cd08      	ldmia	r5!, {r3}
 8000236:	3401      	adds	r4, #1
 8000238:	4798      	blx	r3
 800023a:	42a6      	cmp	r6, r4
 800023c:	d1fa      	bne.n	8000234 <__libc_init_array+0x28>
 800023e:	bd70      	pop	{r4, r5, r6, pc}
 8000240:	08000370 	.word	0x08000370
 8000244:	08000370 	.word	0x08000370
 8000248:	08000370 	.word	0x08000370
 800024c:	08000378 	.word	0x08000378

08000250 <register_fini>:
 8000250:	4b03      	ldr	r3, [pc, #12]	; (8000260 <register_fini+0x10>)
 8000252:	b510      	push	{r4, lr}
 8000254:	2b00      	cmp	r3, #0
 8000256:	d002      	beq.n	800025e <register_fini+0xe>
 8000258:	4802      	ldr	r0, [pc, #8]	; (8000264 <register_fini+0x14>)
 800025a:	f000 f805 	bl	8000268 <atexit>
 800025e:	bd10      	pop	{r4, pc}
 8000260:	00000000 	.word	0x00000000
 8000264:	08000279 	.word	0x08000279

08000268 <atexit>:
 8000268:	b510      	push	{r4, lr}
 800026a:	0001      	movs	r1, r0
 800026c:	2300      	movs	r3, #0
 800026e:	2200      	movs	r2, #0
 8000270:	2000      	movs	r0, #0
 8000272:	f000 f819 	bl	80002a8 <__register_exitproc>
 8000276:	bd10      	pop	{r4, pc}

08000278 <__libc_fini_array>:
 8000278:	b570      	push	{r4, r5, r6, lr}
 800027a:	4d07      	ldr	r5, [pc, #28]	; (8000298 <__libc_fini_array+0x20>)
 800027c:	4c07      	ldr	r4, [pc, #28]	; (800029c <__libc_fini_array+0x24>)
 800027e:	1b64      	subs	r4, r4, r5
 8000280:	10a4      	asrs	r4, r4, #2
 8000282:	d005      	beq.n	8000290 <__libc_fini_array+0x18>
 8000284:	3c01      	subs	r4, #1
 8000286:	00a3      	lsls	r3, r4, #2
 8000288:	58eb      	ldr	r3, [r5, r3]
 800028a:	4798      	blx	r3
 800028c:	2c00      	cmp	r4, #0
 800028e:	d1f9      	bne.n	8000284 <__libc_fini_array+0xc>
 8000290:	f000 f866 	bl	8000360 <_fini>
 8000294:	bd70      	pop	{r4, r5, r6, pc}
 8000296:	46c0      	nop			; (mov r8, r8)
 8000298:	08000378 	.word	0x08000378
 800029c:	0800037c 	.word	0x0800037c

080002a0 <__retarget_lock_acquire_recursive>:
 80002a0:	4770      	bx	lr
 80002a2:	46c0      	nop			; (mov r8, r8)

080002a4 <__retarget_lock_release_recursive>:
 80002a4:	4770      	bx	lr
 80002a6:	46c0      	nop			; (mov r8, r8)

080002a8 <__register_exitproc>:
 80002a8:	b5f0      	push	{r4, r5, r6, r7, lr}
 80002aa:	46d6      	mov	lr, sl
 80002ac:	464f      	mov	r7, r9
 80002ae:	4646      	mov	r6, r8
 80002b0:	b5c0      	push	{r6, r7, lr}
 80002b2:	4f26      	ldr	r7, [pc, #152]	; (800034c <__register_exitproc+0xa4>)
 80002b4:	b082      	sub	sp, #8
 80002b6:	0006      	movs	r6, r0
 80002b8:	6838      	ldr	r0, [r7, #0]
 80002ba:	4692      	mov	sl, r2
 80002bc:	4698      	mov	r8, r3
 80002be:	4689      	mov	r9, r1
 80002c0:	f7ff ffee 	bl	80002a0 <__retarget_lock_acquire_recursive>
 80002c4:	4b22      	ldr	r3, [pc, #136]	; (8000350 <__register_exitproc+0xa8>)
 80002c6:	681b      	ldr	r3, [r3, #0]
 80002c8:	9301      	str	r3, [sp, #4]
 80002ca:	23a4      	movs	r3, #164	; 0xa4
 80002cc:	9a01      	ldr	r2, [sp, #4]
 80002ce:	005b      	lsls	r3, r3, #1
 80002d0:	58d5      	ldr	r5, [r2, r3]
 80002d2:	2d00      	cmp	r5, #0
 80002d4:	d02e      	beq.n	8000334 <__register_exitproc+0x8c>
 80002d6:	686c      	ldr	r4, [r5, #4]
 80002d8:	2c1f      	cmp	r4, #31
 80002da:	dc30      	bgt.n	800033e <__register_exitproc+0x96>
 80002dc:	2e00      	cmp	r6, #0
 80002de:	d10f      	bne.n	8000300 <__register_exitproc+0x58>
 80002e0:	1c63      	adds	r3, r4, #1
 80002e2:	606b      	str	r3, [r5, #4]
 80002e4:	464b      	mov	r3, r9
 80002e6:	3402      	adds	r4, #2
 80002e8:	00a4      	lsls	r4, r4, #2
 80002ea:	6838      	ldr	r0, [r7, #0]
 80002ec:	5163      	str	r3, [r4, r5]
 80002ee:	f7ff ffd9 	bl	80002a4 <__retarget_lock_release_recursive>
 80002f2:	2000      	movs	r0, #0
 80002f4:	b002      	add	sp, #8
 80002f6:	bce0      	pop	{r5, r6, r7}
 80002f8:	46ba      	mov	sl, r7
 80002fa:	46b1      	mov	r9, r6
 80002fc:	46a8      	mov	r8, r5
 80002fe:	bdf0      	pop	{r4, r5, r6, r7, pc}
 8000300:	2288      	movs	r2, #136	; 0x88
 8000302:	4651      	mov	r1, sl
 8000304:	0028      	movs	r0, r5
 8000306:	00a3      	lsls	r3, r4, #2
 8000308:	18eb      	adds	r3, r5, r3
 800030a:	5099      	str	r1, [r3, r2]
 800030c:	3a87      	subs	r2, #135	; 0x87
 800030e:	40a2      	lsls	r2, r4
 8000310:	3089      	adds	r0, #137	; 0x89
 8000312:	30ff      	adds	r0, #255	; 0xff
 8000314:	6801      	ldr	r1, [r0, #0]
 8000316:	4311      	orrs	r1, r2
 8000318:	6001      	str	r1, [r0, #0]
 800031a:	2184      	movs	r1, #132	; 0x84
 800031c:	4640      	mov	r0, r8
 800031e:	0049      	lsls	r1, r1, #1
 8000320:	5058      	str	r0, [r3, r1]
 8000322:	2e02      	cmp	r6, #2
 8000324:	d1dc      	bne.n	80002e0 <__register_exitproc+0x38>
 8000326:	002b      	movs	r3, r5
 8000328:	338d      	adds	r3, #141	; 0x8d
 800032a:	33ff      	adds	r3, #255	; 0xff
 800032c:	6819      	ldr	r1, [r3, #0]
 800032e:	430a      	orrs	r2, r1
 8000330:	601a      	str	r2, [r3, #0]
 8000332:	e7d5      	b.n	80002e0 <__register_exitproc+0x38>
 8000334:	0015      	movs	r5, r2
 8000336:	354d      	adds	r5, #77	; 0x4d
 8000338:	35ff      	adds	r5, #255	; 0xff
 800033a:	50d5      	str	r5, [r2, r3]
 800033c:	e7cb      	b.n	80002d6 <__register_exitproc+0x2e>
 800033e:	6838      	ldr	r0, [r7, #0]
 8000340:	f7ff ffb0 	bl	80002a4 <__retarget_lock_release_recursive>
 8000344:	2001      	movs	r0, #1
 8000346:	4240      	negs	r0, r0
 8000348:	e7d4      	b.n	80002f4 <__register_exitproc+0x4c>
 800034a:	46c0      	nop			; (mov r8, r8)
 800034c:	20000428 	.word	0x20000428
 8000350:	0800036c 	.word	0x0800036c

08000354 <_init>:
 8000354:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8000356:	46c0      	nop			; (mov r8, r8)
 8000358:	bcf8      	pop	{r3, r4, r5, r6, r7}
 800035a:	bc08      	pop	{r3}
 800035c:	469e      	mov	lr, r3
 800035e:	4770      	bx	lr

08000360 <_fini>:
 8000360:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8000362:	46c0      	nop			; (mov r8, r8)
 8000364:	bcf8      	pop	{r3, r4, r5, r6, r7}
 8000366:	bc08      	pop	{r3}
 8000368:	469e      	mov	lr, r3
 800036a:	4770      	bx	lr
