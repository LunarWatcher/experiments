# reflect-cpp

## Various results

### Performance
The write speed of the sample structs is comparatively low (~125MB/s over one sample), but it can write ~1.4 million strings per second with the small 4 element struct. Not sure how this compares to plain yyjson, but it is fast, so looks like the overhead from reflect-cpp is minimal. 

The overhead appears to mainly be in binary size. The relatively small test program is over 700kB in debug mode, which is a fair bit. To be fair, the release build is just 30kB, which doesn't appear as blatantly unreasonable. A good chunk of the size could likely be attributed to the rfl and stdlib includes, but I can't read the `objdump` well enough to estimate the actual size of my program. The main function is quite significant in size, so there is a noticeable increase.

But, it isn't big enough to be an argument against reflect-cpp. Debug builds are always inflated, especially for small binaries, so debug mode alone doesn't mean anything. 

The low write speed may be attributed to how the  yyjson benchmarks were performed. They're based on comparatively massive datasets, ranging from 60-ish kB to nearly 50MB (as far as I can tell without opening the benchmark repo in my local editor/file explorer to see actual sizes), while the test struct used for timing is a whole 84 bytes. The smallest dataset used by yyjson's benchmarks is around 600 times larger. I could run a larger sample test, but I frankly don't care. For web applications, the 1.4M/sec number matters more, especially since a good chunk of the data I'll be working with is small data. 

The main bottleneck is likely just  general overhead, especially since there's a good chunk of `std::string`s involved, which notoriously like copying. I assume rfl tries to optimise, but it just means there's a finite limit to how fast things can be expected to get. 

### `transitive`

The unintuitively named `rfl::Skip`. Not sure how useful it actually is. If going a step further with [sqlgen](<https://github.com/getml/sqlgen>), the skip is sort of conditional. It shouldn't be skipped for sqlgen, but it should be skipped for web use, because for obvious reasons, the password is needed locally when extracted from the database, but not for web use. 

The entire motivation behind using `::Skip` in the first place was to consolidate structs, but I don't think it's worth it over just properly specifying what structs are used where. I already need to do some cleanup of stuff.

## Potential future ideas

Considering `crow::returnable`, it might be worth adding some kind of wrapper that allows functions to, basically, be expressed in the form `ReturnType func(Server*, crow::request)`. In either case, some way to more dynamically extract which functions return what (more macros maybe?) would allow for that API doc generator thing I've half considered making for *checks calendar* around a year and never bothered. Depends on how easy it is to access the routes dynamically - might have to make a PR directly to crow for this to be feasible. Does mean the idea (internal reference: STAGING-3) is slightly more feasible though. Additional fucking around required.
