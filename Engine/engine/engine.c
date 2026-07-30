#include <stdint.h>
#include <stdlib.h>

#include "Cute.h"

#include "common/atom.h"
#include "common/config.h"
#include "common/error.h"

#include "image/image.h"
#include "objects/manager.h"
#include "utils/utils.h"

#include "engine/context.h"
#include "engine/engine.h"


void
ct_engine_init(CtEngine* engine) {
	CT_LOG("engine", "vroom vroom\n");
	engine->exit_code = 0;
}


void
ct_engine_end(CtEngine* engine) {

	if (engine->image.header.magic_id == ct_magic_id) {
		CT_LOG("engine", "Freeing image resources.\n");
		ct_image_free(&engine->image);
	}

	CT_LOG("engine", "Ending engine.\n");
	exit(engine->exit_code);
}


void
ct_engine_load(CtEngine* engine, const char* filepath) {
	
	CT_LOG("engine", "Loading image file: %s\n", filepath);

	CtImageStatus code = ct_image_read(&engine->image, filepath);

	ctError error;

	switch (code) {

		case CT_IMAGE_STATUS_SUCCESS:
			CT_LOG("engine", "Image loaded successfully.\n");
			break;

		case CT_IMAGE_STATUS_FILE_NOT_FOUND:
			error.code = ctErrorCode_Engine;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Cannot find image file: %s", filepath
			);
			break;

		case CT_IMAGE_STATUS_READ_WRITE_FAILURE:
			error.code = ctErrorCode_Engine;
			ct_utils_format(
				error.msg,
				sizeof(error.msg),
				"Failed to read image file: %s", filepath
			);
			break;

		case CT_IMAGE_CORRUPTED_FILE:
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

	if (code != CT_IMAGE_STATUS_SUCCESS) {
		ct_error_print(error);
		engine->exit_code = 1;
		ct_engine_end(engine);
		exit(1);
	}
}


void
ct_engine_run_context(CtEngine* engine, CtContext* ctx) {

	ct_engine_exec(engine, ctx);
	
	if (ctx->error.code) {
		ct_error_print(ctx->error);
	}

	engine->exit_code = ctx->exit_code;
}

void
cute_run(int argc, char** argv) {

	CtEngine engine;
	ct_engine_init(&engine);
	ct_engine_load(&engine, argv[0]);

	
	ct_engine_run_context(
		&engine,
		ct_ctx_new(&engine.image, ct_objects_init(), 0)
	);

	ct_engine_end(&engine);

}