<p align="center">
	<h1 align="center">City Builder</h2>
	<p align="center">An isometric C++/SDL2 city building game</p>
</p>
<p align="center">
	<a href="./LICENSE">
		<img alt="License" src="https://img.shields.io/badge/license-GPL-blue?color=7aca00"/>
	</a>
	<a href="https://github.com/LordOfTrident/city-builder/issues">
		<img alt="Issues" src="https://img.shields.io/github/issues/LordOfTrident/city-builder?color=0088ff"/>
	</a>
	<a href="https://github.com/LordOfTrident/city-builder/pulls">
		<img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/LordOfTrident/city-builder?color=0088ff"/>
	</a>
	<br><br><br>
<!--	<img width="450px" src="res/clip.gif"/> -->
</p>

## Table of contents
* [Introduction](#introduction)
* [Quickstart](#quickstart)
* [Milestones](#milestones)
* [Controls](#controls)
* [Bugs](#bugs)
* [Dependencies](#dependencies)
* [Make](#make)

## Introduction
I love the feel of isometric games, so this is my second attempt
([click here for the first one](https://github.com/lordoftrident/your-factory)) at making one. This
will be a simple city building game, because building is fun.

## Quickstart
```sh
$ make
$ ./bin/app
```

## Milestones
- [X] UI system (and the main menu)
- [X] First tiled map, you can also move around it freely
- [ ] Selecting tiles, changing them
- [ ] Buildings, placing and deleting them
- [ ] Finish the main menu, add settings
- [ ] Connectable roads
- [ ] Time, day/night system
- [ ] Save/load worlds and a menu for that
- [ ] Restaurants, buildings that make you money

## Controls
| Key         | Action                  |
| ----------- | ----------------------- |
| W           | Move camera up          |
| A           | Move camera left        |
| S           | Move camera down        |
| D           | Move camera right       |
| RMB         | Drag to move the camera |
| Scrollwheel | Zoom in/out             |

## Bugs
If you find any bugs, please create an issue and report them.

## Dependencies
- [SDL2](https://www.libsdl.org/)

## Make
Run `make all` to see all the make rules.
