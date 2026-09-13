# TUI

I couldn't help myself. This is a follow-up to the GTK and Qt experiments to try to identify how feasible it is to implement a TUI library

## Notes

### Codes
https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 has a fairly decent list of ANSI codes. TIL wtf OSC means though, I thought it was a parallel standard, but it's just a subcategory of ANSI.

https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html has some useful stuff on raw mode. Raw mode is going to be the main issue for a proper cross-platform implementation, since raw mode  uses platform-specific features. Not a problem for my use-case (for now), but might be worth accounting for future support

## Important codes

* `\r`: moves the cursor to the start of the line
* `ESC[2K`: Clears line
* `ESC[#A`: Moves the cursor up `#` characters
  * Similar codes for ABCD, which together with `\r` for anchoring should make it doable to do partial redraws
  * I believe I read somewhere at some point ages ago that partial redraws are necessary for screenreaders?
    * I did find another article while double-checking this, but it was of course written by an AI sloperator
  * hiding the cursor when drawing non-(user-)text should help here

## Results

The ftxui source is completely beyond any form of salvage. Treating the screen as an array of pixels is how it works internally, which makes any attempt to salvage accessibility incompatible with its core design. I have not attempted to confirm this with an actual screenreader, as I don't know how to use them. There might be something in the internals somewhere that salvages it, but a default full redraw is going to interfere with screenreaders, at least per some descriptions of how they seem to work with terminals.

Doing anything other than this is involved, and involves fairly heavy use of even more ANSI codes. When scroll also joins chat, this gets quite a lot harder.

Dealing with accessibility in a GUI is mostly a matter of supporting the right native libraries[^1], which can be fairly easily added as a part of existing structures. The TUI approach would require implementing partial redraws by design, which is a fair bit more involved.

I'll probably revisit this at some point. It would certainly be an interesting project.

[^1]: I know fuckall about this in practice, because I have never actually done it. Alui, my allegro-based GUI meant for game use, does not currently implement any of this. It's also way too early in development, since I'm still busy rewriting my shitty flex algorithm implementation, but that's a tangent. But - I think what I said only applies to non-"game" GUI content. I have no fucking clue how accessibility tools go about doing interactive content where object positions in a potentially 3D canvas are in play. I would imagine they usually don't, and implement their own systems instead (or presumably more commonly, don't bother at all).

  However, for screenreaders doing text or standard GUI elements, you can just export the "DOM" of the app via for example [AT-SPI2](https://en.wikipedia.org/wiki/Assistive_Technology_Service_Provider_Interface) on Linux, and fuck knows what on Windows. This might still be doable with a TUI
