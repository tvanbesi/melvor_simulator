#pragma once

#include <chrono>
#include <form.h>
#include <fstream>
#include <iostream>
#include <menu.h>
#include <ncurses.h>
#include <string>

const std::string error_log_filename = "error.log";

// For noexcept functions like destructors. Only for <form.h> and <menu.h> errors.
inline void log_ncurses_error(const std::string& function_name, const int err_code) noexcept
{
    std::ofstream error_log_file(error_log_filename, std::ofstream::app);
    if(!error_log_file.is_open()) {
        std::cerr << "Could not open " << error_log_filename << '\n';
        return;
    }

    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

    error_log_file << std::ctime(&t_c) << "Error in: " << function_name
                   << "(), error code: " << err_code << ' ';
    switch(err_code) {
        // clang-format off
        case E_OK: error_log_file << "E_OK"; break;
        case E_SYSTEM_ERROR: error_log_file << "E_SYSTEM_ERROR"; break;
        case E_BAD_ARGUMENT: error_log_file << "E_BAD_ARGUMENT"; break;
        case E_POSTED: error_log_file << "E_POSTED"; break;
        case E_CONNECTED: error_log_file << "E_CONNECTED"; break;
        case E_BAD_STATE: error_log_file << "E_BAD_STATE"; break;
        case E_NO_ROOM: error_log_file << "E_NO_ROOM"; break;
        case E_NOT_POSTED: error_log_file << "E_NOT_POSTED"; break;
        case E_UNKNOWN_COMMAND: error_log_file << "E_UNKNOWN_COMMAND"; break;
        case E_NO_MATCH: error_log_file << "E_NO_MATCH"; break;
        case E_NOT_SELECTABLE: error_log_file << "E_NOT_SELECTABLE"; break;
        case E_NOT_CONNECTED: error_log_file << "E_NOT_CONNECTED"; break;
        case E_REQUEST_DENIED: error_log_file << "E_REQUEST_DENIED"; break;
        case E_INVALID_FIELD: error_log_file << "E_INVALID_FIELD"; break;
        case E_CURRENT: error_log_file << "E_CURRENT"; break;
        // clang-format on
    }
    error_log_file << '\n';
}