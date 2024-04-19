#include "ncurses_string.hpp"

ncurses_string::ncurses_string(const char* str) : _str(copy(str)) {}

ncurses_string::ncurses_string(const std::string& str) : _str(copy(str)) {}

ncurses_string::~ncurses_string()
{
    delete[] _str;
    _str = nullptr;
}

ncurses_string::ncurses_string(const ncurses_string& other) : ncurses_string(other._str) {}

ncurses_string& ncurses_string::operator=(const ncurses_string& lhs)
{
    delete[] _str;
    _str = copy(lhs);
    return *this;
}

ncurses_string::ncurses_string(ncurses_string&& other) : _str(other._str) { other._str = nullptr; }

ncurses_string& ncurses_string::operator=(ncurses_string&& lhs)
{
    if(this != &lhs) {
        delete[] _str;
        _str = lhs._str;
        lhs._str = nullptr;
    }
    return *this;
}

std::size_t ncurses_string::len() const { return std::strlen(_str); }

bool ncurses_string::empty() const { return len() == 0; }

char* ncurses_string::str() const { return _str; }

char* ncurses_string::copy(const char* src)
{
    std::size_t len(std::strlen(src));
    char* dest = new char[len + 1];
    std::strcpy(dest, src);
    dest[len] = '\0';
    return dest;
}

char* ncurses_string::copy(const std::string& src)
{
    std::size_t len(src.length());
    char* dest = new char[len + 1];
    std::strcpy(dest, src.c_str());
    dest[len] = '\0';
    return dest;
}

char* ncurses_string::copy(const ncurses_string& str) { return copy(str.str()); }
