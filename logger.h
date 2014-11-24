#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <string>
#include <utility>


static auto &log_stream = std::cerr;


template <typename Arg>
void print(Arg &&arg);

template <typename FirstArg, typename... Args>
void print(FirstArg &&first_arg, Args&&... args);

template <typename... MessageArgs>
void print_log_message(const std::string &log_level, const std::string &formatting, MessageArgs&&... message_args);

template <typename... MessageArgs>
void log_info(MessageArgs&&... message_args);

template <typename... MessageArgs>
void log_warn(MessageArgs&&... message_args);

template <typename... MessageArgs>
void log_error(MessageArgs&&... message_args);

template <typename... MessageArgs>
void log_fatal(MessageArgs&&... message_args);


template <typename Arg>
void print(Arg &&arg) {
    log_stream << arg << std::endl;
}

template <typename FirstArg, typename... Args>
void print(FirstArg &&first_arg, Args&&... args) {
    log_stream << first_arg;
    print(std::forward<Args>(args)...);
}

template <typename... MessageArgs>
void print_log_message(const std::string &log_level, const std::string &formatting, MessageArgs&&... message_args) {
    print("\033[", formatting, ";1m[", log_level, "]\033[0m\033[", formatting, "m ", std::forward<MessageArgs>(message_args)..., "\033[0m");
}

template <typename... MessageArgs>
void log_info(MessageArgs&&... message_args) {
    print_log_message("INFO", "39", std::forward<MessageArgs>(message_args)...);
}

template <typename... MessageArgs>
void log_warn(MessageArgs&&... message_args) {
    print_log_message("WARN", "36", std::forward<MessageArgs>(message_args)...);
}

template <typename... MessageArgs>
void log_error(MessageArgs&&... message_args) {
    print_log_message("ERROR", "31", std::forward<MessageArgs>(message_args)...);
}

template <typename... MessageArgs>
void log_fatal(MessageArgs&&... message_args) {
    print_log_message("FATAL", "35", std::forward<MessageArgs>(message_args)...);
}


#endif // LOGGER_H_INCLUDED
