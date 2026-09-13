# Embedding entire other apps in qt

I had the shower thought to embed other apps within qt as a means of avoiding, for example, creating or adding a library for a terminal emulator within qt. This would let me just embed the entire konsole window within the app, or do other forms of automation with other completely unrelated programs.

## Results

Only works under X11, or xwayland or whatever the fuck

Under Wayland, it fails:
```
94854700274736
QWindow::fromWinId(): platform plugin does not support foreign windows.
zsh: segmentation fault (core dumped)  ./test-qt
```

Running
```
make && QT_QPA_PLATFORM=xcb ./test-qt
```

shows that it can still work under wayland, but it only works in this particular case because the target process is also launched within an X11 environment. Not sure if Qt limitation or wayland limitation, but seems to be broken. I don't understand what counts as a foreign window in this case, but subprocess windows (since konsole is spawned as a subprocess in this particular experiment) apparently does not count.

More ~~research~~ cursed ideas needed
