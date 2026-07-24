
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
#include "error/error.h"
#include "utils/utils.h"



struct _ctEngine {
	ctImage*            image;
	ctObjectManager*    object_manager;
	uint8_t             exit_code;
};

typedef struct _ctEngine ctEngine;


ctEngine*
ct_engine_init() {
	CUTE_LOG("engine", "vroom vroom\n");
	ctEngine* engine = (ctEngine*) malloc(sizeof(ctEngine));
	engine->image = NULL;
	engine->object_manager = ct_objects_init();
	engine->exit_code = 0;
	return engine;
}


// End the engine and free all resources
void
ct_engine_end(ctEngine* engine) {

	ct_objects_end(&engine->object_manager);

	if (engine->image->header.magic_id == ctMagicId) {
		CUTE_LOG("engine", "Freeing image resources.\n");
		ct_image_free(engine->image);
	}

	CUTE_LOG("engine", "Ending engine.\n");
	exit(engine->exit_code);
}

// Load an image file. For now, only one image can be loaded.
void
ct_engine_load(ctEngine* engine, const char* filepath) {
	
	CUTE_LOG("engine", "Loading image file: %s\n", filepath);

	engine->image = (ctImage*) malloc(sizeof(ctImage));
	ctImageCode code = ct_image_read(engine->image, filepath);

	ctError error;

	switch (code) {

		case ctImageCode_Success:
			CUTE_LOG("engine", "Image loaded successfully.\n");
			break;

		case ctImageCode_FileNotFound:
			error.code = ctErrorCode_Engine;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Cannot find image file: %s", filepath
			);
			break;

		case ctImageCode_ReadWriteFailure:
			error.code = ctErrorCode_Engine;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Failed to read image file: %s", filepath
			);
			break;

		case ctImageCode_InvalidImage:
			error.code = ctErrorCode_Engine;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Invalid image file: %s", filepath
			);
			break;

		default:
			error.code = ctErrorCode_Engine;
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

// Make a new context
ctContext*
ct_engine_newCtx(ctEngine* engine, uint32_t procedure_id) {
	return ct_ctx_new(engine->image, engine->object_manager, procedure_id);
}

void
ct_engine_delCtx(ctEngine* engine, ctContext* ctx) {
	ct_ctx_del(ctx);
}

void
ct_engine_runCtx(ctEngine* engine, ctContext* ctx) {

	ct_exec(ctx);
	
	if (ctx->error.code) {
		ct_error_print(ctx->error);
	}

	engine->exit_code = ctx->exit_code;
}


void
ct_engine_run(ctEngine* engine) {

	ctContext* ctx = ct_ctx_new(engine->image, engine->object_manager, 0);

	ct_engine_runCtx(engine, ctx);

	ct_ctx_del(ctx);
}