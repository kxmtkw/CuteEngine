
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuteAtom.h"
#include "CuteInstr.h"
#include "CuteEngine.h"

#include "object/object.h"
#include "modules/modules.h"
#include "modules/modulespec.h"
#include "context.h"
#include "exec.h"
#include "error.h"
#include "utils/utils.h"


void
ct_engine_init(ctEngine* engine) {
	CUTE_LOG("engine", "vroom vroom\n");
	engine->ctx = NULL;
	ct_objects_init(&engine->manager);
}

// End the engine and free all resources
void
ct_engine_end(ctEngine* engine) {

	if (engine->ctx) {
		ct_ctx_del(engine->ctx);
	}
	
	ct_objects_end(&engine->manager);

	if (engine->image.header.magic_id == ctMagicId) {
		CUTE_LOG("engine", "Freeing image resources.\n");
		ct_image_free(&engine->image);
	}

	CUTE_LOG("engine", "Ending engine.\n");
}

// Load an image file. For now, only one image can be loaded.
void
ct_engine_loadFile(ctEngine* engine, const char* filepath) {
	CUTE_LOG("engine", "Loading image file: %s\n", filepath);
	ctImageCode code = ct_image_read(&engine->image, filepath);

	ctError error;

	switch (code) {

		case ctImageCode_Success:
			CUTE_LOG("engine", "Image loaded successfully.\n");
			break;

		case ctImageCode_FileNotFound:
			error.code = ctErrorCode_EngineFailure;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Cannot find image file: %s", filepath
			);
			break;

		case ctImageCode_ReadWriteFailure:
			error.code = ctErrorCode_EngineFailure;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Failed to read image file: %s", filepath
			);
			break;

		case ctImageCode_InvalidImage:
			error.code = ctErrorCode_EngineFailure;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Invalid image file: %s", filepath
			);
			break;

		default:
			error.code = ctErrorCode_EngineFailure;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Unknown failure while reading image file: %s", filepath
			);
			break;
	}

	if (code != ctImageCode_Success) {
		ct_error_print(error);
		engine->exit_code = 1;
		ct_engine_end(engine);
		exit(1);
	}
}

// Run the engine with the loaded image file
void
ct_engine_run(ctEngine* engine) {
	engine->ctx = ct_ctx_new(&engine->image, &engine->manager, 0);
	ct_exec(engine->ctx);
	
	if (engine->ctx->has_error) {
		ct_error_print(engine->ctx->error);
	}

	engine->exit_code = engine->ctx->exit_code;
}
