## Documentation

This is a header-only library, as such most of its functional documentation is contained within the "header section" of the
source code in the form of comments. It is highly recommended that you read said documentation before using this library.

## Features

The nibrans library provides a simple adaptive rANS coder using SSE2 acceleration, its features include:

- No hard dependencies besides the standard library, making it fully portable for most purposes
- Allows the adaptive model to be re-used across multiple compressions, useful for networking
- Easy to use as a result of consisting of only three functions (init, encode, and decode)
- Configurable adaption rate with default tuned towards arbitrary binary data

## Attribution

You are not required to give attribution when using this library. If you want to give attribution anyway, either link to
this repository, [my website](https://www.slopegames.com/), or credit me as [BareRose](https://github.com/BareRose).
If you want to support me financially, consider giving to my [Patreon](https://www.patreon.com/slopegames).

## License

Licensed under CC0 aka the most lawyer-friendly way of spelling "public domain".