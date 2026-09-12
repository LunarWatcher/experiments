# Qt testing

## Purpose

Potential TUI replacement since ftxui's flexbox algorithm is beyond fucked. There's extremely few proper GUI libraries for general-purpose use. For cross-platform use, gtk and qt are functionally the only options. wxWidgets is also an option, but this is just gtk, qt, and whatever microslop windows and crapple uses in a trenchcoat. 



## GTK notes

GTK is a lot more lacking in widgets than GTK. For my use, this is a problem.

I did initially have notes here on GTK breaking backwards compatibility, with plans to do so again in the upcoming (at some point:tm:) GTK 5. This complaint was rendered moot by the Qt experiment (see later notes) showing that this isn't a GTK thing.

## Notes

### Docs and help resources

The docs are fucking worthless. They're written under the assumption that you use their specific-purpose IDE, and that isn't going to happen.

There's not one, but two different shitty text-based formats for representing layouts, QML and some XML shit. Everything assumes you use Qt creator or qt designer and QML or the XML shit. The best proper raw C++ resources available are the raw API docs, and that is heavy fucking reading. 


### Backwards compatibility

Like GTK, backwards compat is functionally non-existent. To GTK's credit, however, the docs for at least gtkmm are at least relatively good. The GTK builder is mentioned, but isn't like a key focus. It's mentioned in chapter 26, long after all the relatively useful docs.

Though a lot of things remain fairly similar, a lot of things have broken. This also affects docs and help resources, as described earlier, and is further complicated by Qt being commercial. The commercial part means that qt also has LTS releases, and those LTS releases still actively see questions being asked about them. This means that it's an utter pain to find current information about qt6 - again, unless you use creator or designer or whatever the fuck.

### Ease of usen't

Because all the editors were prioritized over the code, the code itself is a pain to work with - in some ways. There's at least lots of components available, and it's pretty easy to get like the general vibe of the program, but holy fuck is it a pain in the ass to debug anything visual not working.

Also, wtf is up with widgets? Widgets are widgets except when widgets are windows? Fucking why?

### We have MVC at home

Qt makes heavy use of pointers by default, while also taking ownership of them, while also allowing third party deletion from outside the context of the node.

I have... mixed feelings about this. It's really neat, because it means you can just `delete` things and have it fuck off, but it also makes RAII harder to reason about. 

## Conclusion

The only real thing I got out of this is eliminating GTK on account of the component library.

The overlap between a usable, sane GUI library and a GUI library that's actually capable of making apps by modern standards (in practice, read: that also has support for very advanced stuff (/s) like exporting a view of the DOM to the platform's accessibility tool) is the null set. It also doesn't help that there are even more requirements that would be great, but it's still all the null set. Qt and GTK meets all the technical requirements for the end-user, but none of the dev requirements. Qt also seems to not meet certain quality measures based on some blog posts of potentially questionable quality. 

I'm also more concerned about the future of the actual code. I imagine the raw code can't go away, since it's also used to implement QML et. al, but the more QML-centric Qt gets, the harder it'll be to not use it. I am very much influenced by the negative direction android took here, however. Tbf, android's pissing contest was much more about it being indecisive on frameworks and not really the layouts[^1]. The support libraries kept breaking, so all associated code kept breaking. Good 'ol bit of the npm ecosystem there - break everything every major version or you're not doing it right:tm:. 

That said, this has made me realize why electron is so popular, as well as why turning shit into webapps is popular: HTML, CSS, and plain browser JS is the single best GUI library currently in existence. It works consistently on every platform, standard code is comprehensible[^2]. Because of its popularity, all the browser builtins have been significantly beefed up as time has passed, so less needs special-purpose JS to work. Except inexplicably form validation, which still has very flaky browser support outside desktop. (Fix your shit, firefox for android[^3])

Another part of the question was whether or not it was worth it to try to rewrite a TUI from scratch. I'd argue there absolutely is merit to the idea. The standards available to terminals have grown over the years, but not at the expense of anything. As long as it still compiles, you can in theory still run a TUI from the early 2000s, and it'll look fucking identical. The terminal and HTML5 seem to be the only two things that have remained stably good.

I do half suspect the final decision is going to be going for Qt with TUI shit being a future me problem. The main downside with this is that it just makes this so fucking hard compared to a TUI. It has uses, sure, I'm just not convinced I've found any such use-cases right now

---

I have spent far more time on this than I'd like to admit. This part of the doc is written a day after the rest because decisions fucking hard.

I think part of the problem is that I want to make something I don't have to put much effort into maintaining. I spend enough time at work doing pointless rewrites because some library has decided to break its APIs again, I don't want to risk having to do that with utilities I write. 

I did have a brief shower thought to make a meta-library that kinda deals with that, but I don't feel like this would scale particularly well. To be more specific, it would be kinda cool if I could make it port to multiple UI frameworks, so I can also port it to the terminal if I ever get around to rewriting andromeda. The challenge is mostly in data handling and layout handling. Qt currently relies on MVC for a few things (like filtering; `QSortFilterProxyModel`), and doesn't really have a proper flexbox, so the actual layouts are going to be hard to port. In terms of purely abstracting away Qt, there's already wxWidgets, though it has a lot of the same shit patterns (`bind` is pretty much exactly equivalent to `signal` in qt. gtkmm very much wins here on ease of use of registrering handlers).

The filtering is somewhat nice at least, probably the one redeeming quality of qt. Subclassing appears to be the only way to add new kinds of filters. Not sure how relevant that is for my use-case, but it came up in research.

---

I also think a GUI app could work here, but it requires a very different design from the TUI implementation. I can't picture that in my head right now though, so will need to figure out how best to do it later:tm:. Though the part I struggle with here is mostly just that there's more space available in a GUI, as well as more ways to visually represent things 

[^1]: When I used it\*. I hear android since then has moved on to some kotlin DSL shit 
[^2]: read: not frameworks. React et. al make JS much, much worse.
[^3]: Safari has the same issue (allegedly), but no one cares about crapple

