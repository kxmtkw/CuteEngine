
#ifndef CODEGEN_CODEGEN_HPP
#define CODEGEN_CODEGEN_HPP


#include "spec/nodes.hpp"
#include <memory>
#include <vector>

extern "C" {
	#include "CuteInstr.h"
}

class ctCodeGen: ctNodeVisitor {

	ctImage mImage;
	std::vector<ctImageProcedure> mProcedures;
	std::vector<ctInstructionSize> mInstructions;

public:

	void visit(ctProgramNode& node);
	void visit(ctProcedureNode& node);
	void visit(ctOpNode& node);
	void visit(ctWordNode& node);
	void visit(ctSlotNode& node);
	void visit(ctIntNode& node);
	void visit(ctFloatNode& node);

	void generate(ctProgramNode& node, std::string filepath);
};

#endif // CODEGEN_CODEGEN_HPP