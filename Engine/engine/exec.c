#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "CuteAtom.h"
#include "CuteConfig.h"
#include "CuteInstr.h"


#include "context.h"
#include "error.h"
#include "containers/container.h"
#include "utils/utils.h"



#define INSTR_BINARYOP(TYPE, FIELD, OP) \
r1 = instrs[ctx->ip++]; \
r2 = instrs[ctx->ip++]; \
r3 = instrs[ctx->ip++]; \
ct_ctx_loadAtom(ctx, r2, &a1, &t1); \
ct_ctx_loadAtom(ctx, r3, &a2, &t2); \
ct_ctx_storeAtom(ctx, r1, (ctAtom){.FIELD = a1.FIELD OP a2.FIELD}, TYPE);


#define INSTR_UNARYOP(TYPE, FIELD, OP) \
r1 = instrs[ctx->ip++]; \
r2 = instrs[ctx->ip++]; \
ct_ctx_loadAtom(ctx, r2, &a1, &t1); \
ct_ctx_storeAtom(ctx, r1, (ctAtom){.FIELD = OP (a1.FIELD)}, TYPE);


#define INSTR_CMP(TYPE, FIELD) \
r1 = instrs[ctx->ip++]; \
r2 = instrs[ctx->ip++]; \
ct_ctx_loadAtom(ctx, r1, &a1, &t1); \
ct_ctx_loadAtom(ctx, r2, &a2, &t2); \
ctx->cmp_diff = (double)a1.FIELD - (double)a2.FIELD; 


#define INSTR_CMP_RESOLVER(OP) \
r1 = instrs[ctx->ip++]; \
ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_bool = ctx->cmp_diff OP 0 ? 1 : 0}, ctAtomType_Bool); \


#define INSTR_JMP() \
ct_loadBytes(instrs, &ctx->ip, 4, &i32); \
ctx->ip += i32; \
if (ctx->ip >= ctx->image->header.instruction_count) { \
ctx->error = (ctError) {.code=ctErrorCode_IllegalInstruction}; \
ct_utils_format(ctx->error.msg, sizeof(ctx->error.msg), "Out of range ip: 0x%08lX", ctx->ip); ct_ctx_throwError(ctx, ctx->error); };


#define INSTR_JMPABS() \
ct_loadBytes(instrs, &ctx->ip, 4, &u32); \
ctx->ip = u32; \
if (ctx->ip >= ctx->image->header.instruction_count) { \
ctx->error = (ctError) {.code=ctErrorCode_IllegalInstruction}; \
ct_utils_format(ctx->error.msg, sizeof(ctx->error.msg), "Out of range ip: 0x%08lX", ctx->ip); ct_ctx_throwError(ctx, ctx->error); };


static inline void
ct_loadBytes(ctInstructionSize* instrs, uint64_t* ip, uint32_t n, void* dest) {
	memcpy(dest, &instrs[*ip], n);
	*ip += n;
}


static inline void 
out(ctAtom atom, ctAtomTypeSize type) {

	const char* name = ct_atom_stringforms[type];

	switch (type) {
		
		case ctAtomType_NoneType:
			printf("[ %s ]\n", name); break;
		case ctAtomType_Int:
			printf("[ %s %ld ]\n", name, atom.as_int); break;
		case ctAtomType_UInt:
			printf("[ %s %lu ]\n", name, atom.as_uint); break;
		case ctAtomType_Float:
			printf("[ %s %f ]\n", name, atom.as_float); break;
		case ctAtomType_Bool:
			printf("[ %s %u ]\n", name, atom.as_bool ? 1 : 0); break;
		case ctAtomType_Container:
			printf("[ %s %p ]\n", name, atom.as_container); break;
		default:
			printf("[ unknown ]\n");
	}
}

void ct_exec(ctContext* ctx) {
    ctInstructionSize* instrs = ctx->image->instruction_pool;

    static void* dispatch_table[] = {
        [instrNull] = &&opNull,
        [instrHalt] = &&opHalt,
        [instrAssert] = &&opAssert,
        [instrOut] = &&opOut,
        [instrOutBits] = &&opOutBits,
        [instrMov] = &&opMov,
        [instrSetI] = &&opSetI,
        [instrSetU] = &&opSetU,
        [instrSetF] = &&opSetF,
        [instrSetB] = &&opSetB,
        [instrSetN] = &&opSetN,
        [instrAddI] = &&opAddI,
        [instrSubI] = &&opSubI,
        [instrMulI] = &&opMulI,
        [instrDivI] = &&opDivI,
        [instrModI] = &&opModI,
        [instrNegI] = &&opNegI,
        [instrAbsI] = &&opAbsI,
		[instrIncI] = &&opIncI,
        [instrDecI] = &&opDecI,
        [instrAddU] = &&opAddU,
        [instrSubU] = &&opSubU,
        [instrMulU] = &&opMulU,
        [instrDivU] = &&opDivU,
        [instrModU] = &&opModU,
		[instrIncU] = &&opIncU,
        [instrDecU] = &&opDecU,
        [instrAddF] = &&opAddF,
        [instrSubF] = &&opSubF,
        [instrMulF] = &&opMulF,
        [instrDivF] = &&opDivF,
        [instrNegF] = &&opNegF,
        [instrAbsF] = &&opAbsF,
        [instrLogicAnd] = &&opLogicAnd,
        [instrLogicOr] = &&opLogicOr,
        [instrLogicNot] = &&opLogicNot,
        [instrLogicXor] = &&opLogicXor,
        [instrBitAnd] = &&opBitAnd,
        [instrBitOr] = &&opBitOr,
        [instrBitNot] = &&opBitNot,
        [instrBitXor] = &&opBitXor,
        [instrBitShl] = &&opBitShl,
        [instrBitShr] = &&opBitShr,
        [instrCmpI] = &&opCmpI,
        [instrCmpU] = &&opCmpU,
        [instrCmpF] = &&opCmpF,
        [instrEq] = &&opEq,
        [instrNotEq] = &&opNotEq,
        [instrLess] = &&opLess,
        [instrLessEq] = &&opLessEq,
        [instrGreater] = &&opGreater,
        [instrGreaterEq] = &&opGreaterEq,
        [instrJmp] = &&opJmp,
        [instrJmpIf] = &&opJmpIf,
        [instrJmpIfNot] = &&opJmpIfNot,
        [instrJmpAbs] = &&opJmpAbs,
        [instrJmpAbsIf] = &&opJmpAbsIf,
        [instrJmpAbsIfNot] = &&opJmpAbsIfNot,
        [instrCall] = &&opCall,
        [instrReturn] = &&opReturn,
        [instrReturnVal] = &&opReturnVal,
		[instrModCall] = &&opModCall,
        [instrConNew] = &&opConNew,
        [instrConDel] = &&opConDel,
        [instrConGet] = &&opConGet,
        [instrConSet] = &&opConSet,
        [instrConSize] = &&opConSize,
        [instrConCopy] = &&opConCopy,
		[instrConDeepCopy] = &&opConDeepCopy,
        [instrConResize] = &&opConResize,
    };

    uint8_t r1, r2, r3, r4, r5;
    int32_t i32;
    uint32_t u32;
    float f32;
    ctAtom a1, a2, a3;
    ctAtomType t1, t2, t3;
    ctTypedAtom typed_atom;

    goto next;

opNull:
    goto next;

opHalt:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ctx->exit_code = a1.as_uint;
    return;

opAssert:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    if (!a1.as_bool) {
		ctx->error = (ctError) {.code=ctErrorCode_AssertionFailed};
		ct_utils_format(
			ctx->error.msg, 
			sizeof(ctx->error.msg), 
			"Assertion failed at ip 0x%08lX, targeting slot '%u'",
			ctx->ip-1, r1
		);
		ct_ctx_throwError(ctx, ctx->error);
        return;
    }
    goto next;

opOut:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    out(a1, t1);
    goto next;

opOutBits:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    for (int i = 63; i >= 0; i--) {
        printf("%d", (int)((a1.raw >> i) & 1));
        if (i % 8 == 0 && i != 0) printf(" ");
    }
    printf(" [ 0x%016lX ]\n", (uint64_t)a1.raw);
    goto next;

opMov:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_moveAtom(ctx, r2, r1);
    goto next;

opSetI:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &i32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_int=i32}, ctAtomType_Int);
    goto next;

opSetU:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &u32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_uint=u32}, ctAtomType_UInt);
    goto next;

opSetF:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &f32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_float=i32}, ctAtomType_Float);
    goto next;

opSetB:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &u32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_bool=u32}, ctAtomType_Bool);
    goto next;

opSetN:
    r1 = instrs[ctx->ip++];
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_uint=0}, ctAtomType_NoneType);
    goto next;

opAddI: 
	INSTR_BINARYOP(ctAtomType_Int, as_int, +); 
	goto next;

opSubI: 
	INSTR_BINARYOP(ctAtomType_Int, as_int, -); 
	goto next;

opMulI: 
	INSTR_BINARYOP(ctAtomType_Int, as_int, *); 
	goto next;

opDivI: 
	INSTR_BINARYOP(ctAtomType_Int, as_int, /); 
	goto next;

opModI: 
	INSTR_BINARYOP(ctAtomType_Int, as_int, %); 
	goto next;

opNegI: 
	INSTR_UNARYOP(ctAtomType_Int, as_int, -); 
	goto next;

opIncI: 
	r1 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	a1.as_int++;
	ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_UInt);
	goto next;

opDecI: 
	r1 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	a1.as_int--;
	ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_UInt);
	goto next;

opAbsI: 
	INSTR_UNARYOP(ctAtomType_Int, as_int, labs); 
	goto next;

opAddU: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, +); 
	goto next;

opSubU: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, -); 
	goto next;

opMulU: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, *); 
	goto next;

opDivU: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, /); 
	goto next;

opModU: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, %); 
	goto next;

opIncU: 
	r1 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	a1.as_uint++;
	ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_UInt);
	goto next;

opDecU: 
	r1 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	a1.as_uint--;
	ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_UInt);
	goto next;

opAddF: 
	INSTR_BINARYOP(ctAtomType_Float, as_float, +); 
	goto next;

opSubF: 
	INSTR_BINARYOP(ctAtomType_Float, as_float, -); 
	goto next;

opMulF: 
	INSTR_BINARYOP(ctAtomType_Float, as_float, *); 
	goto next;

opDivF: 
	INSTR_BINARYOP(ctAtomType_Float, as_float, /); 
	goto next;

opNegF:
	INSTR_UNARYOP(ctAtomType_Float, as_float, -); 
	goto next;

opAbsF: 
	INSTR_UNARYOP(ctAtomType_Float, as_float, fabs); 
	goto next;

opLogicAnd: 
	INSTR_BINARYOP(ctAtomType_Bool, as_bool, &&); 
	goto next;

opLogicOr: 
	INSTR_BINARYOP(ctAtomType_Bool, as_bool, ||); 
	goto next;

opLogicNot: 	
	INSTR_UNARYOP(ctAtomType_Bool, as_bool, !); 
	goto next;

opLogicXor: 
	INSTR_BINARYOP(ctAtomType_Bool, as_bool, ^); 
	goto next;

opBitAnd: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, &); 
	goto next;

opBitOr: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, |); 
	goto next;

opBitNot: 
	INSTR_UNARYOP(ctAtomType_UInt, as_uint, ~); 
	goto next;

opBitXor: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, ^); 
	goto next;

opBitShl: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, <<); 
	goto next;

opBitShr: 
	INSTR_BINARYOP(ctAtomType_UInt, as_uint, >>); 
	goto next;

opCmpI: 
	INSTR_CMP(ctAtomType_Int, as_int); 
	goto next;

opCmpU: 
	INSTR_CMP(ctAtomType_UInt, as_uint); 
	goto next;

opCmpF: 
	INSTR_CMP(ctAtomType_Float, as_float); 
	goto next;

opEq: 
	INSTR_CMP_RESOLVER(==);
	goto next;

opNotEq: 
	INSTR_CMP_RESOLVER(!=);
	goto next;

opLess: 
	INSTR_CMP_RESOLVER(<);
	goto next;

opLessEq: 
	INSTR_CMP_RESOLVER(<=);
	goto next;

opGreater: 
	INSTR_CMP_RESOLVER(>);
	goto next;

opGreaterEq: 
	INSTR_CMP_RESOLVER(>=);
	goto next;

opJmp: 
	INSTR_JMP(); 
	goto next;

opJmpIf:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    if (a1.as_bool) { INSTR_JMP(); goto next; }
    ctx->ip += 4;
    goto next;

opJmpIfNot:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    if (!a1.as_bool) { INSTR_JMP(); goto next; }
    ctx->ip += 4;
    goto next;

opJmpAbs: 
	INSTR_JMPABS(); 
	goto next;

opJmpAbsIf:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    if (a1.as_bool) { INSTR_JMPABS(); goto next; }
    ctx->ip += 4;
    goto next;

opJmpAbsIfNot:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    if (!a1.as_bool) { INSTR_JMPABS(); goto next; }
    ctx->ip += 4;
    goto next;

opCall:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    r3 = instrs[ctx->ip++];
    r4 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	ct_ctx_loadAtom(ctx, r2, &a2, &t2);
    ct_ctx_callProcedure(ctx, a1.as_uint, a2.as_uint, r3, r4);
    goto next;

opReturn:
    ct_ctx_returnProcedure(ctx, (ctAtom){.as_uint=0}, ctAtomType_NoneType);
    goto next;

opReturnVal:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ct_ctx_returnProcedure(ctx, a1, t1);
    goto next;

opModCall:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    r3 = instrs[ctx->ip++];
    r4 = instrs[ctx->ip++];
	r5 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	ct_ctx_loadAtom(ctx, r2, &a2, &t2);
	ct_ctx_loadAtom(ctx, r3, &a3, &t3);
    ct_ctx_modcall(ctx, a1.as_uint, a2.as_uint, a3.as_uint, r4, r5);
    goto next;

opConNew:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a1, &t1);
    a2.as_container = ct_containers_newContainer(ctx->containers, a1.as_uint);
    if (ctx->containers->error.code != ctErrorCode_None) {
        ct_ctx_throwError(ctx, ctx->containers->error);
        return;
    }
    ct_ctx_storeAtom(ctx, r1, a2, ctAtomType_Container);
    goto next;

opConDel:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_uint=0}, ctAtomType_NoneType);
    goto next;

opConGet:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    r3 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a1, &t1);
    ct_ctx_loadAtom(ctx, r3, &a2, &t2);
    typed_atom = ct_containers_conGet(ctx->containers, a1.as_container, a2.as_uint);
    if (ctx->containers->error.code != ctErrorCode_None) {
        ct_ctx_throwError(ctx, ctx->containers->error);
        return;
    }
    ct_ctx_storeAtom(ctx, r1, typed_atom.atom, typed_atom.type);
    goto next;

opConSet:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    r3 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ct_ctx_loadAtom(ctx, r2, &a2, &t2);
    ct_ctx_loadAtom(ctx, r3, &a3, &t3);
    ct_containers_conSet(ctx->containers, a1.as_container, a2.as_uint, (ctTypedAtom){t3, a3});
    if (ctx->containers->error.code != ctErrorCode_None) {
        ct_ctx_throwError(ctx, ctx->containers->error);
        return;
    }
    goto next;

opConSize:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a1, &t1);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_uint = a1.as_container->size}, ctAtomType_UInt);
    goto next;


opConCopy:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a2, &t2);
    a1.as_container = ct_containers_conCopy(ctx->containers, a2.as_container);
    if (ctx->containers->error.code != ctErrorCode_None) {
        ct_ctx_throwError(ctx, ctx->containers->error);
        return;
    }
    ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_Container);
    goto next;

opConDeepCopy:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a2, &t2);
    a1.as_container = ct_containers_conDeepCopy(ctx->containers, a2.as_container);
    if (ctx->containers->error.code != ctErrorCode_None) {
        ct_ctx_throwError(ctx, ctx->containers->error);
        return;
    }
    ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_Container);
    goto next;

opConResize:
    r1 = instrs[ctx->ip++];
	r2 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ct_ctx_loadAtom(ctx, r2, &a2, &t2);
    ct_containers_conResize(ctx->containers, a1.as_container, a2.as_uint);
    if (ctx->containers->error.code != ctErrorCode_None) {
        ct_ctx_throwError(ctx, ctx->containers->error);
        return;
    }
    goto next;

next:
    if (ctx->running) {
		CUTE_LOG("trace", "ip: 0x%08lX | instr: 0x%02X | ctx: %p\n", ctx->ip, instrs[ctx->ip], ctx); 
        goto *dispatch_table[instrs[ctx->ip++]];
    }

}