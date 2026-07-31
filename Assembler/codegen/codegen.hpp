
#ifndef CUTEASM_CODEGEN_HPP
#define CUTEASM_CODEGEN_HPP

#include <map>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

#include "spec/program.hpp"


class CtCodeGen {

	CtImage mImage;

public:

	void
	generate(std::unique_ptr<CtProgram> program, std::string outpath);

};

#endif // CUTEASM_CODEGEN_HPP