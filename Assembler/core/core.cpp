#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

#include "codegen/codegen.hpp"
#include "parser/parser.hpp"
#include "tokenizer/tokenizer.hpp"

#include "CuteAssembler.hpp"


void ctAssembler::assemble(std::string filepath) {

	std::ifstream file(filepath);

	if (!file.is_open()) {
		std::cerr << "Could not open file: " << filepath << std::endl;
		exit(1);
	}

	std::stringstream ss;
	ss << file.rdbuf();
	std::string content = ss.str();

	assembleString(std::move(content), std::format("{}.out", filepath));
};


void ctAssembler::assembleString(std::string source, std::string outfile) {

	auto stream = mTokenizer.tokenize(source);
	auto program = mParser.parse(&stream);
	mGenerator.generate(*program, outfile);
	
}