# allegro-clay

This experiment pokes around [nicbarker/clay](https://github.com/nicbarker/clay/) with allegro5.

## Results

Cool library, but the macros spew out fucktons of warnings.

There's several things I'm not sure about, particularly around event handling. I would assume its' designed to deal with async/callback-based events rather than updating, for example, the mouse position only once before updating. Not sure how it affects the UI, because I didn't get as far as setting up a decent UI - only a few base rectangles.

Setting up text size calculation is not going to be fun, but it is interesting to see how clay handles it. 

I'll be keeping an eye on the library, but I think that for my use, it's not going to be super applicable.

That said, one key takeaway from it for use in [alui](//github.com/LunarWatcher/alui) is the separation of rendering from the layout data. I'm not sure how this can be done in practice, though. The hard separation done by clay is required, because Clay doesn't include any code on how things could be rendered, whereas alui default-handles it in a certain way. That said, having read up on a few of the Factorio FFFs, particularly the ones where layout bounding boxes were highlighted with lines[^1], it's pretty clear that how these things could be handled boils down to far more than just render some base UI elements and call it a day. 

Speaking from experience in ftxui (a TUI library), where I had to manually customise some built-in elements for a reason I don't remember off the top of my head, supporting drop-in replacements -- especially in OOP -- is both necessary and difficult. Might be worth stepping slightly back from OOP and looking at alternative ways to represent the data?

That said, I'm pretty sure Clay falls into this pit as well. I _think_ it might have a way around it, but I haven't bothered checking. It, as far as I can tell, supports background images and background colour, but what about background gradients? Or cursed half-background colour, half-background image? Just to be clear, this isn't intended as a criticism of Clay, but it's meant to show that layouts are hard. Also, to be fair, stuff like that isn't directly supported in CSS either. CSS _does_ support a fair amount of weird stuff, but like the background being split in half isn't something doable with just CSS. I'm pretty sure this would require either cursed CSS (with `::before` or whatever), or extra dummy layout elements overlaying each other, i.e. (pseudo-pseudocode):
```html
<div class="background-image">
    <div class="background-colour-left-half">
        Content
    </div>
</div>
```

Which you can do in Clay, and eventually in Alui as well, but maybe it could be argued that there's optimisation benefits to avoiding this?

Granted, not all cases are like this. Background gradients, for example, are a completely different beast - and possibly one that would require special shaders to do rather than primitive calls. Then you run into that - what if you want to throw shaders on specific UI elements? You can apply the shader to an entire `alui::GUI`, which means you could separate shaded and non-shaded elements, but that would also require separate shader calls. 

I am absolutely overthinking here, and I won't be implementing a good chunk of this myself, at least not in the short term, but I want to have the option to do so trivially - which is challenging to design.

Again - really neat library, just not quite what I need for any of my use-cases.



## Footnotes

[^1]: Good 'ol https://factorio.com/blog/post/fff-216 - this is already done in alui, but only in debug and only with a specific macro enabled. It is absurdly useful in tests though, especially because some of my (at the time of writing) newer tests include a render step that outputs to a `.bmp`. It's just really neat.
