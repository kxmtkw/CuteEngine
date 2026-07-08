
#ifndef CONTAINERS_ATOMCON_H
#define CONTAINERS_ATOMCON_H


#include <stdint.h>

#include "containers.h"

typedef struct {
	uint32_t            size;
	uint32_t            sub_containers;
	ctAtom*             atoms;
	ctAtomTypeSize*     types;
} ctAtomContainer;


ctContainer*
ct_atomcon_new(ctContainerManager* manager, uint32_t size);

void
ct_atomcon_del(ctContainerManager* manager, ctContainer* con);


static inline uint32_t
ct_atomcon_size(ctContainerManager* manager, ctContainer* con) {
	ctAtomContainer* atomcon = (ctAtomContainer*) con->data;
	return atomcon->size;
};


// Get an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds
ctTypedAtom
ct_atomcon_get(ctContainerManager* manager, ctContainer* container, uint32_t index);

// Set an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds.
void
ct_atomcon_set(ctContainerManager* manager, ctContainer* container, uint32_t index, ctTypedAtom atom);

// Create a shallow copy of a container
ctContainer*
ct_atomcon_copy(ctContainerManager* manager, ctContainer* src);

// Create a deep copy of a container
ctContainer*
ct_atomcon_deepcopy(ctContainerManager* manager, ctContainer* src);

// Resize the container
void
ct_atomcon_resize(ctContainerManager* manager, ctContainer* con, uint32_t new_size);


#endif // CONTAINERS_ATOMCON_H
