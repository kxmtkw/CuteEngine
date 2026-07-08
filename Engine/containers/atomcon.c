#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "CuteAtom.h"
#include "containers.h"
#include "utils/utils.h"
#include "atomcon.h"



ctContainer*
ct_atomcon_new(ctContainerManager* manager, uint32_t size) {

	uint32_t req_size = sizeof(ctAtomContainer) + sizeof(ctAtom) * size + sizeof(ctAtomTypeSize) * size;

	ctContainer* con = ct_containers_newContainer(manager, req_size, 0 /*placeholder*/);
	
	uint8_t* data = con->data;
	ctAtomContainer* atomcon = (ctAtomContainer*) con->data;
	atomcon->atoms = (ctAtom*) (data + sizeof(ctAtomContainer));
	atomcon->types = (ctAtomTypeSize*) (data + sizeof(ctAtomContainer) + sizeof(ctAtom) * size);
	atomcon->size = size;
	atomcon->sub_containers = 0;

	return con;
};


void
ct_atomcon_del(ctContainerManager* manager, ctContainer* con) {

	ctAtomContainer* atomcon = con->data;

	uint32_t j = 0;
	
	for (uint32_t i = 0; i < atomcon->size && j < atomcon->sub_containers; i++) {
		if (atomcon->types[i] == ctAtomType_Container) {
			ct_containers_decRef(manager, atomcon->atoms[i].as_container);
			j++;
		}
	}

	ct_containers_delContainer(manager, con);
};


// Get an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds
ctTypedAtom
ct_atomcon_get(ctContainerManager* manager, ctContainer* con, uint32_t index) {

	ctAtomContainer* atomcon = con->data;

	if (index >= atomcon->size) {
		manager->error.code = ctErrorCode_OutOfBounds;
		ct_utils_format(
			manager->error.msg, 
			sizeof(manager->error.msg), 
			"Can not access container slot #%u (>= %u)", index, atomcon->size
		);
		return (ctTypedAtom){ctAtomType_NoneType, (ctAtom){0}};
	}

	return (ctTypedAtom){atomcon->types[index], atomcon->atoms[index]};

};

// Set an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds.
void
ct_atomcon_set(ctContainerManager* manager, ctContainer* con, uint32_t index, ctTypedAtom atom) {

	ctAtomContainer* atomcon = con->data;

	if (index >= atomcon->size) {
		manager->error.code = ctErrorCode_OutOfBounds;
		ct_utils_format(
			manager->error.msg, 
			sizeof(manager->error.msg), 
			"Can not set container slot #%u (>= %u)", index, atomcon->size
		);
		return;
	}

	if (atomcon->types[index] == ctAtomType_Container) {
		ct_containers_decRef(manager, atomcon->atoms[index].as_container);
		if (atomcon->sub_containers > 0) atomcon->sub_containers--;
	}

	atomcon->atoms[index] = atom.atom;
	atomcon->types[index] = atom.type;

	if (atomcon->types[index] == ctAtomType_Container) {
		ct_containers_incRef(manager, atomcon->atoms[index].as_container);
		atomcon->sub_containers++; 
	}
}

// Create a shallow copy of a container
ctContainer*
ct_atomcon_copy(ctContainerManager* manager, ctContainer* src) {
	
	ctContainer* copy = ct_containers_newContainer(manager, src->data_size, src->data_type);
	ctAtomContainer* atomsrc = src->data;
	ctAtomContainer* atomcopy = copy->data;

	if (!copy) {
		return NULL;
	}

	memcpy(atomcopy->atoms, atomsrc->atoms, atomsrc->size * sizeof(ctAtom));
	memcpy(atomcopy->types, atomsrc->types, atomsrc->size * sizeof(ctAtomTypeSize));

	atomcopy->sub_containers = atomsrc->sub_containers;

	uint32_t j = 0;

	for (uint32_t i = 0; i < atomsrc->size && j < atomsrc->sub_containers; i++) {
		if (atomsrc->types[i] == ctAtomType_Container) {
			ct_containers_incRef(manager, atomsrc->atoms[i].as_container);
			j++;
		}
	}
	
	CUTE_LOG("containers", "Copied container (%u.%u) [%p] from container (%u.%u) [%p]\n", copy->bucket_id, copy->bucket_index, copy, src->bucket_id, src->bucket_index, src);

	return copy;

}

// Create a deep copy of a container
ctContainer*
ct_atomcon_deepcopy(ctContainerManager* manager, ctContainer* src) {
	
	ctContainer* copy = ct_containers_newContainer(manager, src->data_size, src->data_type);
	ctAtomContainer* atomsrc = src->data;
	ctAtomContainer* atomcopy = copy->data;

	if (!copy) {
		return NULL;
	}

	memcpy(atomcopy->atoms, atomsrc->atoms, atomsrc->size * sizeof(ctAtom));
	memcpy(atomcopy->types, atomsrc->types, atomsrc->size * sizeof(ctAtomTypeSize));

	atomcopy->sub_containers = atomsrc->sub_containers;

	uint32_t j = 0;
	
	for (uint32_t i = 0; i < atomsrc->size && j < atomsrc->sub_containers; i++) {
		if (atomsrc->types[i] == ctAtomType_Container) {
			atomcopy->atoms[i].as_container = ct_atomcon_deepcopy(manager, atomsrc->atoms[i].as_container);
			ct_containers_incRef(manager, atomsrc->atoms[i].as_container);
			j++;
		}
	}
	
	CUTE_LOG("containers", "Copied container (%u.%u) [%p] from container (%u.%u) [%p]\n", copy->bucket_id, copy->bucket_index, copy, src->bucket_id, src->bucket_index, src);

	return copy;
}

// Resize the container
void
ct_atomcon_resize(ctContainerManager* manager, ctContainer* con, uint32_t new_size) {
	printf("ATOM CONTAINER RESIZE NOT IMPLEMENTED!\n");
	exit(1);
}
