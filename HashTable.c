//
//  HashTable.c
//  LibLevity
//
//  Created by 褚晓敏 on 11/1/15.
//  Copyright © 2015 褚晓敏. All rights reserved.
//

#include "HashTable.h"
#include <stdlib.h>
#include <string.h>
#include <Block.h>
#include <stdio.h>

UInt32 hash(const char *input) {
	return 200;
}

HashTable *NewHashTable(UInt32 tableSize, VoidBlockWithVoidPtrInput dataDestroy) {
	HashTable *new = (HashTable *)malloc(sizeof(HashTable));
	if (new == NULL) {
		return NULL;
	}
	new->table = (HashElement **)malloc(sizeof(HashElement *) * tableSize);
	if (new->table == NULL) {
		free(new);
		return NULL;
	}
	new->dataDestroy = Block_copy(dataDestroy);
	new->tableSize = tableSize;
	return new;
}

void DestroyHashTable(HashTable *self) {
	for (UInt32 i = 0; i < self->tableSize; i++) {
		if (self->table[i] == NULL) {
			continue;
		}
		HashElement *temp = self->table[i];
		if (temp->next == NULL) {
			self->dataDestroy(temp->data);
			free(temp);
			continue;
		}
		HashElement *tempnext = temp->next;
		while (tempnext != NULL) {
			self->dataDestroy(temp->data);
			free(temp);
			temp = tempnext;
			tempnext = tempnext->next;
		}
		self->dataDestroy(temp->data);
		free(temp);
	}
	free(self->table);
	Block_release(self->dataDestroy);
	free(self);
}

Int8 HashTableInsert(HashTable *self, void *data, const char *key) {
	HashElement *new = (HashElement *)malloc(sizeof(HashElement));
	if (new == NULL) {
		return -1;
	}
	new->data = data;
	strcpy(new->key, key);
	new->next = NULL;
	UInt32 index = hash(key) % self->tableSize;
	if (self->table[index] == NULL) {
		self->table[index] = new;
		return 0;
	}
	HashElement *temp = self->table[index];
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = new;
	return 0;
}

Void *HashTableLookUp(HashTable *self, const char *key) {
	UInt32 index = hash(key) % self->tableSize;
	if (self->table[index] == NULL) {
		return NULL;
	}
	HashElement *temp = self->table[index];
	while (1) {
		if (temp == NULL) {
			return NULL;
		}
		if (strcmp(temp->key, key) == 0) {
			return temp->data;
		}
		temp = temp->next;
	}
}
