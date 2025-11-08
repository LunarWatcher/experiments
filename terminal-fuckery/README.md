# Terminal fuckery

This experiment is a prequel for:

1. An async `syscommand` for [stc](https://github.com/LunarWatcher/stc)
2. An stc API for input and output streaming
3. A standalone testing library for integration testing interactive CLI apps

Not entirely sure if #3 is going to be standalone, or if it ends up being emergent behaviour of the API from #1/#2.

## Goals

1. Check whether or not I can manipulate the stdin of the current app and have it propagate deeper
    * I assume this to be obviously impossible going into this, but I'm trying anyway. This should allow some direct invoking of the main method in certain cases.
    * **TL;DR:** Failed, as expected. This might make sense if I bothered reading up on the internals of how `exec*` handles stdin and stdout if nothing is defined, but that's a problem for future me
2. Check how I can fuck around with stdin for spawned subprocesses, and what the limitations of doing so are.
    * pipe() lets me modify the stdin and stdout/stderr of a spawned subprocess, but does this propagate further if that subprocess then spawns a subprocess? More specifically, will it revert to tty if it isn't overridden?
    * **TL;DR:** mission fucking accomplished
3. Do I have to use a PTY?


## Results

### Propagation (goal 1)

As expected, redirecting `std::cin` does not propagate. This is still a potentially useful approach for standalone CLI apps though, but not one that spawns subprocesses. One quirk of doing it this way is that, if the buffer runs out of input, it outputs an empty string:

```

stdin got [["hi"]]
stdin 2 got [[""]]
stdin 3 got [[""]]
Control buffer contains hi

# Buffer wiped. Note that the empty line before this comment is in the original output,
# because the buffer contains a newline that's printed along with the "hi".
# Even fully wiping the buffer is not enough to shut it up
stdin 4 got [[""]]
Control buffer contains 
```

### Stdin propagation with properly modified subprocesses (goal 2)

#### Redirecting

Doing the actual redirect is fairly easy; spawn a second pipe, write to it, poof, done. 

When doing the writing, it looks like `close(stdinFd[1])` in the host is enough to terminate the subprocess. The test setup was initially set up to respond to `line == EOL` as an exit flag because I was being lazy, but closing the file descriptor is required to terminate properly. 

Using streams directly does not make sense for this API. Maybe using a stream-like API, but not rawdogging streams. This might make sense for output, but I haven't decided.

#### Sub-subprocesses

Mission fucking accomplished here.
```
❯ fmake && fmake run 
[ 25%] Building CXX object src/CMakeFiles/terminal-fuckery.dir/terminal-fuckery/Main.cpp.o
[ 75%] Built target mocksub
[100%] Linking CXX executable ../bin/terminal-fuckery
[100%] Built target terminal-fuckery
[ 66%] Built target terminal-fuckery
[100%] Run terminal-fuckery
wrote 22 bytes (echo 'This is line 1'
)
Out of loop
wrote 22 bytes (echo 'This is line 2'
)
Out of loop
Process exit status is 0
Output:
-------
mocksub has started
argc=2
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ echo 'This is line 1'
This is line 1
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ echo 'This is line 2'
This is line 2
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ exit
Exit code: 0

=== END ===
[100%] Built target run
```

In this setup:

* `terminal-fuckery` (executed by `fmake run`) invokes `./bin/mocksub`, and hijacks its stdin and stdout. `stdout` is put in a string, `stdin` receives the contents of an `std::stringstream`
* `./bin/mocksub` runs `/usr/bin/env bash -i`. `stdout` is not captured, and `stdin` is not touched
* The output shows the combined output from `mocksub` and `bash`. `bash` got two commands via its stdin (which contains the contents of the stringstream from `terminal-fuckery`)
    * I don't know where the `exit` command comes from, but stdin has to be closed once done for the nested terminal to terminate properly, so I half assume pipe closed == `exit`

### Do I have to use a PTY, and when do I use it? (goal 3)

The solution for goal 2 does not seem to allow for translation of control signals and similar, which is an issue. As far as I can tell, this requires a PTY to work properly. Hwoever, the program I want to directly execute with the testing library I'm working on (umbra) wouldn't directly be a TTY in the first place, and I'm not sure how it would react. 

This random gist seems to suggest no: https://gist.github.com/zmwangx/2bac2af9195cad47069419ccd9ee98d8

Actually testing confirms it isn't a problem. However, when interacting directly with a terminal, shit gets weird. The default behaviour is to echo abck the command. This is what I want, but results in a redundant copy caused by the input being written to the same fd we're reading from later:
```
Output: echo 'hi'
exit 69
mocksub has started
argc=2
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ echo 'hi'
hi
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ exit 69
exit
Exit code: 69
```

There is a way to disable it, but this hides all the commands:
```
Output: mocksub has started
argc=2
bash: cannot set terminal process group (2427208): Inappropriate ioctl for device
bash: no job control in this shell
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ 
hi
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ 
exit
Exit code: 69
```

This is not great either, but is marginally better.

Re: the "Inappropriate ioctl for device" message, this appears to come from a missing `setsid` in stc. It is present in the runs with echo enabled as well, but I temporarily added `setsid` for that run, and that patch reverted between the two runs. Weird cmake shit is weird.

I'm not sure when `setsid` is desired, but it looks like it's more required when doing shit with a full PTY. I ahve `setsid()` in my code here to avoid that problem when directly invoking bash, but when invoking `mocksub` that then uses stc to spawn the terminal, that does not seem to work. 

As an aside, output with `openpty` is fucking jank. `read` can return `-1`, which resulted in an extra edge-case. This also means stc has a problem, and has to be modified to not use `size_t` when `read` is used.

I'm not sure what the code structure is going to end up being for the PTY interface. If anything, this experiment raised more questions than answers. 

There's so many options that can be set and that depend on just execution context. I think the test interface is going to end up doing a lot more polling than a real-world interface would. 

Fascinatingly, this seems to sync back up if there's a sleep:
```
Output: mocksub has started
argc=2
bash: cannot set terminal process group (2429228): Inappropriate ioctl for device
bash: no job control in this shell
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ echo 'hi'
hi
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ 
olivia@vixen:~/programming/experiments/terminal-fuckery/build$ exit 69
exit
Exit code: 69
```

Maybe it isn't really an echo, but just a consequence of being so fast you beat the terminal's processing speed?

The sleep only in the loop (line 198 in Main.cpp) causes `echo 'hi'` to print at the top, while the sleep at line 186 removes it again. This suggests that it's purely a sync thing.

On the bright side, I can send ctrl-c, but the behaviour around it in bash is jank. I don't know if this in particular is a consequence of the two layers of indirection, but I do see `^C` printed in the terminal. It's unable to interrupt a `sleep 10`, but it's possible this is just due to the jank way I actually send the command. Further testing is needed.

## Conclusions

A PTY is necessary to properly emulate the environment a terminal expects. However, I'm still not sure how well the PTY handles the nesting I want to do when I don't actually spawn a PTY to run the nested bash. There's something jank going on here, and I don't know what exactly. 

However, the initialisation and read methods are mostly identical for both cases. A common and better API for command execution based around exec should allow arbitrarily picking between PTY and non-PTY execution. 

Investigating whether `poll` is necessary or at least usable for non-PTY use is necessary to see how common that interface actually would be. 

This is rapidly becoming the first addition to stc that has no cross-platform equivalents. The Windows API equivalents are so disgustingly involved that I'm not even going to try. To be fair, the array signature of `exec` is already UNIX-only, so this isn't that big of a leap, and fork() and many of the other mechanics simply do not work on windows.

`pty.h` is apparently not available [with mingw either](https://www.gnu.org/software/gnulib/manual/html_node/pty_002eh.html), and it's missing on certain platforms. I did not bother googling what the latest version of any of those platforms are, so I don't know if those are the current versions or one-offs that were fixed, and I don't care right now. Extended cross-platform support can be added when needed by end-users.

