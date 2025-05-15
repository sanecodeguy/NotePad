#include <cstdio>
#include <fstream>
#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;
char getch() {
  char buf = 0;
  termios old = {};
  fflush(stdout);
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  termios new_ = old;
  new_.c_lflag &= ~ICANON;
  new_.c_lflag &= ~ECHO;
  new_.c_cc[VMIN] = 1;
  new_.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &new_) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return buf;
}

void gotoxy(int x, int y) { printf("\033[%d;%dH", y + 1, x + 1); }
class Node {
public:
  Node(char value)
      : value(value), prev(nullptr), next(nullptr), up(nullptr), down(nullptr) {
  }

  char getValue() const { return value; }
  void setValue(char newValue) { value = newValue; }

  Node *getPrev() const { return prev; }
  void setPrev(Node *newPrev) { prev = newPrev; }

  Node *getNext() const { return next; }
  void setNext(Node *newNext) { next = newNext; }

  Node *getUp() const { return up; }
  void setUp(Node *newUp) { up = newUp; }

  Node *getDown() const { return down; }
  void setDown(Node *newDown) { down = newDown; }

private:
  char value;
  Node *prev;
  Node *next;
  Node *up;
  Node *down;
};

class Notepad {
private:
  Node *head;
  Node *tail;
  Node *cursor;
  Node *currentLine;

public:
  Notepad()
      : head(nullptr), tail(nullptr), cursor(nullptr), currentLine(nullptr) {}

  void displaycursor() {
    Node *currLine = head;
    while (currLine) {
      Node *currChar = currLine;
      while (currChar) {
        if (currChar->getValue() == '_') {
          cout << "_";
        }
        currChar = currChar->getNext();
      }
      currLine = currLine->getDown();
    }
  }
  void display() const {
    Node *currLine = head;
    int y = 0;

    while (currLine) {
      Node *currChar = currLine;
      int x = 0;
      while (currChar) {
        gotoxy(x, y);
        if (currChar == cursor) {
          cout << "\033[7m" << currChar->getValue() << "\033[0m";
        } else {
          cout << currChar->getValue();
        }
        currChar = currChar->getNext();
        x++;
      }
      y++;
      currLine = currLine->getDown();
    }

    cout.flush();
  }

  void movecursorleft() {
    if (cursor && cursor->getPrev()) {
      cursor = cursor->getPrev();
    } else if (currentLine && currentLine->getUp()) {
      currentLine = currentLine->getUp();
      cursor = currentLine;
      while (cursor->getNext()) {
        cursor = cursor->getNext();
      }
    }
  }

  void moveCursorRight() {
    if (cursor && cursor->getNext()) {
      cursor = cursor->getNext();
    } else if (currentLine && currentLine->getDown()) {

      currentLine = currentLine->getDown();
      cursor = currentLine;
    }
  }

  void moveCursorUp() {

    if (cursor && cursor->getUp()) {

      currentLine = currentLine->getUp();
      Node *prevLineChar = cursor->getUp();
      while (prevLineChar && prevLineChar->getDown() != currentLine) {
        prevLineChar = prevLineChar->getDown();
      }
      if (prevLineChar) {
        cursor = prevLineChar;
      } else {

        cursor = currentLine;
      }
    }
  }

  void moveCursorDown() {
    if (cursor && cursor->getDown()) {

      currentLine = currentLine->getDown();
      Node *nextLineChar = cursor->getDown();
      while (nextLineChar && nextLineChar->getUp() != currentLine) {
        nextLineChar = nextLineChar->getUp();
      }
      if (nextLineChar) {
        cursor = nextLineChar;
      } else {

        cursor = currentLine;
        while (cursor->getNext()) {
          cursor = cursor->getNext();
        }
      }
    }
  }
  void deleteCharBeforeCursor() {
    if (cursor && cursor->getPrev()) {
      Node *prevChar = cursor->getPrev();
      Node *prevPrevChar = prevChar->getPrev();
      if (prevPrevChar) {
        prevPrevChar->setNext(cursor);
      } else {
        currentLine = cursor;
      }
      cursor->setPrev(prevPrevChar);
      delete prevChar;
    } else if (currentLine && currentLine->getUp()) {

      Node *prevLine = currentLine->getUp();
      Node *prevLineChar = cursor->getUp();
      while (prevLineChar && prevLineChar->getDown() != currentLine) {
        prevLineChar = prevLineChar->getDown();
      }
      if (prevLineChar) {
        prevLineChar->setDown(nullptr);
      } else {

        prevLineChar = currentLine;
      }
      cursor = prevLineChar;
      while (cursor->getNext()) {
        cursor = cursor->getNext();
      }
      cursor->setNext(prevLine);
      prevLine->setPrev(cursor);
      currentLine = prevLine;
    }
  }
  void insertChar(char ch) {
    if (ch == '\n') {

      Node *newLine = new Node('\n');
      if (cursor) {

        Node *nextLine = currentLine->getDown();
        if (nextLine) {

          newLine->setDown(nextLine);
          nextLine->setUp(newLine);
        }
        currentLine->setDown(newLine);
        newLine->setUp(currentLine);
        cursor = currentLine = newLine;
      } else {

        head = tail = cursor = currentLine = newLine;
      }
      return;
    }

    Node *newNode = new Node(ch);
    if (!head) {

      head = newNode;
      tail = newNode;
      currentLine = newNode;
      cursor = newNode;
    } else if (cursor) {
      if (cursor->getNext()) {

        newNode->setNext(cursor->getNext());
        cursor->getNext()->setPrev(newNode);
      } else {

        tail = newNode;
      }
      newNode->setPrev(cursor);
      cursor->setNext(newNode);
      cursor = newNode;
    } else {

      newNode->setNext(currentLine);
      currentLine->setPrev(newNode);
      head = newNode;
      cursor = newNode;
    }
  }

  void insertNewLine() {
    Node *newLine = new Node('\n');

    if (currentLine) {
      Node *nextLine = currentLine->getDown();
      if (nextLine) {
        newLine->setDown(nextLine);
        nextLine->setUp(newLine);
      }
      currentLine->setDown(newLine);
      newLine->setUp(currentLine);
    } else {
      head = tail = currentLine = newLine;
    }

    cursor = currentLine = newLine;
  }

  void deleteChar() {
    if (cursor && cursor->getNext()) {

      Node *nextNode = cursor->getNext();
      cursor->setNext(nextNode->getNext());
      if (nextNode->getNext()) {
        nextNode->getNext()->setPrev(cursor);
      } else {

        tail = cursor;
      }
      delete nextNode;
    } else if (currentLine && currentLine->getDown()) {

      Node *nextLine = currentLine->getDown();
      if (nextLine->getNext()) {

        tail->setNext(nextLine->getNext());
        nextLine->getNext()->setPrev(tail);
        tail = nextLine->getNext();
      } else {

        tail = cursor;
      }
      currentLine->setDown(nextLine->getDown());
      if (nextLine->getDown()) {
        nextLine->getDown()->setUp(currentLine);
      }
      delete nextLine;
    }
  }
};
