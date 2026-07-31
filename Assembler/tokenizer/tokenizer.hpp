#ifndef CUTEASM_TOKENIZER_HPP
#define CUTEASM_TOKENIZER_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "tokens.hpp"


class CtTokenizer {

	std::string mSource;
	std::vector<CtToken> mTokens;
	uint mCurrent;
	uint mSize;

	// get next char
	char next();
	// peek next char
	char peek();
	// backtrack by one char
	void backtrack();


	// check whether eol reached
	bool eof();

	// eat all spaces, new lines, tabs until there is none.
	void eat_whitspace();

	// read until new line
	void read_comment();

	// tokenize a word. a word will always start with an alphabet or underscore and can be continued using numbers.
	void tokenize_word();
	// tokenize a number. ints and floats included. does not handle negatives
	void tokenize_number();
	// tokenize a symbol. usually a single character
	void tokenize_symbol();
	// tokenize a string, backslashes are not resolved here but the TokenStream resolves them with getValue
	void tokenize_string();
	// tokenize a char, should use '', and only a single is allowed as usual
	void tokenize_char();


public:

	// tokenize a string and return CtTokenStream object.
	CtTokenStream tokenize(std::string source);
};

#endif // CUTEASM_TOKENIZER_HPP