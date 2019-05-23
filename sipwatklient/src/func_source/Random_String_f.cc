/*
 * Functions.cc
 *
 *  Created on: 4 sty 2018
 *      Author: Krzysztof Œwidrak
 */
#include "Functions.h"

std::string random_string(std::string::size_type length) {
    static auto& chrs = "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg { std::random_device { }() };
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(
            0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while (length--)
        s += chrs[pick(rg)];

    return s;
}
bool checkIsDouble(const char* x) {
    const char* s = x;
    while (*s != '\0') {
        if ((*s == '.' and *(s + 1) == '.') or (*s == ' ' and *(s + 1) == ' ')
                or (*s == '.' and *(s + 1) == ' ')
                or (*s == ' ' and *(s + 1) == '.')) {
            return false;
        }
        if (*s == '1' or *s == '2' or *s == '3' or *s == '4' or *s == '5'
                or *s == '6' or *s == '7' or *s == '8' or *s == '9' or *s == '0'
                or *s == '.' or *s == ' ') {
            s++;
        }
        else{
            return false;
        }
    }
    return true;

}
bool checkIsAddress(const char* x) {
    const char* s = x;
    while (*s != '\0') {
        if ((*s == '.' and *(s + 1) == '.') or (*s == ' ' and *(s + 1) == ' ')
                or (*s == '.' and *(s + 1) == ' ')
                or (*s == ' ' and *(s + 1) == '.')) {
            return false;
        }
        if (isalnum(*s) or *s=='@' or *s==' ') {
            s++;
        }
        else{
            return false;
        }
    }
    return true;

}

