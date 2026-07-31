

#include <concepts>
#include <cstddef>
#include <cstring>
#include <memory>
#include <sys/types.h>
#include <vector>
#include "codegen.hpp"
#include "spec/program.hpp"

extern "C" {
	#include "CuteInstr.h"
}

void
CtCodeGen::generate(std::unique_ptr<CtProgram> program, std::string outpath) {

	std::vector<CtImageProcedure> image_procedures(program->procedures.size());

	for (auto& proc: program->procedures) {
		
		if (proc->id >= image_procedures.size()) {
			image_procedures.resize(proc->id);
		}

		CtImageProcedure image_proc = {.bytecode_index = proc->address, .arg_count = proc->arg_count};
		image_procedures[proc->id] = image_proc;
	}

	mImage.header.instruction_count = program->instructions.size();
	mImage.header.procedure_count = program->procedures.size();

	mImage.instruction_pool = (CtInstrSize*) program->instructions.data();
	mImage.procedure_table = image_procedures.data();
	
	ct_image_write(&mImage, outpath.data());
}