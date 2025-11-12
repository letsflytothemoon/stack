#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>

class A
{
public:
    int value;
    A() : value(0) { std::cout << "A::A()"         << std::endl; }
    A(const A& other) : value(other.value)
    { std::cout << "A::A(const A&)" << std::endl; }
    A(A&& other) noexcept : value(other.value)
    {
        other.value = 0;
        std::cout << "A::A(A&&)"      << std::endl;
    }
    ~A() { std::cout << "A::~A() //value = " << value        << std::endl; }
    A(int value) : value(value)
    { std::cout << "A::A(" << value << ")"      << std::endl; }
    A& operator=(const A& other)
    {
        std::cout << "A::operator=(const A&)" << std::endl;
        value = other.value;
        return *this;
    }
    A& operator=(A&& other) noexcept
    {
        std::cout << "A::operator=(A&&)" << std::endl;
        value = other.value;
        other.value = 0;
        return *this;
    }
    A& operator=(int value)
    {
        std::cout << "A::operator=(int)" << std::endl;
        this->value = value;
        return *this;
    }
    operator int() const
    {
        std::cout << "A::operator int()" << std::endl;
        return value;
    }
};

class B
{
public:
    int value;
    B() : value(0) { std::cout << "B::B()"         << std::endl; }
    B(const B& other) : value(other.value)
    { std::cout << "B::B(const B&)" << std::endl; }
    B(B&& other) noexcept : value(other.value)
    {
        other.value = 0;
        std::cout << "B::B(B&&)"      << std::endl;
    }
    ~B() { std::cout << "B::~B() //value = " << value        << std::endl; }
    B(int value) : value(value)
    { std::cout << "B::B(" << value << ")"      << std::endl; }
    B& operator=(const B& other)
    {
        std::cout << "B::operator=(const B&)" << std::endl;
        value = other.value;
        return *this;
    }
    B& operator=(B&& other) noexcept
    {
        std::cout << "B::operator=(B&&)" << std::endl;
        value = other.value;
        other.value = 0;
        return *this;
    }
    B& operator=(int value)
    {
        std::cout << "B::operator=(int)" << std::endl;
        this->value = value;
        return *this;
    }
    operator int() const
    {
        std::cout << "B::operator int()" << std::endl;
        return value;
    }
    B(const A& a)
    {
        std::cout << "B::A(const A&) //value = " << a.value << std::endl;
        value = a.value;
    }
};

template <class Stream> Stream& operator<<(Stream& stream, const A& a) { return stream << a.value; }
template <class Stream> Stream& operator<<(Stream& stream, const B& b) { return stream << b.value; }

template <class T>
struct Info
{
    template <class S>
    S& Out(S& s) const { return s << "unknown"; }
};

template <class T, class Stream>
Stream& operator<<(Stream& stream, const Info<T>& info)
{ return info.Out(stream); }

template <> struct Info<bool>
{
    template <class S>
    S& Out(S& s) const { return s << "bool"; }
};

template <> struct Info<char>
{
    template <class S>
    S& Out(S& s) const { return s << "char"; }
};

template <> struct Info<short>
{
    template <class S>
    S& Out(S& s) const { return s << "short"; }
};

template <> struct Info<int>
{
    template <class S>
    S& Out(S& s) const { return s << "int"; }
};

template <> struct Info<long>
{
    template <class S>
    S& Out(S& s) const { return s << "long"; }
};

template <> struct Info<long long>
{
    template <class S>
    S& Out(S& s) const { return s << "long long"; }
};

template <> struct Info<float>
{
    template <class S>
    S& Out(S& s) const { return s << "float"; }
};

template <> struct Info<double>
{
    template <class S>
    S& Out(S& s) const { return s << "double"; }
};

template <> struct Info<A>
{
    template <class S>
    S& Out(S& s) const { return s << "A"; }
};

template <> struct Info<B>
{
    template <class S>
    S& Out(S& s) const { return s << "B"; }
};

template <class T> struct Info<T const>
{
    template <class S>
    S& Out(S& s) const { return s << Info<T>() << " const"; }
};

template <class T> struct Info<T volatile>
{
    template <class S>
    S& Out(S& s) const { return s << Info<T>() << " volatile"; }
};

template <class T> struct Info<T*>
{
    template <class S>
    S& Out(S& s) const { return s << Info<T>() << "*"; }
};

template <class T> struct Info<T&>
{
    template <class S>
    S& Out(S& s) const { return s << Info<T>() << "&"; }
};

template <class T> struct Info<T&&>
{
    template <class S>
    S& Out(S& s) const { return s << Info<T>() << "&&"; }
};

template <class T, unsigned int Sz>
struct Info<T[Sz]>
{
    template <class S>
    S& Out(S& s) const { return s << Info<T>() << "[" << Sz << "]"; }
};

template <class T>
struct Info<T[]>
{
    template <class S>
    S& operator <<(S& s) const { return s << Info<T>() << "[]"; }
};

template <class T>
struct Info<std::vector<T>>
{
    template <class S>
    S& Out(S& s) const { return s << "std::vector<" << Info<T>() << ">"; }
};

template <class T>
struct Info<std::list<T>>
{
    template <class S>
    S& Out(S& s) const { return s << "std::list<" << Info<T>() << ">" << std::endl; }
};