#pragma once
#include <utility>

namespace stack
{
    template <class ... Ts>
    class StaticStack
    {
        template <class ... NoMatters>
        friend class StackState;
        void* _top;
    public:
        StaticStack(void* _top) : _top{ _top } {}

        template <class ... OtherTs>
        StaticStack(const StaticStack<OtherTs ...>& other) : _top{ other._top } {}

        template <class T, class ... Args>
        inline StaticStack<T, Ts ...> Push(Args&& ... args) const {
            T& ref = *new (_top) T{ std::forward<Args>(args) ... };
            return { *this, (char*)_top + sizeof(T), ref };
        }

        template <class T>
        inline StaticStack<T, Ts ...> Push(const T& value) const {
            T& ref = *new (_top) T{ value };
            return { *this, (char*)_top + sizeof(T), ref };
        }

        template <class T>
        inline StaticStack<T, Ts ...> Push(T&& value) const {
            T& ref = *new (_top) T{ std::move(value) };
            return { *this, (char*)_top + sizeof(T), ref };
        }

        void Clear() const { }
    };

    template <unsigned int Position>
    struct Extract
    {
        template <class S, class ... STs>
        static auto& get(const StaticStack<S, STs ...>& inst) {
            return Extract<Position - 1>::get((const StaticStack<STs ...>&)inst);
        }
    };

    template<>
    struct Extract<0>
    {
        template <class S, class ... STs>
        static S& get(const StaticStack<S, STs ...>& inst) {
            return inst.top;
        }
    };

    template <class T, class ... Ts>
    class StaticStack<T, Ts ...> : StaticStack<Ts ...>
    {
        template <class ... NoMatters>
        friend class StaticStack;
        void* _top;

        template <class ... Args>
        StaticStack(const StaticStack<Ts ...>& prev, void* _top, T& front) :
            StaticStack<Ts ...>{ prev },
            _top{ _top },
            top{ top } { }
    public:
        T& top;

        template <class U, class ... Args>
        inline StaticStack<U, T, Ts ...> Push(Args&& ... args) const {
            U& ref = *new (_top) U{ std::forward<Args>(args) ... };
            return { *this, (char*)_top + sizeof(U), ref };
        }

        template <class U>
        inline StaticStack<U, T, Ts ...> Push(const U& value) const {
            U& ref = *new (_top) U{ value };
            return { *this, (char*)_top + sizeof(U), ref };
        }

        template <class U>
        inline StaticStack<U, T, Ts ...> Push(U&& value) const {
            U& ref = *new (_top) U{ std::move(value) };
            return { *this, (char*)_top + sizeof(U), ref };
        }

        inline const StaticStack<Ts ...>& Pop() const {
            (&top)->~T();
            return (StaticStack<Ts ...>&) *this;
        }

        template <class U, class ... Nexts>
        StaticStack(const StaticStack<U, Nexts ...>& other) :
            StaticStack<Ts ...>{ (const StaticStack<Nexts ...>&)other },
            _top{ other._top },
            top{ other.top } { }

        template <unsigned int Position>
        auto& at() const {
            return Extract<Position>::get(*this);
        }

        void Clear() const {
            //todo
        }
    };

    template <class T>
    class DynamicStack
    {
        struct Node { Node* prev; } *top;
    public:
        DynamicStack(void* buffer) : top{ (Node*)buffer } {
            new (top) Node{ nullptr };
        }

        template <class U, class ... Args>
        U& Push(Args&& ... args) {
            T* ptr = new (top + 1) U{ std::forward<Args>(args)... };
            top = new ((Node*)((U*)(top + 1) + 1)) Node{ top };
            return *((U*)ptr);
        }

        void Pop() {
            ((T*)(top->prev + 1))->~T();
            top = top->prev;
        }

        T& Top() {
            return *((T*)(top->prev + 1));
        }

        T& At(size_t index) {
            Node* cursor = top->prev;
            while (index--)
                cursor = cursor->prev;
            return *((T*)(cursor + 1));
        }

        void Clear() {
            //todo
        }
    };
}