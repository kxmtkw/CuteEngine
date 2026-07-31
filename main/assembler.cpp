#include <iostream>
#include "CuteAsm.hpp"


int main(int argc, char** argv) {

	if (argc < 2) {
		std::cout << "Usage: %s <image_file>\n";
		return 2;
	}

	CtAssembler assembler;
	assembler.assemble(argv[1]);

	return 0;
}