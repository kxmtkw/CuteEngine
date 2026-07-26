

#include <cstdint>
#include <cstring>
#include <memory>
#include "spec/nodes.hpp"
#include "codegen.hpp"

extern "C" {
#include "CuteInstr.h"
}

void ctCodeGen::visit(ctProgramNode& node) {

	mProcedures.reserve(node.procedures.size());

	for (auto& proc: node.procedures) {
		proc->accept(*this);
	}
}


void ctCodeGen::visit(ctProcedureNode& node) {

	ctImageProcedure proc;
	proc.id = node.id;
	proc.arg_count = node.arg_count;
	proc.bytecode_index = mInstructions.size();


	for (auto& stmt: node.stmts) {
		stmt->accept(*this);
	}

	if (proc.id >= mProcedures.size()) {
		mProcedures.resize(proc.id + 1);
	}

	mProcedures[proc.id] = proc;
}


void ctCodeGen::visit(ctOpNode& node) {
	
	mInstructions.push_back(node.opcode);

	for (auto& expr: node.operands) {
		expr->accept(*this);
	}
};


void ctCodeGen::visit(ctWordNode& node) {

};


void ctCodeGen::visit(ctSlotNode& node) {
	mInstructions.push_back(node.index);
}


void ctCodeGen::visit(ctIntNode& node) {
	ctInstructionSize* ptr = mInstructions.data() + mInstructions.size();
	mInstructions.resize(mInstructions.size() + 4);
	uint32_t i32 = std::stoi(node.val);
	std::memcpy(ptr, &i32, 4);
}

void ctCodeGen::visit(ctFloatNode& node) {
	ctInstructionSize* ptr = mInstructions.data() + mInstructions.size();
	mInstructions.resize(mInstructions.size() + 4);
	float f32 = std::stof(node.val);
	std::memcpy(ptr, &f32, 4);
}

void ctCodeGen::generate(ctProgramNode& node, std::string filepath) {

	mProcedures.clear();
	mInstructions.clear();
	mInstructions.reserve(256);

	node.accept(*this);

	mImage.header.instruction_count = mInstructions.size();
	mImage.header.procedure_count = mProcedures.size();
	mImage.instruction_pool = mInstructions.data();
	mImage.procedure_table = mProcedures.data();

	ct_image_write(&mImage, filepath.data());
}