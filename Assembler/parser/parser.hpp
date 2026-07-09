
#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

#include "tokenizer/tokens.hpp"
#include "utils/utils.hpp"
#include "nodes.hpp"
#include <memory>


class ctParser {

	ctUtils::ErrorCollector mErrors;
	ctTokenStream* mStream;

	std::unique_ptr<ctProgramNode>
	parseProgram();

	std::unique_ptr<ctProcedureNode>
	parseProcedure();

	std::unique_ptr<ctStatementNode>
	parseStmt();

	std::unique_ptr<ctLabelNode>
	parseLabel();

	std::unique_ptr<ctOpNode>
	parseOp();

	std::unique_ptr<ctExpressionNode>
	parseExpr();


	public:

	ctParser() = default;

	std::unique_ptr<ctProgramNode>
	parse(ctTokenStream* stream);
};


#endif // PARSER_PARSER_HPP