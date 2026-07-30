
#ifndef CUTE_CONTAINER_H
#define CUTE_CONTAINER_H

#include <stdint.h>

#include "common/atom.h"
#include "common/error.h"

#include "objects/manager.h"


typedef struct {
	CtObject            __object__;
	uint32_t            sub_objects;
	uint32_t            size;
	CtAtom*             atoms;
	CtAtomTypeSize*     types;
} ctContainer;


CtObject*
ct_container_new(CtObjectManager* manager, uint32_t size, ctError* err);

void
ct_container_del(CtObjectManager* manager, CtObject* con);


static inline uint32_t
ct_container_size(CtObjectManager* manager, CtObject* con) {
	ctContainer* container = (ctContainer*) con;
	return container->size;
};


// Get an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds
CtTypedAtom
ct_container_get(CtObjectManager* manager, CtObject* container, uint32_t index, ctError* err);

// Set an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds.
void
ct_container_set(CtObjectManager* manager, CtObject* container, uint32_t index, CtTypedAtom atom, ctError* err);

// Create a shallow copy of a container
CtObject*
ct_container_copy(CtObjectManager* manager, CtObject* src, ctError* err);

// Create a deep copy of a container
CtObject*
ct_container_deepcopy(CtObjectManager* manager, CtObject* src, ctError* err);


#endif // CONTAINERS_CONTAINER_H
