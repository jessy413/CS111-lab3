# Hash Hash Hash

Add list entries in a hash table without race conditions using mutex.

## Building

To build, run the `make` command.

## Running

Example command:
`./hash-table-tester -t 8 -s 50000`

## First Implementation

In the add entry function, I added the mutex at line 89, before the read section. I added the code to unlock the mutex at line 102, after the write section. I blocked the all function implementation, which guaranteed correctness because no two thread can access the read and the write section to read the wrong value or to overwrite value that would result in inconsistency.

### Performance

Run the tester such that the base hash table completes in 1-2 seconds.
Report the relative speedup (or slow down) with a low number of threads and a
high number of threads. Note that the amount of work (`-t` times `-s`) should
remain constant. Explain any differences between the two.

./hash-table-tester -t 2 -s 50000
#Generation: 11,878 usec
#Hash table base: 61,622 usec

# - 0 missing

#Hash table v1: 96,192 usec

# - 0 missing

./hash-table-tester -t 8 -s 50000
#Generation: 48,963 usec
#Hash table base: 1,910,247 usec

# - 0 missing

#Hash table v1: 2,154,183 usec

# - 0 missing

Version 1 is about 1.1 times slower than the base inplementation with 8 threads and 1.6 times slower with 2 threads. The general slow is because the mutex is added around the whole add entry function implementatoin, making threads other than the one holding the mutex waiting for the current to finish, making the time longer for all threads to finish.

## Second Implementation

In the declaration of the hash table, I added an array of mutex same size with the number of hash table entries in line 28. I initialized the mutexes in the initialization of the hash table. In the add entry function, I locked the specific mutex for the hash table entry given by the key value in line 89, and the critical section is the rest of the function implementation. This is correct because now every hash table entry has its specific mutex and no other thread can modify the list entries within the hash table entry at the same time of the current thread modifying it, so no race conditions would occur.

### Performance

./hash-table-tester -t 8 -s 50000
Generation: 48,963 usec
Hash table base: 1,910,247 usec

- 0 missing
  Hash table v1: 2,154,183 usec
- 0 missing
  Hash table v2: 629,913 usec
- 0 missing

Version 2 uses only 0.3 times of the base implementation. This performance increase between version 2 and the other implementation is caused by the difference in locaiton of the addition of the mutexes. Now each mutex corresponds to one hash table entries, so when different threads trying to modify list entries within different hash table bucket, they will not be block by each other. In the meantime, only if different threads are trying to modifying list entries within in the same hash table entry, one has to wait for the one that owns the mutex for current bucket to finish in order to modify the same hash table entry. Hence concurrency is allowed while accuracy is maintained.

## Cleaning up

run the `make clean` command to clean up the binary files
