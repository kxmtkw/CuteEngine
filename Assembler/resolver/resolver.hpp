#ifndef RESOLVER_RESOLVER_HPP
#define RESOLVER_RESOLVER_HPP

#include "parser/nodes.hpp"
#include "symbols.hpp"

class ctResolver: ctNodeVisitor {

	std::map<std::string, ctSymbol> mSymbolMap;

	bool mFirstPass = false;

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

	std::map<std::string, ctSymbol> resolve(ctProgramNode& node);
};
#endif // RESOLVER_RESOLVER_HPP