#include "parser/nodes.hpp"
#include "resolver.hpp"
#include "symbols.hpp"


void ctResolver::visit(ctProgramNode& node) {

	for (auto& proc: node.procedures) {
		proc->accept(*this);
	}

};


void ctResolver::visit(ctProcedureNode& node) {


	if (mSymbolMap.contains(node.name) && mFirstPass) {
		std::cout << "ERROR Procedure already exists\n";
	};

	mSymbolMap.emplace(node.name, ctSymbol({ctSymbolType::Procedure, node}));

	for (auto& stmt: node.stmts) {
		stmt->accept(*this);
	}
};

void ctResolver::visit(ctOpNode& node) {
	
	for (auto& expr: node.operands) {
		expr->accept(*this);
	}

}

void ctResolver::visit(ctLabelNode& node) {

	if (mSymbolMap.contains(node.name) && mFirstPass) {
		std::cout << "ERROR Label already exists\n";
	};

	mSymbolMap.emplace(node.name, ctSymbol({ctSymbolType::Procedure, node}));
}

void ctResolver::visit(ctWordNode& node) {

	if (mFirstPass) {return;}

	if (!mSymbolMap.contains(node.val)) {
		std::cout << "ERROR Unknown word\n";
	};
}

void ctResolver::visit(ctSlotNode& node) {

}

void ctResolver::visit(ctDirectiveNode& node) {
	node.expr->accept(*this);
}

void ctResolver::visit(ctIntNode& node) {

}

void ctResolver::visit(ctFloatNode& node) {

}


std::map<std::string, ctSymbol> ctResolver::resolve(ctProgramNode& node) {
	mSymbolMap.clear();
	mFirstPass = true;
	node.accept(*this);
	mFirstPass = false;
	node.accept(*this);
	return std::move(mSymbolMap);
}