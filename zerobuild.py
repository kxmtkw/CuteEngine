from zero import *

build = Build()
build.compiler = "gcc"
build.directory = ".build"

# Cute Engine

CuteEngine = StaticLibrary()

cute_engine_src = Path("Engine")

CuteEngine.source = Source(
	cute_engine_src / "engine" / "engine.c",
	cute_engine_src / "engine" / "exec.c",
	cute_engine_src / "engine" / "context.c",
	cute_engine_src / "object" / "object.c",
	cute_engine_src / "container" / "container.c",
	cute_engine_src / "image" / "image.c",
	cute_engine_src / "utils" / "utils.c",
)

CuteEngine.headers.public = cute_engine_src / "include"
CuteEngine.headers.private = cute_engine_src
CuteEngine.arguments = "-Wall",  "-Wextra",  "-g"

# Engine Entry Point

cute = Executable()
cute.source = Source("main/main.c")
cute.arguments = "-Wall",  "-Wextra",  "-g"
cute.link(CuteEngine)

# Assembler

cuteasm = Executable()
cuteasm.compiler = "g++"

cute_asm_src = Path("Assembler")

cuteasm.source = Source(
	cute_asm_src / "main.cpp",
	cute_asm_src / "tokenizer/tokenizer.cpp",
	cute_asm_src / "tokenizer/stream.cpp",
	cute_asm_src / "parser/parser.cpp",
	cute_asm_src / "codegen/codegen.cpp"
)

cuteasm.headers.private = cute_asm_src
cuteasm.arguments = "-Wall",  "-Wextra",  "-g"
cuteasm.link(CuteEngine)