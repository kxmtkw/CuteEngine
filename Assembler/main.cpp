#include "parser/nodes.hpp"
#include "parser/parser.hpp"
#include "resolver/resolver.hpp"
#include "tokenizer/tokenizer.hpp"
#include "tokenizer/tokens.hpp"
#include <fstream>
#include <sstream>


int main() {
	ctTokenizer tokenizer;
	
	std::ifstream file("test.cta");
	std::stringstream ss;
	ss << file.rdbuf();
	std::string content = ss.str();

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

	ctResolver resolver;
	auto map = resolver.resolve(*program);

	return 0;
}