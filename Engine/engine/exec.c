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
ct_loadBytes(instrs, &ctx->ip, 4, &i32); \
ctx->ip += i32; \
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
ct_incAtom(ctContext* ctx, uint8_t slot) {	
	if (ctx->current_frame->file.types[slot] == ctAtomType_Object) {
		ct_objects_decRef(ctx->objects, ctx->current_frame->file.atoms[slot].as_object);
		ctx->current_frame->object_field_count--;
	};
	ctx->current_frame->file.atoms[slot].as_uint++;
	ctx->current_frame->file.types[slot] = ctAtomType_Primitive;
};


static inline void
ct_decAtom(ctContext* ctx, uint8_t slot) {	
	if (ctx->current_frame->file.types[slot] == ctAtomType_Object) {
		ct_objects_decRef(ctx->objects, ctx->current_frame->file.atoms[slot].as_object);
		ctx->current_frame->object_field_count--;
	};
	ctx->current_frame->file.atoms[slot].as_uint--;
	ctx->current_frame->file.types[slot] = ctAtomType_Primitive;
};


static inline void 
ct_out(uint8_t fmt, ctAtom atom, ctAtomTypeSize type) {

	switch (fmt) {
		
		case 0:
			printf("[ binary ");
			for (int i = 63; i >= 0; i--) {
				printf("%d", (int)((atom.raw >> i) & 1));
				if (i % 8 == 0 && i != 0) printf(" ");
			}
			printf(" ]\n");
			break;
		case 1:
			printf("[ hexadecimal 0x%016lX ]\n", (uint64_t)atom.raw); break;
		case 2:
			printf("[ int %ld ]\n", atom.as_int); break;
		case 3:
			printf("[ uint %lu ]\n", atom.as_uint); break;
		case 4:
			printf("[ float %f ]\n", atom.as_float); break;
		case 5:
			printf("[ bool %s ]\n", atom.as_bool ? "true" : "false"); break;
		case 6:
			printf("[ object %p ]\n", atom.as_object); break;
		default:
			printf("[ hexadecimal 0x%016lX (unk-format %u) ]\n", (uint64_t)atom.raw, fmt);
	}
}

#ifndef CUTE_CONF_DEBUG

#define NEXT() if (ctx->running) {goto *dispatch_table[instrs[ctx->ip++]];};

#else 

#define NEXT() if (ctx->running) { \
	CUTE_LOG("trace", "ip: 0x%08lX | instr: 0x%02X | ctx: %p\n", ctx->ip, instrs[ctx->ip], ctx); goto *dispatch_table[instrs[ctx->ip++]]; \
};

#endif // CUTE_CONF_DEBUG


void ct_exec(ctContext* ctx) {


    static void* dispatch_table[] = {
        [instrNull] = &&opNull,
        [instrHalt] = &&opHalt,
        [instrOut] = &&opOut,
        [instrMov] = &&opMov,
		[instrCastI2F] = &&opCastI2F,
		[instrCastF2I] = &&opCastF2I,
		[instrCastU2F] = &&opCastU2F,
		[instrCastF2U] = &&opCastF2U,
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


	for (uint32_t i = 0; i < sizeof(dispatch_table)/sizeof(dispatch_table[0]); i++) {
		if (dispatch_table[i] == NULL) {
			dispatch_table[i] = &&opIllegalInstruction;
		}
	}


	ctInstructionSize* instrs = ctx->image->instruction_pool;

    uint8_t r1, r2, r3, r4, r5;
    int32_t i32;
    uint32_t u32;
    float f32;
    ctAtom a1, a2, a3;
    ctAtomType t1, t2, t3;
    ctTypedAtom typed_atom;
	
    NEXT();

opNull:
    NEXT();

opHalt:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ctx->exit_code = a1.as_uint;
    return;

opOut:
    r1 = instrs[ctx->ip++];
	r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a1, &t1);
    ct_out(r1, a1, t1);
    NEXT();

opMov:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_moveAtom(ctx, r2, r1);
    NEXT();

opCastI2F:
	r1 = instrs[ctx->ip++];
	r2 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r2, &a1, &t1);
	ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_float=a1.as_int}, ctAtomType_Primitive);
	NEXT();

opCastF2I:
	r1 = instrs[ctx->ip++];
	r2 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r2, &a1, &t1);

	if (!isfinite(a1.as_float) || a1.as_float > INT64_MAX || a1.as_float < INT64_MIN) {
		CUTE_ERROR(
			(&ctx->error),
			ctErrorCode_Overflow,
			"Unable to cast %f to int",
			a1.as_float, a1.raw
		);
		ct_ctx_throwError(ctx, ctx->error);
		return;
	};

	ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_int=a1.as_float}, ctAtomType_Primitive);
	NEXT();

opCastU2F:
	r1 = instrs[ctx->ip++];
	r2 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r2, &a1, &t1);
	ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_float=a1.as_uint}, ctAtomType_Primitive);
	NEXT();

opCastF2U:
	r1 = instrs[ctx->ip++];
	r2 = instrs[ctx->ip++];
	ct_ctx_loadAtom(ctx, r2, &a1, &t1);

	if (!isfinite(a1.as_float) || a1.as_float > UINT64_MAX || a1.as_float < 0) {
		CUTE_ERROR(
			(&ctx->error),
			ctErrorCode_Overflow,
			"Unable to cast %f to uint",
			a1.as_float, a1.raw
		);
		ct_ctx_throwError(ctx, ctx->error);
		return;
	};

	ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_int=a1.as_float}, ctAtomType_Primitive);
	NEXT();

opSetI:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &i32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_int=i32}, ctAtomType_Primitive);
    NEXT();

opSetU:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &u32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_uint=u32}, ctAtomType_Primitive);
    NEXT();

opSetF:
    r1 = instrs[ctx->ip++];
    ct_loadBytes(instrs, &ctx->ip, 4, &f32);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_float=f32}, ctAtomType_Primitive);
    NEXT();

opAddI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, +); 
	NEXT();

opSubI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, -); 
	NEXT();

opMulI:
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, *); 
	NEXT();

opDivI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, /); 
	NEXT();

opModI: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, %); 
	NEXT();

opNegI: 
	INSTR_UNARYOP(ctAtomType_Primitive, as_int, -); 
	NEXT();

opIncI:
	r1 = instrs[ctx->ip++];
	ct_incAtom(ctx, r1);
	NEXT();

opDecI: 
	r1 = instrs[ctx->ip++];
	ct_decAtom(ctx, r1);
	NEXT();

opAbsI: 
	INSTR_UNARYOP(ctAtomType_Primitive, as_int, labs); 
	NEXT();

opAddU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, +); 
	NEXT();

opSubU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, -); 
	NEXT();

opMulU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, *); 
	NEXT();

opDivU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, /); 
	NEXT();

opModU: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, %); 
	NEXT();

opIncU: 
	r1 = instrs[ctx->ip++];
	ct_incAtom(ctx, r1);
	NEXT();

opDecU: 
	r1 = instrs[ctx->ip++];
	ct_decAtom(ctx, r1);
	NEXT();

opAddF: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_float, +); 
	NEXT();

opSubF: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_float, -); 
	NEXT();

opMulF: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_float, *); 
	NEXT();

opDivF: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_float, /); 
	NEXT();

opNegF:
	INSTR_UNARYOP(ctAtomType_Primitive, as_float, -); 
	NEXT();

opAbsF: 
	INSTR_UNARYOP(ctAtomType_Primitive, as_float, fabs); 
	NEXT();

opLogicAnd: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_bool, &&); 
	NEXT();

opLogicOr: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_bool, ||); 
	NEXT();

opLogicNot: 	
	INSTR_UNARYOP(ctAtomType_Primitive, as_bool, !); 
	NEXT();

opLogicXor: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_bool, ^); 
	NEXT();

opBitAnd: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, &); 
	NEXT();

opBitOr: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, |); 
	NEXT();

opBitNot: 
	INSTR_UNARYOP(ctAtomType_Primitive, as_uint, ~); 
	NEXT();

opBitXor: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, ^); 
	NEXT();

opBitShl: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, <<); 
	NEXT();

opBitShr: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_uint, >>); 
	NEXT();

opBitShrA: 
	INSTR_BINARYOP(ctAtomType_Primitive, as_int, >>); 
	NEXT();

opCmpI: 
	INSTR_CMP(ctAtomType_Primitive, as_int); 
	NEXT();

opCmpU: 
	INSTR_CMP(ctAtomType_Primitive, as_uint); 
	NEXT();

opCmpF: 
	INSTR_CMP(ctAtomType_Primitive, as_float); 
	NEXT();

opEq: 
	INSTR_CMP_RESOLVER(==);
	NEXT();

opNotEq: 
	INSTR_CMP_RESOLVER(!=);
	NEXT();

opLess: 
	INSTR_CMP_RESOLVER(<);
	NEXT();

opLessEq: 
	INSTR_CMP_RESOLVER(<=);
	NEXT();

opGreater: 
	INSTR_CMP_RESOLVER(>);
	NEXT();

opGreaterEq: 
	INSTR_CMP_RESOLVER(>=);
	NEXT();

opJmp: 
	INSTR_JMP(); 
	NEXT();

opJmpEq:
    if (ctx->cmp_diff == 0) { INSTR_JMP(); NEXT(); }
    ctx->ip += 4;
    NEXT();

opJmpNe:
    if (ctx->cmp_diff != 0) { INSTR_JMP(); NEXT(); }
    ctx->ip += 4;
    NEXT();

opJmpGt:
    if (ctx->cmp_diff > 0) { INSTR_JMP(); NEXT(); }
    ctx->ip += 4;
    NEXT();

opJmpGe:
    if (ctx->cmp_diff >= 0) { INSTR_JMP(); NEXT(); }
    ctx->ip += 4;
    NEXT();

opJmpLt:
    if (ctx->cmp_diff < 0) { INSTR_JMP(); NEXT(); }
    ctx->ip += 4;
    NEXT();

opJmpLe:
    if (ctx->cmp_diff <= 0) { INSTR_JMP(); NEXT(); }
    ctx->ip += 4;
    NEXT();

opCall:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    r3 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ct_ctx_callProcedure(ctx, a1.as_uint, r2, r3);
    NEXT();

opReturn:
    ct_ctx_returnProcedure(ctx, (ctAtom){.as_uint=0}, ctAtomType_Primitive);
    NEXT();

opReturnVal:
    r1 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r1, &a1, &t1);
    ct_ctx_returnProcedure(ctx, a1, t1);
    NEXT();

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
    NEXT();

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
    NEXT();


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
    NEXT();

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
    NEXT();

opConSize:
    r1 = instrs[ctx->ip++];
    r2 = instrs[ctx->ip++];
    ct_ctx_loadAtom(ctx, r2, &a1, &t1);
	CHECK_IF_OBJECT(t1);
    ct_ctx_storeAtom(ctx, r1, (ctAtom){.as_uint = ct_container_size(ctx->objects, a1.as_object)}, ctAtomType_Primitive);
    NEXT();


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
    NEXT();

opIllegalInstruction:
	CUTE_ERROR(
		(&ctx->error),
		ctErrorCode_Engine,
		"Illegal instruction: 0x%x",
		instrs[--ctx->ip]
	);
	ct_ctx_throwError(ctx, ctx->error);
	return;
}