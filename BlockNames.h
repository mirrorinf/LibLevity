//
//  BlockNames.h
//  LibLevity
//
//  Created by 褚晓敏 on 11/1/15.
//  Copyright © 2015 褚晓敏. All rights reserved.
//

#ifndef BlockNames_h
#define BlockNames_h

#include <Block.h>
#include "TypeRenaming.h"

typedef Void (^VoidBlockWithVoidPtrInput)(Void *);
typedef Int8 (^Int8BlockWithTwoVoidPtrInput)(Void *, Void *);
typedef UInt32 (^UInt32BlockWithVoidPtrInput)(Void *);
typedef UInt32 (^UInt32BlockWithConstCharPtrInput)(const char *);
typedef Int8 (^Int8BlockWithTwoConstVoidPtrInput)(const char *, const char *);

#endif /* BlockNames_h */
