#pragma once

template <class ... Ts>
class StackState
{
    template <class ... NoMatters>
    friend class StackState;
    void* top;
public:
    StackState(void* top) : top{ top } { }

    template <class T, class ... Args>
    inline StackState<T, Ts ...> Push(Args&& ... args) const {
        T& ref = *new (top) T{std::forward<Args>(args) ...};
        return { *this, (char*)top + sizeof(T), ref };
    }
};

template <class T, class ... Ts>
class StackState<T, Ts ...> : StackState<Ts ...>
{
    template <class ... NoMatters>
    friend class StackState;
    void* top;
public:
    T& front;

    template <class U, class ... Args>
    inline StackState<U, T, Ts ...> Push(Args&& ... args) const {
        U& ref = *new (top) U{std::forward<Args>(args) ...};
        return { *this, (char*)top + sizeof(U), ref };
    }

    inline const StackState<Ts ...>& Pop() const {
        (&front)->~T();
        return (StackState<Ts ...>&)*this;
    }

    template <class ... Args>
    StackState(const StackState<Ts ...>& prev, void* top, T& front)  :
    StackState<Ts ...> { prev },
    top { top },
    front { front } { }

    template <class U, class ... Nexts>
    StackState(const StackState<U, Nexts ...>& other) :
    StackState<Ts ...> { (const StackState<Nexts ...>&)other },
    top{ other.top },
    front { other.front }
    { }
};

