

#include <memory>
#include "CuteInstr.h"
#include "parser/nodes.hpp"
#include "resolver/symbols.hpp"
#include "codegen.hpp"


void ctCodeGen::visit(ctProgramNode& node) {

	mInstructions.clear();
	mInstructions.reserve(256);

	mProcedures.clear();
	mProcedures.reserve(8);

	mProcIndex = 1;

	for (auto& proc: node.procedures) {
		proc->accept(*this);
	}
}


void ctCodeGen::visit(ctProcedureNode& node) {

	if (node.name == "main") {
		node.id = 0;
	} else {
		node.id = mProcIndex++;
	};

	ctImageProcedure proc;
	proc.id = node.id;
	proc.arg_count = node.arg_count;
	proc.bytecode_index = mInstructions.size();

	for (auto& stmt: node.stmts) {
		stmt->accept(*this);
	}
}


void ctCodeGen::visit(ctOpNode& node) {
	
	
	for (auto& expr: node.operands) {
		expr->accept(*this);
	}
};


void ctCodeGen::visit(ctLabelNode& node);
void ctCodeGen::visit(ctWordNode& node);
void ctCodeGen::visit(ctSlotNode& node);
void ctCodeGen::visit(ctDirectiveNode& node);
void ctCodeGen::visit(ctIntNode& node);
void ctCodeGen::visit(ctFloatNode& node);

void ctCodeGen::generate(ctProgramNode& node, std::map<std::string, ctSymbol> symbolMap) {
	mSymbolMap = std::move(symbolMap);
	node.accept(*this);
}