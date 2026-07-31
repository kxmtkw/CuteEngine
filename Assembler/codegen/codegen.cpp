

#include <concepts>
#include <cstring>
#include <memory>
#include <sys/types.h>
#include "spec/nodes.hpp"
#include "codegen.hpp"

extern "C" {
	#include "CuteInstr.h"
}


static inline
int resolveJump(unsigned int label, unsigned int ref) {
	return label - ref;
}


void ctCodeGen::visit(ctProgramNode& node) {

	mProcedures.reserve(node.procedures.size());

	for (auto& proc: node.procedures) {
		proc->accept(*this);
	}
}


void ctCodeGen::visit(ctProcedureNode& node) {

	CtImageProcedure proc;
	proc.arg_count = node.arg_count;
	proc.bytecode_index = mInstructions.size();


	for (auto& stmt: node.stmts) {
		stmt->accept(*this);
	}

	if (node.id >= mProcedures.size()) {
		mProcedures.resize(node.id + 1);
	}

	for (auto label: mUnresolvedLabels) {
		unsigned int label_position = mLabelPositions[label.second];
		unsigned int current_position = label.first;
		int offset = resolveJump(label_position, current_position + 4);

		CtInstrSize* ptr = mInstructions.data() + current_position;
		std::memcpy(ptr, &offset, sizeof(offset));
	}

	mProcedures[node.id] = proc;
}


void ctCodeGen::visit(ctLabelNode& node) {

	node.position = mInstructions.size();
	mLabelPositions[node.name] = node.position;

};


void ctCodeGen::visit(ctOpNode& node) {
	
	mInstructions.push_back(node.opcode);

	for (auto& expr: node.operands) {
		expr->accept(*this);
	}
};


void ctCodeGen::visit(ctWordNode& node) {

	if (mLabelPositions.contains(node.val)) {
		
		unsigned int label_position = mLabelPositions[node.val];
		unsigned int current_position = mInstructions.size() + 4;
		
		CtInstrSize* ptr = mInstructions.data() + mInstructions.size();
		mInstructions.resize(mInstructions.size() + 4);

		int offset = resolveJump(label_position, current_position);
		std::memcpy(ptr, &offset, 4);

	} else {
		mUnresolvedLabels[mInstructions.size()] = node.val;
		mInstructions.resize(mInstructions.size() + 4);
	}

};


void ctCodeGen::visit(ctSlotNode& node) {
	mInstructions.push_back(node.index);
}


void ctCodeGen::visit(ctIntNode& node) {
	CtInstrSize* ptr = mInstructions.data() + mInstructions.size();
	mInstructions.resize(mInstructions.size() + 4);
	uint32_t i32 = std::stoi(node.val);
	std::memcpy(ptr, &i32, 4);
}

void ctCodeGen::visit(ctFloatNode& node) {
	CtInstrSize* ptr = mInstructions.data() + mInstructions.size();
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