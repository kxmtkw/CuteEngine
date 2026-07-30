from zero import *

build = Build()
build.compiler = "gcc"
build.directory = ".build"

# Cute Engine

CuteEngine = StaticLibrary()
CuteEngine.compiler = "gcc"
cute_engine_src = Path("Engine")

CuteEngine.source = Source(
	cute_engine_src / "engine" / "engine.c",
	cute_engine_src / "engine" / "exec.c",
	cute_engine_src / "engine" / "context.c",
	cute_engine_src / "object" / "object.c",
	cute_engine_src / "object" / "container.c",
	cute_engine_src / "image" / "image.c",
	cute_engine_src / "utils" / "utils.c",
)

CuteEngine.headers.public = cute_engine_src / "include"
CuteEngine.headers.private = cute_engine_src
CuteEngine.arguments = "-Wall",  "-Wextra",  "-g"

cute = Executable()
cute.source = Source("main/engine.c")
cute.arguments = "-Wall",  "-Wextra",  "-g"
cute.link(CuteEngine)

# Assembler

CuteAssembler = StaticLibrary()
CuteAssembler.compiler = "g++"

cute_asm_src = Path("Assembler")

CuteAssembler.source = Source(
	cute_asm_src / "tokenizer/tokenizer.cpp",
	cute_asm_src / "tokenizer/stream.cpp",
	cute_asm_src / "parser/parser.cpp",
	cute_asm_src / "codegen/codegen.cpp",
	cute_asm_src / "core/core.cpp"
)

CuteAssembler.headers.private = cute_asm_src
CuteAssembler.headers.public = cute_asm_src / "include"
CuteAssembler.link(CuteEngine)


cuteasm = Executable()
cuteasm.source = Source("main/assembler.cpp")
cuteasm.arguments = "-Wall",  "-Wextra",  "-g"
cuteasm.link(CuteAssembler)
cuteasm.link(CuteEngine)
cuteasm.compiler = CuteAssembler.compiler