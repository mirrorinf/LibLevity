//
//  HashTable.h
//  LibLevity
//
//  Created by 褚晓敏 on 11/1/15.
//  Copyright © 2015 褚晓敏. All rights reserved.
//

#ifndef HashTable_h
#define HashTable_h

#include "BlockNames.h"

typedef struct HashElement_ {
	void *data;
	char key[31];
	struct HashElement_ *next;
} HashElement;

typedef struct HashTable_ {
	HashElement **table;
	UInt32 tableSize;
	VoidBlockWithVoidPtrInput dataDestroy;
} HashTable;

HashTable *NewHashTable(UInt32 tableSize, VoidBlockWithVoidPtrInput dataDestroy);

void DestroyHashTable(HashTable *self);

Int8 HashTableInsert(HashTable *self, void *data, const char *key);

Void *HashTableLookUp(HashTable *self, const char *key);

#endif /* HashTable_h */