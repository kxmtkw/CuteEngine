#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "CuteConfig.h"
#include "error/error.h"
#include "utils/utils.h"

#include "object.h"


struct _ctObjectManager {
	ctObjectBucket**    buckets;
	uint32_t            bucket_count;
	uint32_t            bucket_capacity;
	ctObjectBucket**    empty_buckets;
	uint32_t            empty_bucket_count;
	uint32_t            empty_bucket_capacity;
	ctError             error;
};
typedef struct _ctObjectManager ctObjectManager;


ctObjectManager*
ct_objects_init() {

	ctObjectManager* manager = malloc(sizeof(ctObjectManager));
	
	manager->buckets = NULL;
	manager->bucket_count = 0;
	manager->bucket_capacity = 0;
	
	manager->empty_buckets = NULL;
	manager->empty_bucket_count = 0;
	manager->empty_bucket_capacity = 0;

	ct_objects_newBucket(manager);

	return manager;
}


void
ct_objects_end(ctObjectManager** manager_ptr) {

	ctObjectManager* manager = *manager_ptr;

	CUTE_LOG("objects", "Starting cleanup.\n");

	for (uint32_t i = 0; i < manager->bucket_count; i++) {
		ctObjectBucket* bucket = manager->buckets[i];

		for (uint32_t j = 0; j < sizeof(bucket->objects)/sizeof(bucket->objects[0]); j++) {
			if (ct_utils_isBitSet(bucket->bitmask, j)) {
				free(bucket->objects[j].data);
			}
		}
		free(bucket);
	}

	free(manager->buckets);
	manager->buckets = NULL;
	manager->bucket_capacity = 0;
	manager->bucket_count = 0;

	free(manager);
	manager_ptr = NULL;

	CUTE_LOG("objects", "All objects unallocated.\n");
}


bool
ct_objects_checkError(ctObjectManager* manager, ctError* error) {
	if (manager->error.code != ctErrorCode_None) {
		*error = manager->error;
		return true;
	}
	return false;
}


uint32_t
ct_objects_newBucket(ctObjectManager* manager) {
	
	if (manager->bucket_count == manager->bucket_capacity) {
		manager->bucket_capacity = manager->bucket_capacity == 0 ? 1 : manager->bucket_capacity * 2;
		manager->buckets = realloc(manager->buckets, sizeof(ctObjectBucket*) * manager->bucket_capacity);
	}

	ctObjectBucket* bucket = malloc(sizeof(ctObjectBucket));

	if (bucket == NULL) {
		CUTE_LOG("objects", "Failed to allocate memory for new bucket.\n");
		return UINT32_MAX;
	}

	bucket->id = manager->bucket_count;
	memset(bucket->objects, 0, sizeof(bucket->objects));

	manager->buckets[manager->bucket_count] = bucket;
	manager->bucket_count++;

	ct_objects_pushEmptyBucket(manager, bucket);

	CUTE_LOG("objects", "New bucket (%u) [%p] allocated. Pushed it to empty buckets stack.\n", bucket->id, bucket);
	return bucket->id;
};


// Mark a bucket as empty and push it to the empty buckets stack
void
ct_objects_pushEmptyBucket(ctObjectManager* manager, ctObjectBucket* bucket) {
	
	if (manager->empty_bucket_count == manager->empty_bucket_capacity) {
		manager->empty_bucket_capacity = manager->empty_bucket_capacity == 0 ? 1 : manager->empty_bucket_capacity * 2;
		manager->empty_buckets = realloc(manager->empty_buckets, sizeof(ctObjectBucket*) * manager->empty_bucket_capacity);
	}

	manager->empty_buckets[manager->empty_bucket_count++] = bucket;
};


ctObjectBucket*
ct_objects_popEmptyBucket(ctObjectManager* manager) {
	if (manager->empty_bucket_count == 0) {
		return NULL;
	};
	return manager->empty_buckets[--manager->empty_bucket_count];
};


ctObject*
ct_objects_newObject(ctObjectManager* manager, uint32_t data_size, uint64_t data_type, ctObjectDelete del_func) {

	ctObjectBucket* assigned_bucket = NULL;
	ctObject* assigned_obj = NULL;

	assigned_bucket = ct_objects_popEmptyBucket(manager);
	
	// Have to allocate a new bucket if no bucket was found
	if (!assigned_bucket) {
		ct_objects_newBucket(manager);
		assigned_bucket = ct_objects_popEmptyBucket(manager);
	}
	

	// Looking for a valid index to assign the Object to in the bucket
	for (uint32_t j = 0; j < sizeof(assigned_bucket->objects)/sizeof(assigned_bucket->objects[0]); j++) {

		if (ct_utils_isBitSet(assigned_bucket->bitmask, j)) {
			continue;;
		}

		assigned_obj = &assigned_bucket->objects[j];
		assigned_obj->bucket_id = assigned_bucket->id;
		assigned_obj->bucket_index = j;

		ct_utils_setBit(&assigned_bucket->bitmask, j);

		if (assigned_bucket->bitmask == 0xFFFFFFFFFFFFFFFF) {
			CUTE_LOG("objects", "Bucket (%u) [%p] is full, so did not push to empty buckets stack.\n", assigned_bucket->id, assigned_bucket);
		} else {
			CUTE_LOG("objects", "Bucket (%u) [%p] is not full, so pushed to empty buckets stack.\n", assigned_bucket->id, assigned_bucket);
			ct_objects_pushEmptyBucket(manager, assigned_bucket);
		}

		break;
	};

	if (!assigned_obj) {
		manager->error.code = ctErrorCode_EngineFailure;
		ct_utils_format(
			manager->error.msg, 
			sizeof(manager->error.msg), 
			"Engine failed to find a free slot for a Object. This should never happen."
		);
		CUTE_LOG("objects", "Failed to allocate new Object. No available slots in bucket (%u)\n", assigned_bucket->id);
		return NULL;
	}


	// Allocating atoms
	uint8_t* ptr = malloc(data_size);

	if (ptr == NULL) {
		ct_utils_clearBit(&assigned_bucket->bitmask, assigned_obj->bucket_index);
		manager->error.code = ctErrorCode_EngineFailure;
		ct_utils_format(
			manager->error.msg, 
			sizeof(manager->error.msg), 
			"Engine failed to allocate memory for new Object (%u.%u). Out of memory.", assigned_obj->bucket_id, assigned_obj->bucket_index
		);

		CUTE_LOG("objects", "Failed to allocate memory for Object (%u.%u) [%p] of size %u\n", assigned_obj->bucket_id, assigned_obj->bucket_index, assigned_obj, data_size);
		return NULL;
	}
	
	assigned_obj->ref_count = 0;
	assigned_obj->data_size = data_size;
	assigned_obj->data = ptr;
	assigned_obj->data_type = data_type;
	assigned_obj->data_del_func = del_func;


	CUTE_LOG("objects", "New Object (%u.%u) [%p] (data:%u bytes) allocated to bucket (%u)\n", assigned_obj->bucket_id, assigned_obj->bucket_index, assigned_obj, data_size, assigned_bucket->id);
	return assigned_obj;
}


void
ct_objects_delObject(ctObjectManager* manager, ctObject* obj) {

	
	ctObjectBucket* bucket = manager->buckets[obj->bucket_id];

	if (bucket->bitmask == 0xFFFFFFFFFFFFFFFF) {
		ct_objects_pushEmptyBucket(manager, bucket);
	};

	ct_utils_clearBit(&bucket->bitmask, obj->bucket_index);

	if (obj->data_del_func) {
		obj->data_del_func(manager, obj);
	}

	free(obj->data);

	CUTE_LOG("objects", "Object (%u.%u) [%p] unallocated.\n", obj->bucket_id, obj->bucket_index, obj);
}
