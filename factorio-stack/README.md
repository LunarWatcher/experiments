# factorio-stack [ongoing]

Minor test game based on the same-ish core tech stack as factorio, with SDL2, allegro5, and agui

## Rationale

Internationalised game, and (AFAIK) no display issues with internationalised text.

Factorio _is_ wonky with ibus as IME input, but this seems to be the case for all games:
* https://old.reddit.com/r/archlinux/comments/syr0p7/ibus_in_games/

Based on reports, it seems that the main problems are input-related (shock), and boil down to two categories;
1. Input method, possibly specifically ibus, that doesn't seem to cooperate with any games (fun fact: IBus IME input doesn't even work with the search bar in my start menu. It does work in FF though)
2. Misc. problems with how other languages are treated in Factorio's code: https://factorio.com/blog/post/fff-436

It does seem that factorio directly interacts with IME (https://forums.factorio.com/116118), so certain input things may still need to be handled in the game, but that's a future me problem that I can manage. Text rendering, on the other hand, is just cancer to sort out, and is _the_ main bit I don't want to work extensively with. Along with standardised input and UI rendering, because these are solved problems, but are also wildly complex when not constraining what's being rendered.

## Results
* Including both necessary and unnecessary boilerplate, and various other useless overengineered shit largely based on a tutorial from the allegro wiki, font rendering with full unicode support (given a font that supports the characters) is done in just 74 lines of code. For something that isn't a full game engine, that's pretty damn good. This is also prior to fucking around with agui


