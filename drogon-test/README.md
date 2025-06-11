# drogon-test

## Results

Really, really neat, but also really, really awkward library.

The default `.csp` format is really awkward. The syntax is not supported by Vim, and they require a special drogon-specific tool to generate a header/source file pair. Right pain in the ass to use compared to inja or mustache, and requires a bunch of extra CMake extensions for it to work. 

It _does_ look more stable than crow at least, but it looks much worse to work with code-wise. Especially in light of 2.0 having several improvements, but not even being public over 4-5 years in. 

### Libraries
* The built-in JSON library (jsoncpp, never heard of it before) is apparently [significantly slower than nlohmann/json](https://github.com/drogonframework/drogon/issues/682#issuecomment-758337004). This isn't breaking, but it does mean all JSON needs to be handled externally. This would likely need to be the case anyway now that I'm switching over to reflectcpp + yyjson.
* Trantor (some library I've never heard of) is used for dates some reason. Probably because legacy code, and it is being refactored[^1]. 


### Misc.
* Drogon 2.0 has breaking changes (including switching to nlohmann/json), but 2.0 is not even on a publicly available branch.[^1] This is mildly annoying, because it means it isn't possible to get ahead of the most breaking changes in the four years since that comment was made[^2].


[^1]: https://github.com/drogonframework/drogon/issues/806#issuecomment-820425308
[^2]: The comment suggests a total rewrite, so it's understandable that it wasn't released immediately, but it's been four years and still nothing. 
