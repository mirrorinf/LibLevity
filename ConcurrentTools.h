//
//  ConcurrentTools.h
//  LibLevity
//
//  Created by 褚晓敏 on 11/1/15.
//  Copyright © 2015 褚晓敏. All rights reserved.
//

#ifndef ConcurrentTools_h
#define ConcurrentTools_h

#include "TypeRenaming.h"
#include <pthread.h>

typedef pthread_t *LevityThread;
typedef pthread_mutex_t *LevityMutex;

void LevityMutexInit(LevityMutex *mutex);

void LevityMutexDestroy(LevityMutex mutex);

Int8 LevityStartThreadWithFunction(LevityThread thread, void *(*work)(void *), void *data);

Int8 LevityThreadInit(LevityThread *thread);

Int8 LevityMutexLock(LevityMutex mutex);

Int8 LevityMutexUnlock(LevityMutex mutex);

#endif /* ConcurrentTools_h */
