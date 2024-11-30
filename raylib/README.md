This folder contains an experiment with Raylib 

## Experiment results

### Fonts

Font management is utter cancer. In an early iteration that tried working with unicode, it appears that fonts are static once loaded. All the docs say characters must be loaded alongside the font, which fucking sucks if you have arbitrary input, for example. There's no (trivial) way to load glyphs on-demand without reloading the entire font, which is such a waste.

Dealing with fallback fonts is also difficult, particularly in a UI context. Would probably have to do cursed shit to reliably render text with two different fonts 

### Misc.

`raygui` is explicitly listed as being for development-related stuff - same thing as imgui, but imgui is arguably orders of magnitude more flexible

This also really seems like a very oversimplified wrapper around OpenGL that still requires OpenGL for anything game-related. Would probably be easier to abuse imgui than use raylib. Unfortunately.



