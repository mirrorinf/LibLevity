//
//  ConcurrentTools.c
//  LibLevity
//
//  Created by 褚晓敏 on 11/6/15.
//  Copyright © 2015 褚晓敏. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ConcurrentTools.h"

void LevityMutexInit(LevityMutex *mutex) {
	*mutex = (LevityMutex)malloc(sizeof(pthread_mutex_t));
	int rc = pthread_mutex_init(*mutex, NULL);
	printf("%d\n", rc);
}

void LevityMutexDestroy(LevityMutex mutex) {
	pthread_mutex_destroy(mutex);
	free(mutex);
}

Int8 LevityThreadInit(LevityThread *thread) {
	*thread = malloc(sizeof(pthread_t));
	if (*thread == NULL) {
		return -1;
	} else {
		return 1;
	}
}

Int8 LevityStartThreadWithFunction(LevityThread thread, void *(*work)(void *), void *data) {
	Int8 rc = pthread_create(thread, NULL, work, data);
	return rc;
}

Int8 LevityMutexLock(LevityMutex mutex) {
	Int8 rc = pthread_mutex_lock(mutex);
	return rc;
}

Int8 LevityMutexUnlock(LevityMutex mutex) {
	return pthread_mutex_unlock(mutex);
}
