This repo is a collection repo of misc. experiments. The code here is mainly kept as a public archive, and because it's low-quality experiments, the code quality is likely questionable. If you plan to use anything from this repo, please remember that this repo is not a shining example of anything.

There are two possible experiment states; `ended` and `ongoing`. If you're looking for examples of stuff to point and laugh at, I suggest looking at experiments that have ended and not experiments that are still ongoing.

This repo is intentionally undocumented, aside the bare minimum I need to understand the code, and some various results. Please don't open issues asking for docs.

---

Current experiments in reverse chronological order:
* `drogon-test`: Quick test of [drogon](https://github.com/drogonframework/drogon) to see if it works as a replacement for crowcpp (due to significant bugs)
* `reflect-cpp`: Mini-test fucking around with reflect-cpp. Nothing too fancy. Largely exists to test some small things before I decide to fully switch from `nlohmann/json`. I do like nlohmann/json, but the code overhead for deserialising structs, especially when the deserialisation (and serialisation) is primitive is just annoying to implement. Plus, it reflect-cpp uses yyjson as its JSON backend, and yyjson is fast enough that I [already use it directly in a project](https://github.com/LunarWatcher/se-data-dump-transformer/blob/3257c034980665b0a29469f1418686f3379d288a/transformer/CMakeLists.txt#L77-L82) that needs the gigabyte writes promised by yyjson.
* `allegro-clay`: Brief minimal Allegro renderer for Clay
* `factorio-stack`: test game based on the same core stack as Factorio (SDL2, Allegro5, AGUI)
* `raylib`: test game with raylib that ended up just being screwing around with fonts

