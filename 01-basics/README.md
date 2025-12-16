## Problem: Concurrent Hash Table with Reader-Writer Lock
Implement a thread-safe hash table (simple chaining, 10 buckets) supporting:

```cpp
insert(key, value)
lookup(key) -> optional<value>
remove(key) -> bool
```

### Constraints:

- Multiple concurrent reads allowed
- Writes are exclusive (no concurrent reads or writes)
- Use std::shared_mutex (or implement reader-writer semantics with std::mutex + std::condition_variable if you want the challenge)
- Write a test with 4 reader threads and 2 writer threads hammering it

---------------
### Key concepts: 
- std::shared_lock vs std::unique_lock, when shared ownership makes sense, lock granularity choices

---------------
### Intuition focus: 
- This gets at why we need different lock types. With pthreads you might have used rwlocks, but C++'s type system makes read vs write intent explicit at compile time. Try timing it: how much faster are concurrent reads vs serializing everything with a plain mutex?


--------------
### Bonus challenge: 
- Make locking per-bucket instead of global. How does this change contention patterns? What new race conditions appear?