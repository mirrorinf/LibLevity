//
//  Levity.c
//  LibLevity
//
//  Created by 褚晓敏 on 11/13/15.
//  Copyright © 2015 褚晓敏. All rights reserved.
//

#include "Levity.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void *LevityThreadWork(void *pass) {
	LevityContext *self = (LevityContext *)pass;
	LevityTask *present;
	LevityFunction *work;
	LevityQueue *queue;
	while (1) {
		LevityMutexLock(self->mainQueue->nullQueue);
		LevityMutexUnlock(self->mainQueue->nullQueue);
		LevityMutexLock(self->mainQueue->operating);
		if (self->mainQueue->count == 0) {
			LevityMutexUnlock(self->mainQueue->operating);
			continue;
		}
		present = self->mainQueue->head;
		if (self->mainQueue->count == 1) {
			self->mainQueue->count = 0;
			self->mainQueue->head = NULL;
			self->mainQueue->tail = NULL;
			LevityMutexLock(self->mainQueue->nullQueue);
		} else {
			self->mainQueue->count--;
			self->mainQueue->head = present->next;
		}
		LevityMutexUnlock(self->mainQueue->operating);
		work = (LevityFunction *)HashTableLookUp(self->functions, present->identifier);
		work->invoke(present->input);
		queue = (LevityQueue *)HashTableLookUp(self->queues, present->queue);
		LevityMutexLock(queue->operating);
		if (queue->count == 1) {
			queue->count = 0;
			LevityMutexLock(queue->nullQueue);
			LevityMutexUnlock(queue->running);
		} else {
			queue->count--;
		}
		LevityMutexUnlock(queue->operating);
	}
	return NULL;
}

LevityContext *NewLevityContext(UInt32 numberOfThreads, UInt32 functionTableSize, UInt32 queueTableSize, UInt32 staticVariableTableSize) {
	LevityContext *newContext = (LevityContext *)malloc(sizeof(LevityContext));
	if (newContext == NULL) {
		return NULL;
	}
	newContext->errors = NULL;
	newContext->functions = NewHashTable(functionTableSize, ^(Void *trash) {
		LevityFunction *temp = (LevityFunction *)trash;
		Block_release(temp->invoke);
		free(temp);
	});
	if (newContext->functions == NULL) {
		free(newContext);
		return NULL;
	}
	newContext->mainQueue = (LevityGrandQueue *)malloc(sizeof(LevityGrandQueue));
	if (newContext->mainQueue == NULL) {
		DestroyHashTable(newContext->functions);
		free(newContext);
		return NULL;
	}
	newContext->mainQueue->count = 0;
	newContext->mainQueue->head = newContext->mainQueue->tail = NULL;
	LevityMutexInit(&newContext->mainQueue->nullQueue);
	LevityMutexLock(newContext->mainQueue->nullQueue);
	LevityMutexInit(&newContext->mainQueue->operating);
	newContext->numberOfErrors = 0;
	newContext->numberOfThreads = numberOfThreads;
	newContext->queues = NewHashTable(queueTableSize, ^(Void *trash){
		LevityQueue *t = trash;
		LevityMutexDestroy(t->nullQueue);
		LevityMutexDestroy(t->operating);
		LevityMutexDestroy(t->running);
		free(trash);
	});
	if (newContext->queues == NULL) {
		DestroyHashTable(newContext->functions);
		free(newContext->mainQueue);
		free(newContext);
		return NULL;
	}
	newContext->staticVariables = NULL;
	newContext->workThreads = (LevityThread *)malloc(sizeof(LevityThread) * numberOfThreads);
	for (UInt32 i = 0; i < numberOfThreads; i++) {
		LevityThreadInit(&newContext->workThreads[i]);
		LevityStartThreadWithFunction(newContext->workThreads[i], LevityThreadWork, (void *)newContext);
	}
	return newContext;
}

Int8 LevityFunctionRegist(LevityContext *context, const char *identifier, VoidBlockWithVoidPtrInput function, UInt32 selfSize, UInt32 inputSize) {
	LevityFunction *new = (LevityFunction *)malloc(sizeof(LevityFunction));
	if (new == NULL) {
		return -1;
	}
	new->inputSize = inputSize;
	new->selfSize = selfSize;
	new->invoke = Block_copy(function);
	return HashTableInsert(context->functions, new, identifier);
}

Int8 AddQueueToContext(LevityContext *self, UInt8 flag, const char *identifier) {
	LevityQueue *new = (LevityQueue *)malloc(sizeof(LevityQueue));
	if (new == NULL) {
		return -1;
	}
	new->count = 0;
	new->flag = flag;
	LevityMutexInit(&new->nullQueue);
	LevityMutexInit(&new->operating);
	LevityMutexInit(&new->running);
	LevityMutexLock(new->nullQueue);
	return HashTableInsert(self->queues, new, identifier);
}

Int8 LevitySubmitAsync(LevityContext *context, const char *function, const char *queue, void *input) {
	LevityQueue *present = (LevityQueue *)HashTableLookUp(context->queues, queue);
	if (present == NULL) {
		printf("Hei!!!!!\n");
	}
	LevityTask *task = (LevityTask *)malloc(sizeof(LevityTask));
	strcpy(task->identifier, function);
	strcpy(task->queue, queue);
	task->input = input;
	task->next = NULL;
	LevityMutexLock(present->operating);
	if (present->count == 0) {
		present->count = 1;
		LevityMutexUnlock(present->nullQueue);
		LevityMutexLock(present->running);
	} else {
		present->count++;
	}
	LevityMutexLock(context->mainQueue->operating);
	if (context->mainQueue->count == 0) {
		context->mainQueue->count = 1;
		LevityMutexUnlock(context->mainQueue->nullQueue);
	} else {
		context->mainQueue->count++;
	}
	if (context->mainQueue->count == 1) {
		context->mainQueue->head = task;
		context->mainQueue->tail = task;
	} else {
		context->mainQueue->tail->next = task;
		context->mainQueue->tail = task;
	}
	LevityMutexUnlock(context->mainQueue->operating);
	LevityMutexUnlock(present->operating);
	return 0;
}

void LevityQueueWait(LevityContext *context, const char *queue) {
	LevityQueue *waiting = HashTableLookUp(context->queues, queue);
	LevityMutexLock(waiting->nullQueue);
	LevityMutexUnlock(waiting->nullQueue);
}