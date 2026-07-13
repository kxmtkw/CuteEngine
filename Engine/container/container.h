
#ifndef CONTAINER_CONTAINER_H
#define CONTAINER_CONTAINER_H

#include <stdint.h>
#include "CuteAtom.h"
#include "engine/error.h"
#include "object/object.h"


typedef struct {
	uint32_t            size;
	uint32_t            sub_objects;
	ctAtom*             atoms;
	ctAtomTypeSize*     types;
} ctContainer;


ctObject*
ct_container_new(ctObjectManager* manager, uint32_t size, ctError* err);

void
ct_container_del(ctObjectManager* manager, ctObject* con);


static inline uint32_t
ct_container_size(ctObjectManager* manager, ctObject* con) {
	ctContainer* container = (ctContainer*) con->data;
	return container->size;
};


// Get an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds
ctTypedAtom
ct_container_get(ctObjectManager* manager, ctObject* container, uint32_t index, ctError* err);

// Set an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds.
void
ct_container_set(ctObjectManager* manager, ctObject* container, uint32_t index, ctTypedAtom atom, ctError* err);

// Create a shallow copy of a container
ctObject*
ct_container_copy(ctObjectManager* manager, ctObject* src, ctError* err);

// Create a deep copy of a container
ctObject*
ct_container_deepcopy(ctObjectManager* manager, ctObject* src, ctError* err);


#endif // CONTAINERS_CONTAINER_H
