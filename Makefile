all:
	@clang -shared -fPIC -o libLevity.dylib ConcurrentTools.c HashTable.c Levity.c -lpthread