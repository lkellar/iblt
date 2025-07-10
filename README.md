#  Invertible Bloom Lookup Trees

Implementations of a collection of IBLTs, as well as some hash functions

- [Original IBLT](https://ieeexplore.ieee.org/document/6120248)
- [HPW IBLT](https://epubs.siam.org/doi/10.1137/1.9781611977585.ch21)
- [Stacked IBLT](https://drops.dagstuhl.de/entities/document/10.4230/LIPIcs.ESA.2024.54)

I can't vouch for the speed or memory usage or performance of any of the data structures, they really only were used to compare against each other.

However, they both ran decently fast on a modern laptop and probably could be further sped up by locking in a hash function and running it through a profiler.

Some cool visualizations are available in the [Slides](https://lkellar.org/about/kellar_iblt.pdf)

### Original IBLT

Stores both keys/values and can (usually) answer queries when the number of elements in the structure is below the provided threshold.

I've found that with the KnuthHash, passing in a size of at least `1.3 * m` (where m is the upper bound of elements you want to decode), works best.

Supports: 
- `insert(key, val)`
- `delete(key, val)`
- `get(key)`
- `decode()` (**destructive**) - returns as much of the data as possible, may return an incomplete flag

### HPW IBLT

Stores only keys and only can list out entries, no point queries

Like the Original IBLT, KnuthHash with at least 1.3 * m will work best.

Supports:
- `toggle(key)` - insert/remove a key from the structure
- `merge(other_sketch)`
- `decode()` (**destructive**) - returns entire list or nothing

### Stacked IBLT

Stores key/values in a more complex stacking structure, using multiple "Basic IBLTs"

There are three configurable parameters, two constants `C` and `C_0`, as well as a parameter `error_rate`. I haven't quite gotten the tuning down yet, but setting `C = 5/e, C_0 = 5, error_rate=0.05` should work pretty consistently. I've found that `C` is the most responsive parameter, and turning it slightly up drastically increases success rates.

If interested, check out the [original paper](https://drops.dagstuhl.de/entities/document/10.4230/LIPIcs.ESA.2024.54) for more detail on the parameters and maybe a better intuition on how to set them.

Supports:
- `insert(key, val)`
- `delete(key, val)`
- `decode()` (**destructive**) - returns as much of the data as possible, may return an incomplete flag

## Hash Functions

Also included are a handful of hash functions, and each of the IBLTs can pretty easily accept each of the hash functions (except Stacked doesn't accept the Tabular hash at the moment, but it wouldn't be hard to implement)

- DualHash: A simple `ax + b % PRIME % array_size` linear hash
- KnuthHash: Knuth's multiplicative hash `ax >> (w-b) % array_size`, where `w = word size` and `b = bits` needed to represent the array size
- [TabularHash](https://en.wikipedia.org/wiki/Tabulation_hashing): Accepts a "max bits" argument that represents how many bits in the input should be hashed. Defaults to a block size of 1 (which is a little small, but it didn't really work at larger sizes for my test data).

### Usage

Right now the only real usage is in `main.cpp` which has a handful of helper functions to test the IBLTs with random numbers. For actual usage, I'd reccomend selecting an IBLT and hash function, melding them together, and running it through a profiler.  