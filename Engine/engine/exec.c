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


#include "object/container.h"
#include "context.h"
#include "error/error.h"
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
ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_bool = ctx->cmp_diff OP 0 ? 1 : 0}, ctAtomType_Primitive); \


#define INSTR_JMP() \
ct_loadBytes(instrs, &ctx->ip, 8, &u64); \
ctx->ip = u64; \
if (ctx->ip >= ctx->image->header.instruction_count) { \
ctx->error = (ctError) {.code=ctErrorCode_Engine}; \
ct_utils_format(ctx->error.msg, sizeof(ctx->error.msg), "Out of range ip: 0x%08lX", ctx->ip); ct_ctx_throwError(ctx, ctx->error); };


#define CHECK_IF_OBJECT(TYPE) \
if (TYPE != ctAtomType_Object) { \
	ctx->error.code = ctErrorCode_Type; \
	ct_utils_format(ctx->error.msg, sizeof(ctx->error.msg), "Expected container, got primitive."); \
	ct_ctx_throwError(ctx, ctx->error); \
	return; \
}; \

static inline void
ct_loadBytes(ctInstructionSize* instrs, uint64_t* ip, uint32_t n, void* dest) {
	memcpy(dest, &instrs[*ip], n);
	*ip += n;
}


static inline void 
out(uint8_t fmt, ctAtom atom, ctAtomTypeSize type) {

	static const char* ct_atom_stringforms[] = {
		[0]    = "none",
		[1]    = "int",
		[2]    = "uint",
		[3]    = "float",
		[4]    = "bool",
		[5]    = "object"
	};

	switch (fmt) {
		
		case 0:
			printf("[ %s ]\n", ct_atom_stringforms[fmt]); break;
		case 1:
			printf("[ %s %ld ]\n", ct_atom_stringforms[fmt], atom.as_int); break;
		case 2:
			printf("[ %s %lu ]\n", ct_atom_stringforms[fmt], atom.as_uint); break;
		case 3:
			printf("[ %s %f ]\n", ct_atom_stringforms[fmt], atom.as_float); break;
		case 4:
			printf("[ %s %u ]\n", ct_atom_stringforms[fmt], atom.as_bool ? 1 : 0); break;
		case 5:
			printf("[ %s %p ]\n", ct_atom_stringforms[fmt], atom.as_object); break;
		case 6:
			printf("[ %s %p ]\n", ct_atom_stringforms[fmt], atom.as_object); break;
		case 7:
			printf("[ ");
			for (int i = 63; i >= 0; i--) {
				printf("%d", (int)((atom.raw >> i) & 1));
				if (i % 8 == 0 && i != 0) printf(" ");
			}
			printf(" ]\n");
			break;
		case 8:
			printf(" [ 0x%016lX ]\n", (uint64_t)atom.raw);
		default:
			printf("[ unknown format ]\n");
	}
}

void ct_exec(ctContext* ctx) {
    ctInstructionSize* instrs = ctx->image->instruction_pool;

    static void* dispatch_table[] = {
        [instrNull] = &&opNull,
        [instrHalt] = &&opHalt,
        [instrOut] = &&opOut,
        [instrMov] = &&opMov,
        [instrSetI] = &&opSetI,
        [instrSetU] = &&opSetU,
        [instrSetF] = &&opSetF,
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
		[instrBitShrA] = &&opBitShrA,
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
        [instrJmpEq] = &&opJmpEq,
        [instrJmpNe] = &&opJmpNe,
        [instrJmpGt] = &&opJmpGt,
        [instrJmpGe] = &&opJmpGe,
        [instrJmpLt] = &&opJmpLt,
		[instrJmpLe] = &&opJmpLe,
        [instrCall] = &&opCall,
        [instrReturn] = &&opReturn,
        [instrReturnVal] = &&opReturnVal,
		[instrModCall] = &&opModCall,
        [instrConNew] = &&opConNew,
        [instrConGet] = &&opConGet,
        [instrConSet] = &&opConSet,
        [instrConSize] = &&opConSize,
        [instrConCopy] = &&opConCopy,
    };

    uint8_t r1, r2, r3, r4, r5;
    int32_t i32;
    uint32_t u32;
    float f32;
	uint64_t u64;
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

opOut:
    r1 = instrs[ctx->ip++];
	r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a1, &t1);
    out(r1, a1, t1);
    goto next;

opMov:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_moveAtom(ctx, r2, r1);
    goto next;

opSetI:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &i32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_int=i32}, ctAtomType_Primitive);
    goto next;

opSetU:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &u32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_uint=u32}, ctAtomType_Primitive);
    goto next;

opSetF:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &f32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_float=i32}, ctAtomType_Primitive);
    goto next;

opAddI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, +); 
	goto next;

opSubI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, -); 
	goto next;

opMulI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, *); 
	goto next;

opDivI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, /); 
	goto next;

opModI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, %); 
	goto next;

opNegI: 
	INSTR_UNARYOP(ctAtomType_Primitive, as_int, -); 
	goto next;

opIncI:
	r1 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	a1.as_int++;
	ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_Primitive);
	goto next;

opDecI: 
	r1 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	a1.as_int--;
	ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_Primitive);
	goto next;

opAbsI: 
	INSTR_UNARYOP(ctAtomType_Primitive, as_int, labs); 
	goto next;

opAddU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, +); 
	goto next;

opSubU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, -); 
	goto next;

opMulU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, *); 
	goto next;

opDivU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, /); 
	goto next;

opModU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, %); 
	goto next;

opIncU: 
	r1 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	a1.as_uint++;
	ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_Primitive);
	goto next;

opDecU: 
	r1 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r1, &a1, &t1);
	a1.as_uint--;
	ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_Primitive);
	goto next;

opAddF: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_float, +); 
	goto next;

opSubF: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_float, -); 
	goto next;

opMulF: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_float, *); 
	goto next;

opDivF: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_float, /); 
	goto next;

opNegF:
	INSTR_UNARYOP(ctAtomType_Primitive, as_float, -); 
	goto next;

opAbsF: 
	INSTR_UNARYOP(ctAtomType_Primitive, as_float, fabs); 
	goto next;

opLogicAnd: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_bool, &&); 
	goto next;

opLogicOr: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_bool, ||); 
	goto next;

opLogicNot: 	
	INSTR_UNARYOP(ctAtomType_Primitive, as_bool, !); 
	goto next;

opLogicXor: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_bool, ^); 
	goto next;

opBitAnd: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, &); 
	goto next;

opBitOr: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, |); 
	goto next;

opBitNot: 
	INSTR_UNARYOP(ctAtomType_Primitive, as_uint, ~); 
	goto next;

opBitXor: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, ^); 
	goto next;

opBitShl: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, <<); 
	goto next;

opBitShr: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, >>); 
	goto next;

opBitShrA: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, >>); 
	goto next;

opCmpI: 
	INSTR_CMP(ctAtomType_Primitive, as_int); 
	goto next;

opCmpU: 
	INSTR_CMP(ctAtomType_Primitive, as_uint); 
	goto next;

opCmpF: 
	INSTR_CMP(ctAtomType_Primitive, as_float); 
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

opJmpEq:
    if (ctx->cmp_diff == 0) { INSTR_JMP(); goto next; }
    ctx->ip += 4;
    goto next;

opJmpNe:
    if (ctx->cmp_diff != 0) { INSTR_JMP(); goto next; }
    ctx->ip += 4;
    goto next;

opJmpGt:
    if (ctx->cmp_diff > 0) { INSTR_JMP(); goto next; }
    ctx->ip += 4;
    goto next;

opJmpGe:
    if (ctx->cmp_diff >= 0) { INSTR_JMP(); goto next; }
    ctx->ip += 4;
    goto next;

opJmpLt:
    if (ctx->cmp_diff < 0) { INSTR_JMP(); goto next; }
    ctx->ip += 4;
    goto next;

opJmpLe:
    if (ctx->cmp_diff <= 0) { INSTR_JMP(); goto next; }
    ctx->ip += 4;
    goto next;

opCall:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    r3 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ct_ctx_callProcedure(ctx, a1.as_uint, r2, r3);
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
    a2.as_object = ct_container_new(ctx->objects, a1.as_uint, &ctx->error);
    if (ctx->error.code) {
		ct_ctx_throwError(ctx, ctx->error);
        return;
	}
    ct_ctx_storeAtom(ctx, r1, a2, ctAtomType_Object);
    goto next;


opConGet:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    r3 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a1, &t1);
    ct_ctx_loadAtom(ctx, r3, &a2, &t2);
	CHECK_IF_OBJECT(t1);
    typed_atom = ct_container_get(ctx->objects, a1.as_object, a2.as_uint, &ctx->error);
	if (ctx->error.code) {
		ct_ctx_throwError(ctx, ctx->error);
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
	CHECK_IF_OBJECT(t1);
    ct_container_set(ctx->objects, a1.as_object, a2.as_uint, (ctTypedAtom){t3, a3}, &ctx->error);
    if (ctx->error.code) {
		ct_ctx_throwError(ctx, ctx->error);
        return;
	}
    goto next;

opConSize:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a1, &t1);
	CHECK_IF_OBJECT(t1);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_uint = ct_container_size(ctx->objects, a1.as_object)}, ctAtomType_Primitive);
    goto next;


opConCopy:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a2, &t2);
	CHECK_IF_OBJECT(t2);
    a1.as_object = ct_container_copy(ctx->objects, a2.as_object, &ctx->error);
    if (ctx->error.code) {
		ct_ctx_throwError(ctx, ctx->error);
        return;
	}
    ct_ctx_storeAtom(ctx, r1, a1, ctAtomType_Object);
    goto next;

next:
    if (ctx->running) {
		CUTE_LOG("trace", "ip: 0x%08lX | instr: 0x%02X | ctx: %p\n", ctx->ip, instrs[ctx->ip], ctx); 
        goto *dispatch_table[instrs[ctx->ip++]];
    }

}