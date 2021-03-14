# SDL Learning Examples

This repository is one for a class I'm preparing on using C and C++ with the Simple DirectMedia Layer (SDL) library.

There is a bare minimum logger implementation provided. To get that working you just do this:

```
make CFLAGS="-D LOG_USE"
```

If you want colors and your terminal supports it, you can do this:

```
make CFLAGS="-D LOG_USE -D LOG_USE_COLOR"
```
