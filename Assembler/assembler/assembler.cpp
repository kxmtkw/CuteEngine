#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

#include "tokenizer/tokenizer.hpp"
#include "tokenizer/tokens.hpp"
#include "codegen/codegen.hpp"


#include "CuteAsm.hpp"


void CtAssembler::assemble(std::string filepath) {

	std::ifstream file(filepath);

	if (!file.is_open()) {
		std::cerr << "Could not open file: " << filepath << std::endl;
		exit(1);
	}

	std::stringstream ss;
	ss << file.rdbuf();
	std::string content = ss.str();

	assemble_string(std::move(content), std::format("{}.out", filepath));
};


void CtAssembler::assemble_string(std::string source, std::string outfile) {

	CtTokenizer tokenizer;

	auto stream = tokenizer.tokenize(source);

	while (stream.peek().type != CtTokenType::EndOfFile) {
		CtToken token = stream.next();
		std::cout << stream.get_value(token) << std::endl;
	}

}