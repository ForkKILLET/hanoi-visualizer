#pragma once
#include <concepts>
#include <cstdint>
#include <functional>
#include <memory>
#include <set>

template <typename Ret, typename... Args>
class UniqueFunction {
public:
    using Fn = std::function<Ret (Args...)>;

    template <typename F>
    requires std::convertible_to<F, Fn>
    UniqueFunction(F func) :
        func(Fn(func)),
        token(UniqueFunction<Ret, Args...>::next_token ++)
    {}

    const uint32_t token;
    const Fn func;

    auto operator<=>(const UniqueFunction& other) const {
        return token <=> other.token;
    }
    bool operator==(const UniqueFunction& other) const = default;

    Ret operator()(Args... args) const {
        return func(args...);
    }

private:
    static uint32_t next_token;
};

template <typename Ret, typename... Args>
uint32_t UniqueFunction<Ret, Args...>::next_token = 0;

template <typename... Args>
class Delegate {
public:
    using Fn = std::function<void (Args...)>;
    using UFn = UniqueFunction<void, Args...>;
    using Self = Delegate<Args...>;

    Delegate() = default;
    Delegate(UFn func) {
        functions->insert(func);
    }

    Self& operator+=(UFn func) {
        functions->insert(func);
        return *this;
    }
    Self& operator-=(UFn func) {
        functions->erase(func);
        return *this;
    }

    Self& operator+=(const Self& other) {
        functions->insert(other.functions->begin(), other.functions->end());
        return *this;
    }
    Self& operator-=(const Self& other) {
        for (const auto& func : *other.functions) {
            functions->erase(func);
        }
        return *this;
    }

    void operator()(Args... args) const {
        for (auto& func : *functions) {
            func(args...);
        }
    }

private:
    std::shared_ptr<std::set<UFn>> functions = std::make_shared<std::set<UFn>>();
};