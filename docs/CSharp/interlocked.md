Interlocked可以为多个线程共享的变量提供原子操作。
- Interlocked.Increment 以原子操作的形式递增指定变量的值并存储结果。
- Interlocked.Decrement 以原子操作的形式递减制定变量的值并存储结果。
- Interlocked.Add(ref int src, int val) 以原子操作的形式，添加两个整数并用两者的和替换第一个整数。
- Interlocked.CompareExhange(ref target, desiredVal, startVal); 将target的值和startVal比较，相等则用desiredVal替换target，否则不操作。