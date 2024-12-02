# factorio-stack [ended]

Minor test game based on the same-ish core tech stack as factorio, with SDL2, allegro5, and agui

## Rationale

Internationalised game, and (AFAIK) no display issues with internationalised text.

Factorio _is_ wonky with ibus as IME input, but this seems to be the case for all games:
* https://old.reddit.com/r/archlinux/comments/syr0p7/ibus_in_games/

Based on reports, it seems that the main problems are input-related (shock), and boil down to two categories;
1. Input method, possibly specifically ibus, that doesn't seem to cooperate with any games (fun fact: IBus IME input doesn't even work with the search bar in my start menu. It does work in FF though)
2. Misc. problems with how other languages are treated in Factorio's code: https://factorio.com/blog/post/fff-436

It does seem that factorio directly interacts with IME (https://forums.factorio.com/116118), so certain input things may still need to be handled in the game, but that's a future me problem that I can manage. Text rendering, on the other hand, is just cancer to sort out, and is _the_ main bit I don't want to work extensively with. Along with standardised input and UI rendering, because these are solved problems, but are also wildly complex when not constraining what's being rendered.

> ![NOTE]
> Corrections:
> * Allegro doesn't support IME input, so factorio's implementation (that doesn't cooperate with ibus) might be a hack?
> * Agui has been heavily modified by factorio (and those modifications are closed-source), input in the main version appears to be borked. Further experiments required
> * Implementing a GUI library from scratch in Allegro doesn't look like it would be the worst thing to do

## Results 

### Allegro in general
* Including both necessary and unnecessary boilerplate, and various other useless overengineered shit largely based on a tutorial from the allegro wiki, font rendering with full unicode support (given a font that supports the characters) is done in just 74 lines of code. For something that isn't a full game engine, that's pretty damn good. This is also prior to fucking around with agui
* **Single line image rendering** (not including image loading, which is another 2 lines)
* **Single line primitive drawing**
* Complex primitives are also a single call, but obviously requires vertices sorted out manually
* ✨ Supports shaders ✨, both GLSL and whatever the fuck HLSL is 
    * I _think_ HLSL is for direct3D. Might need to force OpenGL for it to cooperate. Should probably do that anyway.

AKA _exactly_ what I'm looking for.

### GUI
* Misc. agui stuff:
    * The input fields doesn't display the IME thing (at least under Linux), but it accepts at least Korean input correctly. It failed to display the emoji input thing and unicode input thing, which suggests some further input magic is required to properly work with IME
    * The input fields have mysterious double characters when typing in English (`e` becomes `ee`, `E` becomes `eE`). Based on the source code for agui, it appears that the input management system is out of date and doesn't properly handle `KEY_CHAR` and `KEY_DOWN` events. I'm not sure what has changed since agui was made, however.
    * Based on [this post](https://forums.factorio.com/viewtopic.php?t=110516) on the factorio forums (referencing `agui::TableWithSelection`, which does not exist), it looks like Factorio has a private fork of `agui`
        * Also just found [this FFF](https://www.factorio.com/blog/post/fff-216) that doesn't exactly suggest agui is particularly nice. [FFF-212](https://www.factorio.com/blog/post/fff-212) explicitly states that they're using a "heavily modified AGUI"
    * I suspect that if I go down this rabbithole, I'll need to make a fork of agui
* Misc. general GUI stuff:
    * There aren't that many GUI libraries for allegro. 
        * [lgui](https://github.com/frank256/lgui): like agui, not actively maintained, seems to have overall less support and popularity though
        * [EagleGUI](https://github.com/EdgarReynaldo/EagleGUI): Actively maintained, but seems to abstract allegro behind itself. Seems more like a GUI library in the same category as Qt or GTK rather than one meant for games. 
        * Plus various otehrs listed on the wiki that are also dead and not worth mentioning, or is straight up Qt, WxWidgets, or imgui.
    * Though imgui is not inteded for it, I'm starting to suspect it might be the easiest way to implement at least some parts of the UI for the time being. It's explicitly stated that it supports IME stuff: https://github.com/ocornut/imgui/blob/ee2119d7cbe1361689f9a7a6ef6853c1deae08f6/docs/BACKENDS.md?plain=1#L24
    * That said, [allegro does not support IME input](https://github.com/liballeg/allegro5/issues/1191), but I'll ignore that for now. Too low-level for me to do anything about anyway. 

I overall have a weak spot for forking, because it looks like most of the tedious shit is done, and Allegro alone does a lot of heavy lifting. Allegro has functionally native text rendering support with full unicode and TTF support, so it wouldn't be nearly as difficult. Using agui will require significant work before it's even barely usable, which isn't great. On the other hand, doing so might be more useful. 

Looks like the shortest path to a GUI is imgui, while the [!!FUN!!](https://dwarffortresswiki.org/index.php/Fun) path is forking agui.

(Why do I do this to myself?)
