#include <fstream>
#include <sstream>

#include "codegen/codegen.hpp"
#include "spec/nodes.hpp"
#include "parser/parser.hpp"
#include "tokenizer/tokenizer.hpp"
#include "tokenizer/tokens.hpp"



int main() {
	
	std::ifstream file("dev/test.cta");
	std::stringstream ss;
	ss << file.rdbuf();
	std::string content = ss.str();

	ctTokenizer tokenizer;
	auto stream = tokenizer.tokenize(content);

	while (stream.peek().type != ctTokenType::EndOfFile) {
		ctToken token = stream.next();
		std::cout << "[ " << tokenTypeToString(token.type) << " " << stream.getValue(token) << " ]\n";
	}
	stream.reset();

	ctParser parser;
	auto program = parser.parse(&stream);

	ctNodePrinter printer;
	program->accept(printer);

	ctCodeGen gen;
	gen.generate(*program, "dev/out");

	return 0;
}