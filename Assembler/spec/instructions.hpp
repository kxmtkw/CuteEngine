
#ifndef CODEGEN_INSTRSPEC_HPP
#define CODEGEN_INSTRSPEC_HPP

#include <cstddef>
#include <map>
#include <string>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}


static const std::map<std::string, ctInstruction> ctInstrMap = {
    {"halt",        {instrHalt}},
    {"null",        {instrNull}},

    {"out",         {instrOut}},

    {"mov",         {instrMov}},

    {"seti",        {instrSetI}},
    {"setu",        {instrSetU}},
    {"setf",        {instrSetF}},


	{"i2f",        {instrCastI2F}},
    {"f2i",        {instrCastF2I}},
    {"u2f",        {instrCastU2F}},
	{"f2u",        {instrCastF2U}},

    {"addi",        {instrAddI}},
    {"subi",        {instrSubI}},
    {"muli",        {instrMulI}},
    {"divi",        {instrDivI}},
    {"modi",        {instrModI}},
    {"negi",        {instrNegI}},
    {"absi",        {instrAbsI}},
    {"inci",        {instrIncI}},
    {"deci",        {instrDecI}},

    {"addu",        {instrAddU}},
    {"subu",        {instrSubU}},
    {"mulu",        {instrMulU}},
    {"divu",        {instrDivU}},
    {"modu",        {instrModU}},
    {"incu",        {instrIncU}},
    {"decu",        {instrDecU}},

    {"addf",        {instrAddF}},
    {"subf",        {instrSubF}},
    {"mulf",        {instrMulF}},
    {"divf",        {instrDivF}},
    {"negf",        {instrNegF}},
    {"absf",        {instrAbsF}},

    {"and",         {instrLogicAnd}},
    {"or",          {instrLogicOr}},
    {"not",         {instrLogicNot}},
    {"xor",         {instrLogicXor}},
    {"band",        {instrBitAnd}},
    {"bor",         {instrBitOr}},
    {"bxor",        {instrBitXor}},
    {"bnot",        {instrBitNot}},
    {"bshl",        {instrBitShl}},
    {"bshr",        {instrBitShr}},
	{"bshra",       {instrBitShrA}},

    {"cmpi",        {instrCmpI}},
    {"cmpu",        {instrCmpU}},
    {"cmpf",        {instrCmpF}},
    {"eq",          {instrEq}},
    {"neq",         {instrNotEq}},
    {"less",        {instrLess}},
    {"lesseq",      {instrLessEq}},
    {"great",       {instrGreater}},
    {"greateq",     {instrGreaterEq}},

    {"jmp",         {instrJmp}},
    {"jmpeq",       {instrJmpEq}},
    {"jmpne",       {instrJmpNe}},
    {"jmpgt",       {instrJmpGt}},
    {"jmpge",       {instrJmpGe}},
    {"jmplt",       {instrJmpLt}},
	{"jmple",       {instrJmpLe}},
    {"call",        {instrCall}},
    {"ret",         {instrReturn}},
    {"retval",      {instrReturnVal}},
    {"modcall",     {instrModCall}},

    {"connew",      {instrConNew}},
    {"conget",      {instrConGet}},
    {"conset",      {instrConSet}},
    {"consize",     {instrConSize}},
    {"concopy",     {instrConCopy}},
};


#endif // CODEGEN_INSTRSPEC_HPP