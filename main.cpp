#include "list.h"
#include <cstdlib>
#include <iostream>

using namespace std;

#define KEY_LEFT 68
#define KEY_RIGHT 67
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_BACKSPACE 127
#define KEY_ENTER '\n'

int main() {
  Notepad pad;

  system("clear");

  while (true) {
    system("clear");
    pad.display();
    char ch = getch();

    if (ch == 27) {
      break;
    } else if (ch == '\033') {
      getch();
      switch (getch()) {
      case KEY_UP:
        pad.moveCursorUp();
        break;
      case KEY_DOWN:
        pad.moveCursorDown();
        break;
      case KEY_LEFT:
        pad.movecursorleft();
        break;
      case KEY_RIGHT:
        pad.moveCursorRight();
        break;
      }
    } else if (ch == KEY_BACKSPACE) {
      pad.deleteCharBeforeCursor();
    } else if (ch == KEY_ENTER) {
      pad.insertNewLine();
    } else if (ch == '\t') {
      pad.insertChar(' ');
      pad.insertChar(' ');
    } else {
      pad.insertChar(ch);
    }
  }

  system("clear");
}
