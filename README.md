# Starship Fontana #

This is an example C++ application using the SDL library.
It tries to be as nicely C++11 as possible but do keep in
mind that SDL is written in C and, at some stage, you have
to interface with it.

## Story ##
The evil b’Kuhn has stolen the code to Earth’s defence system.
With this code he can, at any time, defeat the entire human race.
Only one woman is brave enough to go after b’Kuhn. Will she be
Earth’s hero? Puzzle your way though the universe in the company
of Commander Fontana in **Starship Fontana**.

## Installation ##
You will have to have the SDL development libraries installed on
your system.  The easiest way to compile is to use a command-line

```bash
$ g++ -c -std=c++11 src/*.cpp
$ g++ -o 'Starship Fontana' *.o -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
```

which will produce an executable file called "starship" in the
top-level directory.  To execute this file do the following

`$ ./'Starship Fontana'`
 
from the top-level directory.  The game will expect to find the
`assets` directory under its current working directory.

## Credits ##
The sprites in this game come directly from:
[MillionthVector](http://millionthvector.blogspot.co.uk/p/free-sprites.html),
[Flickr](https://www.flickr.com/photos/58782395@N03/5519580228/in/photolist-9pKhUm-9pGfXt-9pGhd4-9pKiho-9pKiEC-6FoPBk-r2oqT7-4qFbfW-SBPcwu-pdrUNN-9j3JXZ-fRXodg-9ozXcs-4wZFyE-dCQ96L-cFfs9G-7rkfvr-cFfrK1-cFfrxC-qZDS4-cFfrWm-8Kkr79-2buHh8-b35wEn-9tFLFk-2buJip-3aQx-dC5zT7-7eo8x1-4rURJR-rzo6Dc-3G6RZ9-8Xa8R7-wCUX7-BJ7Xv-em4PKd-69Yugv-2o3zry-h919e3-2miSSi-dSYXAH-akhS9Y-qneqCe-m3BqxE-iYW1Zj-rcJuLr-4PbReg-PHLYA-eaqoen-quL1zU),
[CoolText] (https://cooltext.com/Logo-Design-Nova),
and are used under the terms of the [CPL 1.0](http://opensource.org/licenses/cpl1.0.php).

The sound files used in this game come directly from:
[ZapSplat] (http://www.zapsplat.com/),
and are used under the terms of the [CPL 1.0](http://opensource.org/licenses/cpl1.0.php).
