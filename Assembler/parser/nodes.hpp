
#ifndef PARSER_NODES_HPP
#define PARSER_NODES_HPP

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
struct ctNode;

struct ctProgramNode;

struct ctObjectNode;
	struct ctProcedureNode;

struct ctStatementNode;
	struct ctOpNode;
	struct ctLabelNode;

struct ctExpressionNode;
	struct ctWordNode;
	struct ctSlotNode;
	struct ctDirectiveNode;
	struct ctIntNode;
	struct ctFloatNode;


enum class NodeType {
	Program,
	Procedure,
	Label,
	Op,
	Word,
	Slot,
	Directive,
	Int,
	Float
};


struct ctNodeVisitor {

    virtual void visit(ctProgramNode& node) = 0;
    virtual void visit(ctProcedureNode& node) = 0;
    virtual void visit(ctOpNode& node) = 0;
    virtual void visit(ctLabelNode& node) = 0;
    virtual void visit(ctWordNode& node) = 0;
    virtual void visit(ctSlotNode& node) = 0;
    virtual void visit(ctDirectiveNode& node) = 0;
    virtual void visit(ctIntNode& node) = 0;
    virtual void visit(ctFloatNode& node) = 0;

    virtual ~ctNodeVisitor() = default;
};


struct ctNode {
    virtual NodeType getType() const = 0;
    virtual void accept(ctNodeVisitor& visitor) = 0;
    virtual ~ctNode() = default;
};


// --- Group: Root / Structural Nodes ---

struct ctProgramNode : public ctNode {

	std::vector<std::unique_ptr<ctProcedureNode>> procedures;

    NodeType getType() const override { return NodeType::Program; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};

struct ctObjectNode : public ctNode {

};

struct ctProcedureNode : public ctObjectNode {

	std::string name;
	uint8_t arg_count;
	std::vector<std::unique_ptr<ctStatementNode>> stmts;

	unsigned int id;

    NodeType getType() const override { return NodeType::Procedure; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};

// --- Group: Statements ---

struct ctStatementNode : public ctNode {

};

struct ctOpNode : public ctStatementNode {

	std::string opcode;
	std::vector<std::unique_ptr<ctExpressionNode>> operands;

    NodeType getType() const override { return NodeType::Op; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};

struct ctLabelNode : public ctStatementNode {

	std::string name;

	unsigned int bytecode_index;

    NodeType getType() const override { return NodeType::Label; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};

// --- Group: Expressions ---

struct ctExpressionNode : public ctNode {

};

struct ctWordNode : public ctExpressionNode {

	std::string val;

    NodeType getType() const override { return NodeType::Word; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};

struct ctSlotNode : public ctExpressionNode {

	uint8_t index;
	
    NodeType getType() const override { return NodeType::Slot; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};

struct ctDirectiveNode : public ctExpressionNode {

	std::string dir;
	std::unique_ptr<std::unique_ptr<ctExpressionNode>> expr;

    NodeType getType() const override { return NodeType::Directive; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};

struct ctIntNode : public ctExpressionNode {

	std::string val;

    NodeType getType() const override { return NodeType::Int; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};

struct ctFloatNode : public ctExpressionNode {

	std::string val;

    NodeType getType() const override { return NodeType::Float; }
    void accept(ctNodeVisitor& visitor) override { visitor.visit(*this); }
};



class ctNodePrinter : public ctNodeVisitor {
	private:
		int indent_level = 0;
	
		void printIndent() {
			for (int i = 0; i < indent_level; ++i) {
				std::cout << "    "; // 2 spaces per indentation level
			}
		}
	
	public:
		void visit(ctProgramNode& node) override {
			printIndent();
			std::cout << "ProgramNode\n";
			
			indent_level++;
			for (auto& proc : node.procedures) {
				if (proc) proc->accept(*this);
			}
			indent_level--;
		}
	
		void visit(ctProcedureNode& node) override {
			printIndent();
			std::cout << "ProcedureNode [Name: " << node.name 
					  << ", Args: " << static_cast<int>(node.arg_count) 
					  << ", ID: " << node.id << "]\n";
			
			indent_level++;
			for (auto& stmt : node.stmts) {
				if (stmt) stmt->accept(*this);
			}
			indent_level--;
		}
	
		void visit(ctOpNode& node) override {
			printIndent();
			std::cout << "OpNode [Opcode: " << node.opcode << "]\n";
			
			indent_level++;
			for (auto& operand : node.operands) {
				if (operand) operand->accept(*this);
			}
			indent_level--;
		}
	
		void visit(ctLabelNode& node) override {
			printIndent();
			std::cout << "LabelNode [Name: " << node.name 
					  << ", Bytecode Index: " << node.bytecode_index << "]\n";
		}
	
		void visit(ctWordNode& node) override {
			printIndent();
			std::cout << "WordNode [Val: " << node.val << "]\n";
		}
	
		void visit(ctSlotNode& node) override {
			printIndent();
			std::cout << "SlotNode [Index: " << static_cast<int>(node.index) << "]\n";
		}
	
		void visit(ctDirectiveNode& node) override {
			printIndent();
			std::cout << "DirectiveNode [Dir: " << node.dir << "]\n";
			
			if (node.expr && *node.expr) {
				indent_level++;
				(*node.expr)->accept(*this);
				indent_level--;
			}
		}
	
		void visit(ctIntNode& node) override {
			printIndent();
			std::cout << "IntNode [Val: " << node.val << "]\n";
		}
	
		void visit(ctFloatNode& node) override {
			printIndent();
			std::cout << "FloatNode [Val: " << node.val << "]\n";
		}
};

#endif // PARSER_NODES_HPP