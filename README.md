# Gravity Defied Cpp

![](./screens/demo1.gif)

**Gravity Defied** is a legendary mototrial racing mobile game. It was originally developed by Codebrew Software in 2004 for J2ME platform.

This is the C++ & SDL2 port of Gravity Defied, which includes all the features of the original game.

***This Project is not associated with Codebrew Software in any fashion. All rights to the original Gravity Defied, it's name, logotype, brand and all that stuff belong to Codebrew Software.***

# About the fork

- overhauled saving data to files (simplified, changed it to a C/C++ way)
- fixed: game does not save progress after the first save
- fixed: saving high score
- overhauled controls logic (simplified, per key setting instead of predefined key sets)
- added fullscreen and proper scaling for different aspect ratios
- renamed portions of a code to something meaningful

# TODO

- [X] create saves directory and global setting file when missing
- [ ] config file
- [X] command line flags
- [ ] level file selector
- [ ] further refactoring
- [ ] scaling game (or at least font)

# Authors

### Gravity Defied Cpp authors
* **[rgimad](https://github.com/rgimad)** - idea & coding
* **[AntonEvmenenko](https://github.com/AntonEvmenenko)** - coding
* **[turbocat2001](https://github.com/turbocat2001)** - coding

### Codebrew GDTR Authors

* **Tors Björn Henrik Johansson** - system/game logic/interface, testing, levels design
* **Set Elis Norman** - graphics/physics/mathematics/system/tools programming, levels design
* **Per David Jacobsson** - physics programming, game graphics, levels design

For more information, please visit official site of Codebrew Software: [codebrew.se](http://codebrew.se)
