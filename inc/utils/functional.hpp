#pragma once
#include <functional>
#include <type_traits>

namespace fp {

using Condition = std::function<bool ()>;

template <auto value>
auto constant = [] () { return value; };

inline Condition f_not(Condition cond) {
    return [=] { return ! cond(); };
}

template <typename... Args>
requires (std::is_same_v<Args, Condition> && ...)
inline Condition f_and(Args... conds) {
    return [=] { return (conds() && ...); };
}

template <typename... Args>
requires (std::is_same_v<Args, Condition> && ...)
inline Condition f_or(Args... conds) {
    return [=] { return (conds() || ...); };
}

};