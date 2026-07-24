from zero import *

build = Build()
build.compiler = "gcc"
build.directory = "build"

src = Path("Engine")

CuteEngine = StaticLibrary()
CuteEngine.source = Source(
	src / "engine" / "engine.c",
	src / "engine" / "exec.c",
	src / "engine" / "context.c",
	src / "object" / "object.c",
	src / "container" / "container.c",
	src / "image" / "image.c",
	src / "utils" / "utils.c",
)
CuteEngine.headers.public = src, src / "include"
CuteEngine.arguments = "-w", "-fpermissive"

cute = Executable()
cute.source = Source(
	"main/main.c"
)
cute.arguments = "-w", "-fpermissive"
cute.link(CuteEngine)


src = Path("Assembler")
cuteasm = Executable()
cuteasm.name = "cute-asm"
cuteasm.compiler = "g++"
cuteasm.source = Source(
	src / "main.cpp",
	src / "tokenizer/tokenizer.cpp",
	src / "tokenizer/stream.cpp",
	src / "parser/parser.cpp",
	src / "codegen/codegen.cpp"
)
cuteasm.headers.private = src
cuteasm.arguments = "-w", "-fpermissive"
cuteasm.link(CuteEngine)