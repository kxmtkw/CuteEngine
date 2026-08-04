from zero import *

build = Build()
build.compiler = "gcc"
build.directory = ".build"
build.arguments = "-Wall",  "-Wextra",  "-g"


# Cute Instructions

CuteInstr = StaticLibrary()
CuteInstr.source = Source(
	Path("Instr") / "image.c"
)
CuteInstr.headers.public = Path("Instr") / "include"


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

	src / "lib" / "buffer.c",
)

CuteEngine.link(CuteInstr)

CuteEngine.headers.public = src / "include"
CuteEngine.headers.private = src


# cute binary

cute = Executable()
cute.source = Source("main/engine.c")
cute.link(CuteEngine)
cute.link(CuteInstr)


# Cute Assembler

CuteAsm = StaticLibrary()
CuteAsm.compiler = "g++"

cute_asm_src = Path("Assembler")

CuteAsm.source = Source(
	cute_asm_src / "tokenizer" / "tokenizer.cpp",
	cute_asm_src / "tokenizer" / "stream.cpp",
	cute_asm_src / "resolver" / "resolver.cpp",
	cute_asm_src / "codegen" / "codegen.cpp",
	cute_asm_src / "assembler" / "assembler.cpp"
)
CuteAsm.headers.private = cute_asm_src
CuteAsm.headers.public = cute_asm_src / "include"
CuteAsm.link(CuteInstr)


# cuteasm binary

cuteasm = Executable()
cuteasm.source = Source("main/assembler.cpp")
cuteasm.link(CuteAsm)
cuteasm.link(CuteInstr)
cuteasm.compiler = CuteAsm.compiler