
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
    {"assert",      {instrAssert}},

    {"out",         {instrOut}},
    {"outb",        {instrOutBits}},

    {"mov",         {instrMov}},

    {"seti",        {instrSetI}},
    {"setu",        {instrSetU}},
    {"setf",        {instrSetF}},
    {"setb",        {instrSetB}},
    {"setn",        {instrSetN}},

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
    {"jmpif",       {instrJmpIf}},
    {"jmpifnot",    {instrJmpIfNot}},
    {"jma",         {instrJmpAbs}},
    {"jmaif",       {instrJmpAbsIf}},
    {"jmaifnot",    {instrJmpAbsIfNot}},
    {"call",        {instrCall}},
    {"ret",         {instrReturn}},
    {"retval",      {instrReturnVal}},
    {"modcall",     {instrModCall}},

    {"connew",      {instrConNew}},
    {"condel",      {instrConDel}},
    {"conget",      {instrConGet}},
    {"conset",      {instrConSet}},
    {"consize",     {instrConSize}},
    {"concopy",     {instrConCopy}},
    {"condeepcopy", {instrConDeepCopy}},
    {"conresize",   {instrConResize}},
    {"conload",     {instrConLoad}}
};


#endif // CODEGEN_INSTRSPEC_HPP