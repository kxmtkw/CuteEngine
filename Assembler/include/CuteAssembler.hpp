
#ifndef CUTE_ASSEMBLER_HPP
#define CUTE_ASSEMBLER_HPP


#include "codegen/codegen.hpp"
#include "parser/parser.hpp"
#include "tokenizer/tokenizer.hpp"


class ctAssembler {

	ctTokenizer mTokenizer;
	ctParser mParser;
	ctCodeGen mGenerator;

	public:

	void assemble(std::string filepath);

	void assembleString(std::string source, std::string outfile);
};

#endif // CUTE_ASSEMBLER_HPP


