from zero import *

build = Build()
build.compiler = "gcc"
build.directory = ".build"

# Cute Instructions

CuteInstr = StaticLibrary()
CuteInstr.source = Source(
	Path("Instr") / "image.c"
)
CuteInstr.headers.public = Path("Instr") / "include"
CuteInstr.arguments = "-Wall",  "-Wextra",  "-g"

# Cute Engine

CuteEngine = StaticLibrary()
CuteEngine.compiler = "gcc"
src = Path("Engine")

CuteEngine.source = Source(
	src / "engine" / "engine.c",
	src / "engine" / "exec.c",
	src / "engine" / "context.c",
	src / "objects" / "manager.c",
	src / "container" / "container.c",
	src / "utils" / "utils.c",
)
CuteEngine.link(CuteInstr)

CuteEngine.headers.public = src / "include"
CuteEngine.headers.private = src
CuteEngine.arguments = "-Wall",  "-Wextra",  "-g"

# cute binary

cute = Executable()
cute.source = Source("main/engine.c")
cute.arguments = "-Wall",  "-Wextra",  "-g"
cute.link(CuteEngine)
cute.link(CuteInstr)


# Cute Assembler

CuteAssembler = StaticLibrary()
CuteAssembler.compiler = "g++"

cute_asm_src = Path("Assembler")

CuteAssembler.source = Source(
	cute_asm_src / "tokenizer" / "tokenizer.cpp",
	cute_asm_src / "tokenizer" / "stream.cpp",
	cute_asm_src / "assembler" / "assembler.cpp"
)

CuteAssembler.headers.private = cute_asm_src
CuteAssembler.headers.public = cute_asm_src / "include"
CuteAssembler.link(CuteInstr)

# cuteasm binary

cuteasm = Executable()
cuteasm.source = Source("main/assembler.cpp")
cuteasm.arguments = "-Wall",  "-Wextra",  "-g"
cuteasm.link(CuteAssembler)
cuteasm.link(CuteInstr)
cuteasm.compiler = CuteAssembler.compiler