# NBody-Simuation.c

## Explaination

This is an nbody simulation written in C. It was made to be very modular, so it can run all the way from the Solar System to a dust particle simulation.

## Current Capabilities

The solar system works great, the dust simulation does not. 

This seems to have mostly to do with needing to move stack allocated arrays for the bodies, etc. to the heap and including a better simulation type for dust simulations that the classic Newtonian integration. 

It would also help to really use the CPU and GPU to parallize the simulation work, etc. 

There are "stubs" for every simulation type I could think of, but Newtonian integration is the only one actually implemented, so you'll get a linker error if you try to use the other ones.

But I currently do not have the time or drive to do this. For now I am not in need of a dust simulation. But it should be relatively simple to add that functionality. If you do please create a pull request so I can merge it.

## How to use GUI

You can move the camera using WASD and look around with the mouse while holding RMB. To increase / decrease speed, (depending on your config), use the modifier key (SHIFT).

With the scroll-wheel the camera speed can be changed while the simulation is running.

ESC brings you to a pause screen where you can quit and have fun with the non-functional settings button.

## How to use the TUI

There is nothing to use, it just prints all stats of all bodies every 1 second (also not good for the dust simulation). This is also very simple to replace. You'd need to either change the existing implementation or add a config option for the ui to use instead of a toggle between GUI and TUI.

## How to setup

I will not go over everything here, but the basics are nbody.h includes main.h in which you define stuff like what data types to use and if it should include the GUI (raylib & raygui). These options will get compiled, so you can only change them by rebuilding. Then you have main.c which actually gets executed, app options there can be changed at runtime.

There are dedicated examples (Solar System and unfinished Dust simulation) on the other branches. Look how it's done there.

## Compatibility

- Windows: **You'd need to replace basic includes and so on, the terminal ui signal handler would work differently for example**

- macOS: **Should work**

- BSD: **Should work**

- general Linux: **Should work**

- Unix-like: **Works**

- NixOS: **Works**

# Dependencies

- raylib (Build script for Unix-like is included) \[If you define ENABLE_GUI}\]

- raygui.h (Download script is included) \[If you define ENABLE_GUI}\]

- unistd.h
