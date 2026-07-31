#include <cstddef>
#include <memory>
#include <string.h>
#include <string>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

#include "tokenizer/tokens.hpp"
#include "spec/instructions.hpp"
#include "spec/program.hpp"

#include "resolver.hpp"


static inline void
add_bytes_to_vector(std::vector<byte>& vector, unsigned int size, void* src) {
	size_t old_size = vector.size();
	vector.resize(vector.size() + size);
	byte* ptr = vector.data() + old_size;
	memcpy(ptr, src, size);
}


std::unique_ptr<CtProcedure>
CtResolver::resolve_procedure() {

	auto procedure = std::make_unique<CtProcedure>();
	
	std::string val;

	if (!mStream.expect_type(CtTokenType::Int, &val)) {
		// invalid
	}

	procedure->id = std::stoul(val);

	if (!(
		mStream.expect_token("(") &&
		mStream.expect_type(CtTokenType::Int, &val) &&
		mStream.expect_token("(")
	)) {
		// invalid
	}

	procedure->arg_count = std::stoul(val);
	procedure->address = mProgram->instructions.size();

	if (!mStream.expect_token("{")) {
		// invalid
	}

	while (!mStream.eof()) {

		if (mStream.expect_token("}")) {
			break;
		}

		parse_instruction();
	}

	return procedure;
}


void
CtResolver::parse_instruction() {

	std::string val;

	if (!mStream.expect_type(CtTokenType::Word, &val)) {
		
	}
	
	if (!CtInstrMap.contains(val)) {
		// error
	}

	CtInstrSize instr = CtInstrMap.at(val);

	mProgram->instructions.push_back((byte)instr);
	
	while (!mStream.expect_token(";")) {

		if (mStream.expect_token("$")) {

			mStream.expect_type(CtTokenType::Int, &val);
			byte slot_index = (byte) std::stoi(val);
			mProgram->instructions.push_back(slot_index);

		} else if (mStream.expect_type(CtTokenType::Int, &val)) {

			int number = std::stoi(val);
			add_bytes_to_vector(mProgram->instructions, 4, &number);

		} else if (mStream.expect_type(CtTokenType::Float, &val)) {
			
			float number = std::stof(val);
			add_bytes_to_vector(mProgram->instructions, 4, &number);

		}

	}
};


std::unique_ptr<CtProgram>
CtResolver::resolve(CtTokenStream stream) {

	mStream = stream;
	mProgram = std::make_unique<CtProgram>();

	while (!mStream.eof()) {
		
		if (mStream.expect_token("proc")) {

			auto procedure = resolve_procedure();
			mProgram->procedures.push_back(std::move(procedure));

		}
	}

	return std::move(mProgram);
}

