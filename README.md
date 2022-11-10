# Hash Hash Hash

Add list entries in a hash table without race conditions with mutex.

## Building

To build, run the `make` command.

## Running

Example command:
`./hash-table-tester -t 8 -s 50000`

## First Implementation

Describe your first implementation strategy here (the one with a single mutex).
Argue why your strategy is correct.

In the add entry function, I added the mutex at line 89, before the read section. I added the code to unlock the mutex at line 102, after the write section. I blocked the all function implementation, which guaranteed correctness because no two thread can access the read and the write section to read the wrong value or to overwrite value that would result in inconsistency.

### Performance

Run the tester such that the base hash table completes in 1-2 seconds.
Report the relative speedup (or slow down) with a low number of threads and a
high number of threads. Note that the amount of work (`-t` times `-s`) should
remain constant. Explain any differences between the two.

## Second Implementation

Describe your second implementation strategy here (the one with a multiple
mutexes). Argue why your strategy is correct.

### Performance

Run the tester such that the base hash table completes in 1-2 seconds.
Report the relative speedup with number of threads equal to the number of
physical cores on your machine (at least 4). Note again that the amount of work
(`-t` times `-s`) should remain constant. Report the speedup relative to the
base hash table implementation. Explain the difference between this
implementation and your first, which respect why you get a performance increase.

## Cleaning up

Explain briefly how to clean up all binary files.
