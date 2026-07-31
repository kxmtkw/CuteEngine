#include <cctype>
#include <string>
#include <vector>

#include "tokens.hpp"
#include "tokenizer.hpp"



char CtTokenizer::next() {
	if (mCurrent < mSize) {
		return mSource[mCurrent++];
	}
	return '\0';
}


char CtTokenizer::peek() {
	if (mCurrent < mSize) {
		return mSource[mCurrent];
	}
	return '\0';
}


void CtTokenizer::backtrack() {
	mCurrent--;
}


bool CtTokenizer::eof() {
	return mCurrent >= mSize;
}


void CtTokenizer::eatWhitspace() {

	char c;
	c = next();

	while ((c == ' ' or c == '\n' or c == '\t') and !eof()) {
		c = next();
	}

	backtrack();
}


void CtTokenizer::tokenizeWord() {
	char c;
	uint start = mCurrent;

	while (mCurrent < mSize) {
		c = peek();

		if (std::isalnum(c) or c == '_') {
			next();
			continue;
		}
		break;
	}
	mTokens.emplace_back(CtToken(CtTokenType::Word, start, mCurrent-start));
}


void CtTokenizer::tokenizeNumber() {

	uint start = mCurrent;
	char c;
	bool is_float = false;

	while (!eof()) {

		c = peek();

		if (std::isdigit(c)) {
			next();
			continue;
		}

		if (c == '.' and !is_float) {
			is_float = true;
			next();
			continue;
		}

		if (c == ' ' or c == '\n' or c == '\t') {
			break;
		}

		// error, illegal token sequence
	}

	mTokens.emplace_back(CtToken(CtTokenType::Number, start, mCurrent-start));
}


void CtTokenizer::tokenizeChar() {

	char c = next();
	
	if (c != '\'') {
		// error
	}

	uint start = mCurrent;
	next();

	c = next();
	
	if (c != '\'') {
		// error
	}

	mTokens.emplace_back(CtToken(CtTokenType::Char, start, 1));
};


void CtTokenizer::tokenizeString() {
	char c = next();
	
	if (c != '\"') {
		// error
	}

	uint start = mCurrent;

	c = next();

	while (c != '\"') {
		
		c = next();

		if (eof()) {
			// unterminated string
		}

		if (c == '\\') {
			next();
		}
	}

	mTokens.emplace_back(CtToken(CtTokenType::String, start, mCurrent-start-1));
}
	

void CtTokenizer::tokenizeSymbol() {
	char c = next();
	mTokens.emplace_back(CtToken(CtTokenType::Symbol, mCurrent-1, 1));
}



CtTokenStream CtTokenizer::tokenize(std::string source) {

	mSource = std::move(source);
	mCurrent = 0;
	mSize = mSource.size();
	mTokens = {};

	char c;

	while (mCurrent < mSize) {
		
		eatWhitspace();

		c = peek();

		if (std::isalpha(c) or c == '_') {
			tokenizeWord();
		}
		else if (std::isdigit(c)) {
			tokenizeNumber();
		}
		else if (c == '#') {
			while (peek() != '\n') { next(); }
		}
		else if (c == '\'') {
			tokenizeChar();
		}
		else if (c == '\"') {
			tokenizeString();
		}
		else {
			tokenizeSymbol();
		}
	}

	mTokens.emplace_back(CtToken(CtTokenType::EndOfFile, 0 ,0));

	return CtTokenStream(std::move(mSource), std::move(mTokens));
}