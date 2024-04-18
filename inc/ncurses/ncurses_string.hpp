#pragma once

#include <cstring>
#include <memory>
#include <string>

class ncurses_string {
  public:
    ncurses_string(const char* str);
    ncurses_string(const std::string& str);
    ~ncurses_string();
    ncurses_string(const ncurses_string& other);
    ncurses_string& operator=(const ncurses_string& lhs);
    ncurses_string(ncurses_string&& other);
    ncurses_string& operator=(ncurses_string&& lhs);

    std::size_t len() const;
    char* str() const;

  private:
    char* _str;

    static char* copy(const char* str);
    static char* copy(const std::string& str);
    static char* copy(const ncurses_string& str);
};