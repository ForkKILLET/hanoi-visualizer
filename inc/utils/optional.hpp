#pragma once

#include <optional>
namespace optional_ex {

template <typename T>
void try_assign(T& var, std::optional<T>& value) {
    if (value) var = *value;
}
template <typename T>
void try_assign(T& var, std::optional<T>&& value) {
    if (value) var = std::move(*value);
}

};