
#ifndef CODEGEN_CODEGEN_HPP
#define CODEGEN_CODEGEN_HPP

#include "CuteInstr.h"
#include "parser/nodes.hpp"
#include "resolver/symbols.hpp"
#include <memory>
#include <vector>


class ctCodeGen: ctNodeVisitor {

	std::map<std::string, ctSymbol> mSymbolMap;
	ctImage mImage;
	std::vector<ctImageProcedure> mProcedures;
	std::vector<ctInstructionSize> mInstructions;

	std::map<unsigned int, ctSymbol> mPatches; 
	
	unsigned int mProcIndex = 0;

public:

	void visit(ctProgramNode& node);
	void visit(ctProcedureNode& node);
	void visit(ctOpNode& node);
	void visit(ctLabelNode& node);
	void visit(ctWordNode& node);
	void visit(ctSlotNode& node);
	void visit(ctDirectiveNode& node);
	void visit(ctIntNode& node);
	void visit(ctFloatNode& node);

	void generate(ctProgramNode& node, std::map<std::string, ctSymbol> symbolMap);
};

#endif // CODEGEN_CODEGEN_HPP