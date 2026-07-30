
#ifndef CODEGEN_CODEGEN_HPP
#define CODEGEN_CODEGEN_HPP


#include "spec/nodes.hpp"
#include <map>
#include <vector>


extern "C" {
	#include "common/instructions.h"
	#include "image/image.h"
}

class ctCodeGen: ctNodeVisitor {

	CtImage mImage;
	std::vector<CtImageProcedure> mProcedures;
	std::vector<CtInstrSize> mInstructions;
	std::map<std::string, unsigned int> mLabelPositions;
	std::map<unsigned int, std::string> mUnresolvedLabels;

public:

	void visit(ctProgramNode& node);
	void visit(ctProcedureNode& node);
	void visit(ctLabelNode& node);
	void visit(ctOpNode& node);
	void visit(ctWordNode& node);
	void visit(ctSlotNode& node);
	void visit(ctIntNode& node);
	void visit(ctFloatNode& node);

	void generate(ctProgramNode& node, std::string filepath);
};

#endif // CODEGEN_CODEGEN_HPP