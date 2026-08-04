//
// Created by jude2k6 on 8/2/26.
//
#include <expected>
#include <print>
#include <string>
#include <stdexcept>


enum class ParseError { empty, invalid_character, out_of_range, trailing_characters };

std::expected<int, ParseError> parseIntValue(std::string_view s) {
    int num{};
    for (auto i{0uz}; i < s.length(); i++) {
        char byte = s[i];
        if (byte > '9' || byte < '0') { return std::unexpected(ParseError::empty); }
        int temp_num = byte - '0';
        num = num * 10 + temp_num;
    }
    return num;
}

int parseIntException(std::string_view s) {

    int num{};
    for (auto i{0uz}; i < s.length(); i++) {
        char byte = s[i];
        if (byte > '9' || byte < '0') { throw std::exception(); }
        int temp_num = byte - '0';
        num = num * 10 + temp_num;
    }
    return num;
}


int main() {
    std::string s = "1a";

    try {
        auto num1 = parseIntException(s);
        std::print("num1: {}\n", num1);
    } catch (...) {
        std::print("error exception caught\n");
    }

    if (auto num2 = parseIntValue(s)) {
        std::print("num2: {}\n", *num2);
    } else {
        std::print("error");
    }

}