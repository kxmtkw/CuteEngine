
#ifndef CUTEASM_PROGRAM_HPP
#define CUTEASM_PROGRAM_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>


extern "C" {
	#include "CuteInstr.h"
}

using std::byte;

struct CtProcedure {
	unsigned int id;
	unsigned int arg_count;
	std::vector<byte> instructions;
};

struct CtProgram {
	std::vector<std::unique_ptr<CtProcedure>> procedures;
};


#endif // CUTEASM_PROGRAM_HPP