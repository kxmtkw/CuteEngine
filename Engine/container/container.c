#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CuteAtom.h"
#include "container.h"
#include "object/object.h"
#include "utils/utils.h"



ctObject*
ct_container_new(ctObjectManager* manager, uint32_t size,  ctError* err) {

	uint32_t req_size = sizeof(ctContainer) + sizeof(ctAtom) * size + sizeof(ctAtomTypeSize) * size;

	ctObject* obj = ct_objects_newObject(manager, req_size, 0 /*placeholder*/, ct_container_del);

	if (ct_objects_checkError(manager, err)) {
		return NULL;
	};
	
	uint8_t* data = obj->data;
	ctContainer* container = (ctContainer*) obj->data;
	container->atoms = (ctAtom*) (data + sizeof(ctContainer));
	container->types = (ctAtomTypeSize*) (data + sizeof(ctContainer) + sizeof(ctAtom) * size);
	container->size = size;
	container->sub_objects = 0;
	

	CUTE_LOG("containers", "New container (%u.%u) [%p] (size:%u atoms) allocated.\n", obj->bucket_id, obj->bucket_index, obj, size);

	return obj;
};


void
ct_container_del(ctObjectManager* manager, ctObject* obj) {

	ctContainer* container = obj->data;

	uint32_t j = 0;
	
	for (uint32_t i = 0; i < container->size && j < container->sub_objects; i++) {
		if (container->types[i] == ctAtomType_Object) {
			ct_objects_decRef(manager, container->atoms[i].as_object);
			j++;
		}
	}

	CUTE_LOG("containers", "Deleting container (%u.%u) [%p]\n", obj->bucket_id, obj->bucket_index, obj);
};


// Get an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds
ctTypedAtom
ct_container_get(ctObjectManager* manager, ctObject* con, uint32_t index, ctError* err) {

	ctContainer* container = con->data;

	if (index >= container->size) {
		err->code = ctErrorCode_OutOfBounds;
		ct_utils_format(
			err->msg, 
			sizeof(err->msg), 
			"Can not access container slot #%u (>= %u)", index, container->size
		);
		return (ctTypedAtom){ctAtomType_NoneType, (ctAtom){0}};
	}

	return (ctTypedAtom){container->types[index], container->atoms[index]};
};

// Set an atom in the container. Will return ctConManagerCode_OutOfBounds if index is out of bounds.
void
ct_container_set(ctObjectManager* manager, ctObject* con, uint32_t index, ctTypedAtom atom, ctError* err) {

	ctContainer* container = con->data;

	if (index >= container->size) {
		err->code = ctErrorCode_OutOfBounds;
		ct_utils_format(
			err->msg, 
			sizeof(err->msg), 
			"Can not set container slot #%u (>= %u)", index, container->size
		);
		return;
	}

	if (container->types[index] == ctAtomType_Object) {
		ct_objects_decRef(manager, container->atoms[index].as_object);
		if (container->sub_objects > 0) container->sub_objects--;
	}

	container->atoms[index] = atom.atom;
	container->types[index] = atom.type;

	if (container->types[index] == ctAtomType_Object) {
		ct_objects_incRef(manager, container->atoms[index].as_object);
		container->sub_objects++; 
	}
}

// Create a shallow copy of a container
ctObject*
ct_container_copy(ctObjectManager* manager, ctObject* src, ctError* err) {
	
	ctObject* copy = ct_container_new(manager, src->data_size, err);

	if (err->code) {return NULL;}

	ctContainer* consrc = src->data;
	ctContainer* concopy = copy->data;

	if (!copy) {
		return NULL;
	}

	memcpy(concopy->atoms, consrc->atoms, consrc->size * sizeof(ctAtom));
	memcpy(concopy->types, consrc->types, consrc->size * sizeof(ctAtomTypeSize));

	concopy->sub_objects = consrc->sub_objects;

	uint32_t j = 0;

	for (uint32_t i = 0; i < consrc->size && j < consrc->sub_objects; i++) {
		if (consrc->types[i] == ctAtomType_Object) {
			ct_objects_incRef(manager, consrc->atoms[i].as_object);
			j++;
		}
	}
	
	CUTE_LOG("containers", "Copied container (%u.%u) [%p] from container (%u.%u) [%p]\n", copy->bucket_id, copy->bucket_index, copy, src->bucket_id, src->bucket_index, src);

	return copy;

}

// Create a deep copy of a container
ctObject*
ct_container_deepcopy(ctObjectManager* manager, ctObject* src, ctError* err) {
	
	ctObject* copy = ct_container_new(manager, src->data_size, err);

	if (err->code) {return NULL;}

	ctContainer* consrc = src->data;
	ctContainer* concopy = copy->data;

	if (!copy) {
		return NULL;
	}

	memcpy(concopy->atoms, consrc->atoms, consrc->size * sizeof(ctAtom));
	memcpy(concopy->types, consrc->types, consrc->size * sizeof(ctAtomTypeSize));

	concopy->sub_objects = consrc->sub_objects;

	uint32_t j = 0;
	
	for (uint32_t i = 0; i < consrc->size && j < consrc->sub_objects; i++) {
		if (consrc->types[i] == ctAtomType_Object) {
			concopy->atoms[i].as_object = ct_container_deepcopy(manager, consrc->atoms[i].as_object, err);
			ct_objects_incRef(manager, consrc->atoms[i].as_object);
			j++;
		}
	}
	
	CUTE_LOG("containers", "Deep copied container (%u.%u) [%p] from container (%u.%u) [%p]\n", copy->bucket_id, copy->bucket_index, copy, src->bucket_id, src->bucket_index, src);

	return copy;
}