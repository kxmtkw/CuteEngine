#ifndef CUTE_OBJECTS_H
#define CUTE_OBJECTS_H

#include <stdint.h>

#include "common/atom.h"
#include "common/config.h"
#include "common/error.h"


struct CtObjectManager;
typedef struct CtObjectManager CtObjectManager;

typedef void (*CtObjectDelete)(struct CtObjectManager*, struct CtObject*);

struct CtObject {
	uint32_t            bucket_id;
	uint32_t            bucket_index;
	uint32_t            ref_count;
	uint32_t            obj_size;
	uint32_t            obj_type;
	CtObjectDelete      obj_del_func;
}; 

typedef struct {
	uint32_t  id;
	uint64_t  bitmask;
	CtObject* objects[64];
} CtObjectBucket;


struct CtObjectManager {
	CtObjectBucket**    buckets;
	uint32_t            bucket_count;
	uint32_t            bucket_capacity;
	CtObjectBucket**    empty_buckets;
	uint32_t            empty_bucket_count;
	uint32_t            empty_bucket_capacity;
	ctError             error;
};


// Startup the object manager. Pushes the first object bucket as well.
CtObjectManager*
ct_objects_init(void);

// End the Object manager and all its resources. Sets the pointer to NULL for convenience.
void
ct_objects_end(CtObjectManager** manager_ptr);

// Check if the object manager failed.
bool
ct_objects_check_error(CtObjectManager* manager_ptr);

// Allocate a new empty bucket and pushes it to the stack. Returns it's id.
uint32_t
ct_objects_new_bucket(CtObjectManager* manager);

// Push a bucket to the empty stack. THe bucket SHOULD be empty (not filled) before it is pushed.
void
ct_objects_push_empty_bucket(CtObjectManager* manager, CtObjectBucket* bucket);

// Pop an empty bucket from the stack. If the bucket is full after the allocation, do not push it back.
CtObjectBucket*
ct_objects_pop_empty_bucket(CtObjectManager* manager);

// Allocate a new object.
CtObject*
ct_objects_new_object(CtObjectManager* manager, uint32_t obj_size, uint64_t obj_type, CtObjectDelete del_func);

// Delete an object, sub objects are NOT considered. The deleter if it exists, is also called.
void
ct_objects_del_object(CtObjectManager* manager, CtObject* obj);

// Increase the refcount of the object.
static inline void
ct_objects_inc_ref(CtObjectManager* manager, CtObject* obj) {
	obj->ref_count++;
	CT_LOG("objects", "Object (%u.%u) [%p] referenced. References: %u\n", obj->bucket_id, obj->bucket_index, obj, obj->ref_count);
}

// Decrease the ref count of an object. Returns true if the object is deleted.
static inline bool
ct_objects_dec_ref(CtObjectManager* manager, CtObject* obj) {
	obj->ref_count--;
	CT_LOG("objects", "Object (%u.%u) [%p] dereferenced. References: %u\n", obj->bucket_id, obj->bucket_index, obj, obj->ref_count);
	if (obj->ref_count == 0) {
		ct_objects_del_object(manager, obj);
		return true;
	}
	return false;
}


#endif // CUTE_OBJECTS_H