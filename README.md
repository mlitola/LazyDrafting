# LazyDrafting
A command line tool for text editing and making simple ascii drawings.

## Description
I wanted to do some programming with the  new curses lib using standard C (C99) 
and make something simple with it. Made this lazy text editor with a drawing 
mode. Very basic functionality and mostly just a lazy draft.

### Requirements:

- New curses lib
- C compiler (clang, cc)

### Install and compile:
Install ncurses and compile with make:
```
# install ncurses Ubuntu:
sudo apt install libncurses5-dev

# install ncurses MacOS using homebrew:
brew install ncurses

# compile with make (cc or clang used)
make clean && make

```

### Running the binary:
```
./lazydrafting

```

### Todos
  - support for terminal resize
  - support for multiple pages
  - improved drawing with selectable brush
  - improved saving with file name prompt
  - support for colors
  - unit tests
etc.

### Misc

Example art:
```

********  ******  ******   **       **                                        
********  ******  ******   **       **                                        
**        **  **  **  **   **       **                                        
**        **  **  **  **   **       **                                        
**        **  **  **  **   **                                                 
*******   **  **  **  **   *******  **                                        
*******   ******  ******   *******  **

```
