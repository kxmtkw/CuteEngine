
#ifndef CUTEASM_RESOLVER_HPP
#define CUTEASM_RESOLVER_HPP

#include <memory>

extern "C" {
	#include "CuteInstr.h"
}

#include "tokenizer/tokens.hpp"
#include "spec/instructions.hpp"
#include "spec/program.hpp"


class CtResolver {

	CtTokenStream mStream;
	std::unique_ptr<CtProgram> mProgram = nullptr;
	std::unique_ptr<CtProcedure> mCurrentProcedure = nullptr;


	std::unique_ptr<CtProcedure>
	resolve_procedure();

	void
	parse_instruction();

	public:

	std::unique_ptr<CtProgram>
	resolve(CtTokenStream stream);

};

#endif // CUTEASM_RESOLVER_HPP