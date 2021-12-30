`InitializeObjectReferences()`里面会调用`FReadOnlyCVARCache::Get()`
但是它的初始化`GReadOnlyCVARCache.Init()`却是在后面