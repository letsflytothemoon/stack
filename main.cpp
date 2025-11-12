#include <iostream>
#include "testclasses.h"
#include "stack.h"



void F(StackState<> stack) {
    StackState<B> state = stack.Push<B>();
    std::cout << state.front << std::endl;
    state.Pop();
}

int main(int argc, char* argv[]) {
    char buffer[4096];

    StackState<> stack{ buffer };

    StackState<A>    s1 = stack.Push(A{ 99 });
    StackState<A, A> s2 = s1.Push(A{ 34 });

    F(s2);

    s2.Pop();
    s1.Pop();
    
    return 0;
}