#pragma once

#include <ncurses.h>

#include <string>
#include <utility>

namespace hyped::debug {

enum class KeyPress { kUp, kDown, kEnter, kBackspace, kTab, kEscape, kASCII, kNone };

using TerminalInput = std::pair<KeyPress, char>;

class Terminal {
 public:
  void initialize_window()
  {
    initscr();  // initialize ncurses
    cbreak();   // break with ctrl + c
    noecho();   // dont echo inputs

    int height, width, start_y, start_x;
    getmaxyx(stdscr, height, width);  // get terminal size

    start_y = 0;
    start_x = 0;

    WINDOW *window = newwin(height, width, start_y, start_x);  // create window

    keypad(window, true);    // Use keypad
    nodelay(window, true);   // Non-blocking getch
    scrollok(window, TRUE);  // Enable scroll

    set_escdelay(25);  // Set escape delay to 25ms

    wclrtobot(window);  // Clear window
    wrefresh(window);   // Refresh window
    window_ = window;
  };

  ~Terminal() {}

  /**
   * Adds carriage return to current line
   **/
  void cr()
  {
    wprintw(window_, "\n");
    wrefresh(window_);
  }

  /**
   * Prints a string to current cursor position.
   * Adds carriage return.
   **/
  void println(const std::string &msg)
  {
    wprintw(window_, msg.c_str());
    cr();
  }

  template<typename... Args>
  void printf(const std::string &msg, Args... args)
  {
    wprintw(window_, msg.c_str(), args...);
    wrefresh(window_);
  }

  /**
   * Refreshes line with value and prepends delimiter
   * @param std::string new line value
   * @param std::string delimiter
   **/
  void refresh_line(const std::string &new_value, const std::string delimiter)
  {
    int x;
    int y;
    getyx(window_, y, x);
    wmove(window_, y, 0);
    wclrtoeol(window_);
    wprintw(window_, "%s %s", delimiter.c_str(), new_value.c_str());
    wrefresh(window_);
  }

  TerminalInput prompt()
  {
    int c = wgetch(window_);
    switch (c) {
      case KEY_UP:
        return std::make_pair(KeyPress::kUp, ' ');
      case KEY_DOWN:
        return std::make_pair(KeyPress::kDown, ' ');
      case KEY_BACKSPACE:
      case KEY_DC:
      case 127:
        return std::make_pair(KeyPress::kBackspace, ' ');
      case KEY_ENTER:
      case 10:
        return std::make_pair(KeyPress::kEnter, ' ');
      case 9:
        return std::make_pair(KeyPress::kTab, ' ');
      case 27:
        return std::make_pair(KeyPress::kEscape, ' ');
      default:
        if (c >= 32 && c <= 126) {
          return std::make_pair(KeyPress::kASCII, c);
        } else {
          return std::make_pair(KeyPress::kNone, ' ');
        }
    }
    return std::make_pair(KeyPress::kNone, ' ');
  }

  void quit() { endwin(); }

 private:
  WINDOW *window_;
};
}  // namespace hyped::debug