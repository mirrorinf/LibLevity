//
//  Levity.h
//  LibLevity
//
//  Created by 褚晓敏 on 11/13/15.
//  Copyright © 2015 褚晓敏. All rights reserved.
//

#ifndef Levity_h
#define Levity_h

#include "DataTypes.h"

Int8 LevityFunctionRegist(LevityContext *context, const char *identifier, VoidBlockWithVoidPtrInput function, UInt32 selfSize, UInt32 inputSize);

LevityContext *NewLevityContext(UInt32 numberOfThreads, UInt32 functionTableSize, UInt32 queueTableSize, UInt32 staticVariableTableSize);

Int8 AddQueueToContext(LevityContext *self, UInt8 flag, const char *identifier);

Int8 LevitySubmitAsync(LevityContext *context, const char *function, const char *queue, void *input);

//Int8 LevitySubmitSync(char *function, char *queue, void *input);

void LevityQueueWait(LevityContext *context, const char *queue);
#endif /* Levity_h */
