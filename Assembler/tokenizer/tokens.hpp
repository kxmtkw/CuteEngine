
#ifndef TOKENIZER_TOKENS_H
#define TOKENIZER_TOKENS_H

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

enum class CtTokenType {
	EndOfFile,
	Word,
	Number,
	String,
	Char,
	Symbol
};

struct CtToken {

	CtTokenType type;
	uint start;
	uint len;

	CtToken() = default;
	CtToken(CtTokenType t, uint s, uint l): type(t), start(s), len(l) {};
};


static inline std::string 
tokenTypeToString(CtTokenType type) {
    switch (type) {
        case CtTokenType::EndOfFile: return "EndOfFile";
        case CtTokenType::Word:      return "Word";
        case CtTokenType::Number:    return "Number";
        case CtTokenType::String:    return "String";
		case CtTokenType::Char:      return "Char";
        case CtTokenType::Symbol:    return "Symbol";
        default:                     return "Unknown";
    }
}


class CtTokenStream {

	std::string mSource;
	std::vector<CtToken> mTokens;
	uint mCurrent;

	// resolve backslashes of string tokens
	std::string resolveBackSlashes(const std::string& str);

public:

	// get the next token
	CtToken next();
	// take a look at the next token
	CtToken peek();
	// backtrack by one token
	void backtrack();
	// back to index 0
	void reset();

	// get the value of the token.
	std::string getValue(CtToken& token);

	// expect a certain token type and write its value to the string provided.
	bool expeCtTokenType(CtTokenType type, std::string& dest);

	// expect a certain string literal
	bool expeCtToken(const std::string& dest);
	
	CtTokenStream(std::string src, std::vector<CtToken> tokens): 
	mSource(std::move(src)), mTokens(std::move(tokens)), mCurrent(0) {}
};



#endif // TOKENIZER_TOKENS_H