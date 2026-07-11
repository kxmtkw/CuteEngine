
#ifndef RESOLVER_SYMBOLS_HPP
#define RESOLVER_SYMBOLS_HPP

#include "parser/nodes.hpp"

enum class ctSymbolType {
	Procedure,
	Label
};

struct ctSymbol {
	ctSymbolType type;
	const ctNode& node;
	ctSymbol(ctSymbolType _type, const ctNode& _node): type(_type), node(_node) {};
	ctSymbol(const ctSymbol& other) : type(other.type), node(other.node) {}
};

#endif // RESOLVER_SYMBOLS_HPP

