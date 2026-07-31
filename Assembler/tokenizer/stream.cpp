#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "tokens.hpp"
#include "tokenizer.hpp"


std::string CtTokenStream::resolve_string(const std::string& str){

	std::string resolved_str;
	resolved_str.reserve(str.size());
	char c;

	for (uint i = 0; i < str.size(); i++) {
		c = str[i];
		
		if (c != '\\') {
			resolved_str += c;
			continue;
		}

		i++;
		c = str[i];

		switch (c) {
			case '\\': resolved_str += '\\'; break;
			case '\"': resolved_str += '\"'; break;
			case '\'': resolved_str += '\''; break;
			case 'n':  resolved_str += '\n'; break;
			case 't':  resolved_str += '\t'; break;
			case 'r':  resolved_str += '\r'; break;
			case 'b':  resolved_str += '\b'; break;
			case '0':  resolved_str += '\0'; break;
			default:   resolved_str += c;    break;
		}		
	}

	return resolved_str;
}


CtToken CtTokenStream::next() {
	if (mCurrent < mTokens.size()) {
		return mTokens[mCurrent++];
	}
	return mTokens[mTokens.size()-1];
};


CtToken CtTokenStream::peek() {
	if (mCurrent < mTokens.size()) {
		return mTokens[mCurrent];
	}
	return mTokens[mTokens.size()-1];
};


void CtTokenStream::backtrack() {
	if (mCurrent > 0) mCurrent--;
};


bool CtTokenStream::eof() {
	return mCurrent < mTokens.size();
};


void CtTokenStream::reset() {
	mCurrent = 0;
}

std::string CtTokenStream::get_value(const CtToken& token) {

	if (token.start > mSource.size() or token.start + token.len > mSource.size()) {
		return "";
	}

	if (token.type == CtTokenType::String) {
		return resolve_string(mSource.substr(token.start, token.len));
	}

	return mSource.substr(token.start, token.len);
}


bool CtTokenStream::expect_type(CtTokenType type, std::string* dest) {
	CtToken token = peek();

	if (token.type != type) {
		return false;
	}

	*dest = get_value(token);
	next();
	return true;
}


bool CtTokenStream::expect_token(const std::string& dest) {

	CtToken token = next();

	if (get_value(token) == dest) {
		return true;
	}
	else {
		backtrack();
		return false;
	}
	
}