
#ifndef CUTEASM_INSTRUCTIONS_HPP
#define CUTEASM_INSTRUCTIONS_HPP

#include <map>
#include <string>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}


static const std::map<std::string, CtInstr> CtInstrMap = {
	{"halt",        CT_INSTR_HALT},
	{"null",        CT_INSTR_NULL},

	{"out",         CT_INSTR_OUT},

	{"mov",         CT_INSTR_MOV},

	{"seti",        CT_INSTR_SETI},
	{"setu",        CT_INSTR_SETU},
	{"setf",        CT_INSTR_SETF},

	{"i2f",         CT_INSTR_CAST_I2F},
	{"f2i",         CT_INSTR_CAST_F2I},
	{"u2f",         CT_INSTR_CAST_U2F},
	{"f2u",         CT_INSTR_CAST_F2U},

	{"addi",        CT_INSTR_ADDI},
	{"subi",        CT_INSTR_SUBI},
	{"muli",        CT_INSTR_MULI},
	{"divi",        CT_INSTR_DIVI},
	{"modi",        CT_INSTR_MODI},
	{"negi",        CT_INSTR_NEGI},
	{"absi",        CT_INSTR_ABSI},
	{"inci",        CT_INSTR_INCI},
	{"deci",        CT_INSTR_DECI},

	{"addu",        CT_INSTR_ADDU},
	{"subu",        CT_INSTR_SUBU},
	{"mulu",        CT_INSTR_MULU},
	{"divu",        CT_INSTR_DIVU},
	{"modu",        CT_INSTR_MODU},
	{"incu",        CT_INSTR_INCU},
	{"decu",        CT_INSTR_DECU},

	{"addf",        CT_INSTR_ADDF},
	{"subf",        CT_INSTR_SUBF},
	{"mulf",        CT_INSTR_MULF},
	{"divf",        CT_INSTR_DIVF},
	{"negf",        CT_INSTR_NEGF},
	{"absf",        CT_INSTR_ABSF},

	{"and",         CT_INSTR_LOGIC_AND},
	{"or",          CT_INSTR_LOGIC_OR},
	{"not",         CT_INSTR_LOGIC_NOT},
	{"xor",         CT_INSTR_LOGIC_XOR},
	{"band",        CT_INSTR_BIT_AND},
	{"bor",         CT_INSTR_BIT_OR},
	{"bxor",        CT_INSTR_BIT_XOR},
	{"bnot",        CT_INSTR_BIT_NOT},
	{"bshl",        CT_INSTR_BIT_SHL},
	{"bshr",        CT_INSTR_BIT_SHR},
	{"bshra",       CT_INSTR_BIT_SHRA},

	{"cmpi",        CT_INSTR_CMPI},
	{"cmpu",        CT_INSTR_CMPU},
	{"cmpf",        CT_INSTR_CMPF},
	{"eq",          CT_INSTR_EQ},
	{"neq",         CT_INSTR_NOT_EQ},
	{"less",        CT_INSTR_LESS},
	{"lesseq",      CT_INSTR_LESS_EQ},
	{"great",       CT_INSTR_GREATER},
	{"greateq",     CT_INSTR_GREATER_EQ},

	{"jmp",         CT_INSTR_JMP},
	{"jmpeq",       CT_INSTR_JMP_EQ},
	{"jmpne",       CT_INSTR_JMP_NE},
	{"jmpgt",       CT_INSTR_JMP_GT},
	{"jmpge",       CT_INSTR_JMP_GE},
	{"jmplt",       CT_INSTR_JMP_LT},
	{"jmple",       CT_INSTR_JMP_LE},
	{"call",        CT_INSTR_CALL},
	{"ret",         CT_INSTR_RETURN},
	{"retval",      CT_INSTR_RETURN_VAL},
	{"modcall",     CT_INSTR_MOD_CALL},

	{"connew",      CT_INSTR_CON_NEW},
	{"conget",      CT_INSTR_CON_GET},
	{"conset",      CT_INSTR_CON_SET},
	{"consize",     CT_INSTR_CON_SIZE},
	{"concopy",     CT_INSTR_CON_COPY},
};

#endif // CUTEASM_INSTRUCTIONS_HPP