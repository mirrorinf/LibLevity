//
//  DataTypes.h
//  LibLevity
//
//  Created by 褚晓敏 on 11/1/15.
//  Copyright © 2015 褚晓敏. All rights reserved.
//

#ifndef DataTypes_h
#define DataTypes_h

#include "TypeRenaming.h"
#include "BlockNames.h"
#include "HashTable.h"
#include "ConcurrentTools.h"

/* The definition of type: LevityFunction
 * This is the type of function
 * selfSize is for future use */
typedef struct LevityFunction_ {
	VoidBlockWithVoidPtrInput invoke;
	UInt32 selfSize, inputSize;
} LevityFunction;

/* The definition of type: LevityTask
 * This is the essential building block of the Levity library.
 * The member flag is for future use */
typedef struct LevityTask_ {
	char identifier[31], queue[31];
	void *input;
	UInt16 flag;
	struct LevityTask_ *next;
} LevityTask;

/* The definition of type: LevityError
 * This is for the Levity error handling system
 * flag:
 *     1<<5 : 1 for critical error
 *     1<<10 : 1 for output a error message */
typedef struct LevityError_ {
	char descriptor[255];
	UInt16 flag;
} LevityError;

/* The definition of type: LevityQueue
 * This is for the scheduler
 * flag:
 *     1<<5 : 1 for serial queues(not yet supported)
 *     1<<10 : 1 for priority queues(not yet supported) */
typedef struct LevityQueue_ {
	UInt32 count;
	UInt16 flag;
	LevityMutex nullQueue, operating, running;
} LevityQueue;

typedef struct LevityGrandQueue_ {
	LevityTask *head, *tail;
	UInt64 count;
	LevityMutex nullQueue, operating;
} LevityGrandQueue;

// Not supported yet
typedef struct LevityStaticVariable_ {
} LevityStaticVariable;

/* The definition of type: LevityContext
 * This is the container of all the things needed
 * Static variables are not supported yet */
typedef struct LevityContext_ {
	LevityError *errors;
	UInt32 numberOfErrors;
	UInt32 numberOfThreads;
	HashTable *functions, *staticVariables, *queues;
	LevityThread *workThreads;
	LevityGrandQueue *mainQueue;
} LevityContext;

#endif /* DataTypes_h */