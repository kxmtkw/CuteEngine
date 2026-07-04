

#ifndef CONTAINERS_CONTAINER_H
#define CONTAINERS_CONTAINER_H

#include <stdint.h>

#include "CuteAtom.h"
#include "CuteConfig.h"
#include "CuteInstr.h"
#include "engine/error.h"


typedef struct {
	uint32_t id;
	uint64_t bitmask;
	ctContainer containers[64];
} ctContainerBucket;


typedef struct {
	ctContainerBucket** buckets;
	uint32_t            bucket_count;
	uint32_t            bucket_capacity;
	ctContainerBucket** empty_buckets;
	uint32_t            empty_bucket_count;
	uint32_t            empty_bucket_capacity;
	ctError             error;
} ctContainerManager;


// Start up the container manager
void
ct_containers_init(ctContainerManager* manager);

// End the container manager
void
ct_containers_end(ctContainerManager* manager);

// Allocate a new bucket, return its id
uint32_t
ct_containers_newBucket(ctContainerManager* manager);

// Mark a bucket as empty and push it to the empty buckets stack
void
ct_containers_pushEmptyBucket(ctContainerManager* manager, ctContainerBucket* bucket);

// Mark a bucket as not empty and remove it from the empty buckets stack
ctContainerBucket*
ct_containers_popEmptyBucket(ctContainerManager* manager);

// Allocate a new container
ctContainer*
ct_containers_newContainer(ctContainerManager* manager, uint32_t size);

// Delete a container, keeping in mind sub containers
void
ct_containers_delContainer(ctContainerManager* manager, ctContainer* con);


static inline void
ct_containers_incRef(ctContainerManager* manager, ctContainer* con) {
	con->ref_count++;
	CUTE_LOG("containers", "Container (%u.%u) [%p] referenced. References: %u\n", con->bucket_id, con->bucket_index, con, con->ref_count);
}


static inline void
ct_containers_decRef(ctContainerManager* manager, ctContainer* con) {
	con->ref_count--;
	CUTE_LOG("containers", "Container (%u.%u) [%p] dereferenced. References: %u\n", con->bucket_id, con->bucket_index, con, con->ref_count);
	if (con->ref_count == 0) {
		ct_containers_delContainer(manager, con);
	}
}

// Get an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds
ctTypedAtom
ct_containers_conGet(ctContainerManager* manager, ctContainer* container, uint32_t index);

// Set an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds.
void
ct_containers_conSet(ctContainerManager* manager, ctContainer* container, uint32_t index, ctTypedAtom atom);

// Create a shallow copy of a container
ctContainer*
ct_containers_conCopy(ctContainerManager* manager, ctContainer* src);

// Create a deep copy of a container
ctContainer*
ct_containers_conDeepCopy(ctContainerManager* manager, ctContainer* src);

// Makes a clone of the container
ctContainer*
ct_containers_conClone(ctContainerManager* manager, ctContainer* src);

#endif // CONTAINERS_CONTAINER_H