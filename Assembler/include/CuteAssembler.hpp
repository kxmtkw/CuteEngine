
#ifndef CUTE_ASSEMBLER_HPP
#define CUTE_ASSEMBLER_HPP

#include <string>

class ctAssembler {

	public:

	void assemble(std::string filepath);

	void assembleString(std::string source, std::string outfile);
};

#endif // CUTE_ASSEMBLER_HPP


