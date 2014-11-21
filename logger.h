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
void print_log_message(const std::string &log_level, MessageArgs&&... message_args);

template <typename... MessageArgs>
void log_warn(MessageArgs&&... message_args);

template <typename... MessageArgs>
void log_error(MessageArgs&&... message_args);


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
void print_log_message(const std::string &log_level, MessageArgs&&... message_args) {
    print("[", log_level, "] ", std::forward<MessageArgs>(message_args)...);
}

template <typename... MessageArgs>
void log_warn(MessageArgs&&... message_args) {
    print_log_message("WARN", std::forward<MessageArgs>(message_args)...);
}

template <typename... MessageArgs>
void log_error(MessageArgs&&... message_args) {
    print_log_message("ERROR", std::forward<MessageArgs>(message_args)...);
}


#endif // LOGGER_H_INCLUDED
