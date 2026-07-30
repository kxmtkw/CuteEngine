#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/atom.h"
#include "common/config.h"
#include "common/error.h"

#include "objects/manager.h"

#include "container.h"

#include "utils/utils.h"



CtObject*
ct_container_new(CtObjectManager* manager, uint32_t size,  ctError* err) {

	uint32_t req_size = sizeof(ctContainer) + sizeof(CtAtom) * size + sizeof(CtAtomTypeSize) * size;

	CtObject* obj = ct_objects_new_object(manager, req_size, 0 /*placeholder*/, ct_container_del);

	if (ct_objects_check_error(manager)) {
		*err = manager->error;
		return NULL;
	};
	
	uint8_t* data = (uint8_t*) obj;
	ctContainer* container = (ctContainer*) obj;
	container->atoms = (CtAtom*) (data + sizeof(ctContainer));
	container->types = (CtAtomTypeSize*) (data + sizeof(ctContainer) + sizeof(CtAtom) * size);
	container->size = size;
	container->sub_objects = 0;

	CT_LOG("containers", "New container (%u.%u) [%p] (size:%u atoms) allocated.\n", obj->bucket_id, obj->bucket_index, obj, size);

	return obj;
};


void
ct_container_del(CtObjectManager* manager, CtObject* obj) {

	ctContainer* container = (ctContainer*) obj;

	uint32_t j = 0;
	
	for (uint32_t i = 0; i < container->size && j < container->sub_objects; i++) {
		if (container->types[i] == CT_ATOM_OBJECT) {
			ct_objects_dec_ref(manager, container->atoms[i].as_object);
			j++;
		}
	}

	CT_LOG("containers", "Deleting container (%u.%u) [%p]\n", obj->bucket_id, obj->bucket_index, obj);
};


// Get an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds
CtTypedAtom
ct_container_get(CtObjectManager* manager, CtObject* obj, uint32_t index, ctError* err) {

	ctContainer* container = (ctContainer*) obj;

	if (index >= container->size) {
		CT_ERROR(err, ctErrorCode_OutOfBounds, "Can not access container slot #%u (>= %u)", index, container->size);
		return (CtTypedAtom){CT_ATOM_PRIMITIVE, (CtAtom){0}};
	}

	return (CtTypedAtom){container->types[index], container->atoms[index]};
};

// Set an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds.
void
ct_container_set(CtObjectManager* manager, CtObject* obj, uint32_t index, CtTypedAtom atom, ctError* err) {

	ctContainer* container = (ctContainer*) obj;

	if (index >= container->size) {
		CT_ERROR(err, ctErrorCode_OutOfBounds, "Can not access container slot #%u (>= %u)", index, container->size);
		return;
	}

	if (container->types[index] == CT_ATOM_OBJECT) {
		ct_objects_dec_ref(manager, container->atoms[index].as_object);
		if (container->sub_objects > 0) container->sub_objects--;
	}

	container->atoms[index] = atom.atom;
	container->types[index] = atom.type;

	if (container->types[index] == CT_ATOM_OBJECT) {
		ct_objects_inc_ref(manager, container->atoms[index].as_object);
		container->sub_objects++; 
	}
}

// Create a shallow copy of a container
CtObject*
ct_container_copy(CtObjectManager* manager, CtObject* src, ctError* err) {
	
	ctContainer* consrc = (ctContainer*) src;

	CtObject* copy = ct_container_new(manager, consrc->__object__.obj_size, err);

	if (err->code) {return NULL;}

	ctContainer* concopy = (ctContainer*) copy;

	if (!copy) {
		return NULL;
	}

	memcpy(concopy->atoms, consrc->atoms, consrc->size * sizeof(CtAtom));
	memcpy(concopy->types, consrc->types, consrc->size * sizeof(CtAtomTypeSize));

	concopy->sub_objects = consrc->sub_objects;

	uint32_t j = 0;

	for (uint32_t i = 0; i < consrc->size && j < consrc->sub_objects; i++) {
		if (consrc->types[i] == CT_ATOM_OBJECT) {
			ct_objects_inc_ref(manager, consrc->atoms[i].as_object);
			j++;
		}
	}
	
	CT_LOG("containers", "Copied container (%u.%u) [%p] from container (%u.%u) [%p]\n", copy->bucket_id, copy->bucket_index, copy, src->bucket_id, src->bucket_index, src);

	return copy;
}