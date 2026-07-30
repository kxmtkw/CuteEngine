#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

#include "codegen/codegen.hpp"
#include "parser/parser.hpp"
#include "spec/nodes.hpp"
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

	ctTokenizer tokenizer;
	ctParser parser;
	ctCodeGen generator;

	auto stream = tokenizer.tokenize(source);
	auto program = parser.parse(&stream);
	auto printer = ctNodePrinter();
	program->accept(printer);
	generator.generate(*program, outfile);

}