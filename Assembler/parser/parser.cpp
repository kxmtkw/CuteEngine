
#include <memory>
#include <string>
#include <utility>
#include "tokenizer/tokens.hpp"
#include "nodes.hpp"
#include "parser.hpp"


std::unique_ptr<ctProgramNode>
ctParser::parseProgram() {

	std::unique_ptr<ctProgramNode> program = std::make_unique<ctProgramNode>();

	while (mStream->peek().type != ctTokenType::EndOfFile) {
		
		if (mStream->expectToken("proc")) {
			auto proc = parseProcedure();
			if (proc) program->procedures.push_back(std::move(proc));
		}
	};

	return program;
}

std::unique_ptr<ctProcedureNode>
ctParser::parseProcedure() {
	
	std::unique_ptr<ctProcedureNode> procedure = std::make_unique<ctProcedureNode>();

	if (!mStream->expectTokenType(ctTokenType::Word, procedure->name)) {
		// did not get word.
	};

	std::string arg_count;

	if (! (mStream->expectToken("(") && mStream->expectTokenType(ctTokenType::Int, arg_count) && mStream->expectToken(")"))) {
		// did not get the req structure.
	};

	procedure->arg_count = std::stoul(arg_count);

	if (!mStream->expectToken("{")) {
		// nuh uh
	};

	while (!mStream->expectToken("}")) {
		auto stmt = parseStmt();
		if (stmt) procedure->stmts.push_back(std::move(stmt));
	}

	return procedure;
}


std::unique_ptr<ctStatementNode>
ctParser::parseStmt() {

	if (mStream->expectToken("@")) {

		if (mStream->expectToken("label")) {
			auto label = parseLabel();
			if (label) return label;
		} else {
			// unknown 
		}
	} else {
		auto op = parseOp();
		if (op) return op;
	};

	return nullptr;
}

std::unique_ptr<ctLabelNode>
ctParser::parseLabel() {

	std::unique_ptr<ctLabelNode> label = std::make_unique<ctLabelNode>();

	if (!mStream->expectTokenType(ctTokenType::Word, label->name)) {
		// failure
	};

	if (!mStream->expectToken(";")) {
		// failure
	}

	return label;
}


std::unique_ptr<ctOpNode>
ctParser::parseOp() {

	std::unique_ptr<ctOpNode> op = std::make_unique<ctOpNode>();
	
	if (!mStream->expectTokenType(ctTokenType::Word, op->opcode)) {
		// failure
	};

	while (!mStream->expectToken(";")) {
		auto expr = parseExpr();
		if (expr) op->operands.push_back(std::move(expr));
	}

	return op;
}


std::unique_ptr<ctExpressionNode>
ctParser::parseExpr() {

	std::string val;

	if (mStream->expectTokenType(ctTokenType::Int, val)) {
		std::unique_ptr<ctIntNode> int_node = std::make_unique<ctIntNode>();
		int_node->val = std::move(val);
		return int_node;

	} else if (mStream->expectTokenType(ctTokenType::Float, val)) {
		std::unique_ptr<ctFloatNode> float_node = std::make_unique<ctFloatNode>();
		float_node->val = std::move(val);
		return float_node;

	} else if (mStream->expectTokenType(ctTokenType::Word, val)) {
		std::unique_ptr<ctWordNode> word_node = std::make_unique<ctWordNode>();
		word_node->val = std::move(val);
		return word_node;

	} else if (mStream->expectToken("$")) {
		
		if (!mStream->expectTokenType(ctTokenType::Int, val)) {
			// failure
		}

		std::unique_ptr<ctSlotNode> slot = std::make_unique<ctSlotNode>();
		slot->index = std::stoul(val);
		return slot;
	}

	return nullptr;
}



std::unique_ptr<ctProgramNode>
ctParser::parse(ctTokenStream* stream) {
	mStream = stream;
	return parseProgram();
}