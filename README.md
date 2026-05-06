# Hash table in C

This is a simple macro-driven hashtable implementation in C that uses dynamic stacks for collision handling. It’s designed for low friction and flexibility, not as a full-featured production hashmap.

## Features
- Header-only (just include and go)
- Works with arbitrary value types
- Collision handling via dynamic stacks
- Automatic resizing (rehashing)
- Custom hash support (int, string, or user-defined)

## How It Works

The hashtable layout in memory:

- A hidden header (ht_header_t) storing size and count
- An array of buckets, each a dynamic stack
- A parallel array storing the original hash values

Each insert:
- Computes index = hash % size
- Pushes the value onto the bucket stack
- Stores the raw hash alongside the bucket

When load factor exceeds 0.75, the table is resized and all entries are rehashed.

## Usage
#### Create a table
```
int* table = NULL;
HASHTABLE(table, 16);
```
#### Insert values
```
HASHTABLE_PUT(table, HASH_INT(42), 100);
HASHTABLE_PUT(table, HASH_STRING("key"), 200);
```
#### Get values
```
int value = HASHTABLE_GET(table, HASH_INT(42));
```
#### Remove values
```
HASHTABLE_DISCARD(table, HASH_INT(42));
```
#### Free memory
```
HASHTABLE_FREE(table);
```

## Hash Functions

Included:

- HASH_INT(key)
- HASH_STRING(key) // uses djb2

Custom hash:
```
size_t h = my_hash(key);
HASHTABLE_PUT(table, h, value);
```
## Benchmark

| Operation                         | p50        | p90        | p99        |
|----------------------------------|-----------:|-----------:|-----------:|
| HASHTABLE_PUT (int)              | 9.22 μs    | 12.12 μs   | 20.76 μs   |
| PUT (int, no rehash)             | 4.89 μs    | 6.66 μs    | 10.65 μs   |
| HASHTABLE_GET (int)              | 9.20 μs    | 12.01 μs   | 18.37 μs   |
|                                  |            |            |            |
| HASHTABLE_PUT (string)           | 205.95 ns  | 237.18 ns  | 1.48 μs    |
| HASHTABLE_GET (string)           | 441.99 ns  | 494.50 ns  | 2.24 μs    |
| PUT (string ×100)                | 3.55 μs    | 5.02 μs    | 8.59 μs    |
|                                  |            |            |            |
| HASHTABLE_REHASH                 | 1.60 μs    | 1.73 μs    | 3.60 μs    |
| HASHTABLE_DISCARD                | 176.51 ns  | 203.87 ns  | 380.45 ns  |


## Load Factor
```
float load = HASH_LOAD(table);
```
Rehash triggers at load > 0.75

## Limitations

- No key-value pairing (only hash → value)
- No key comparison (collisions share buckets blindly)
- No deletion by key (only stack pop)
- No iteration API
- Not thread-safe
- Requires GNU extensions (__typeof__)

If two different keys produce the same hash, a new value is pushed into the corresponding stack.

## Design Tradeoffs

This implementation favors:

- Simplicity over completeness
- Macro flexibility over type safety

## Example
```
#include <stdio.h>
#include "hashtable.h"

int main() {
    int* table = NULL;
    HASHTABLE(table, 8);

    HASHTABLE_PUT(table, HASH_INT(1), 10);
    HASHTABLE_PUT(table, HASH_INT(1), 20);

    printf("%d\n", HASHTABLE_GET(table, HASH_INT(1))); // 20

    HASHTABLE_DISCARD(table, HASH_INT(1));
    printf("%d\n", HASHTABLE_GET(table, HASH_INT(1))); // 10

    HASHTABLE_FREE(table);
}
```
