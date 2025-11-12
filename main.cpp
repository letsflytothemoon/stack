#include <iostream>
#include "testclasses.h"
#include "stack/stack.h"

void F(StackState<A> stack) {
    StackState<B> state = stack.Push<B>(stack.front);
    std::cout << state.front << std::endl;
    state.Pop();
    stack.Pop();
}

int main(int argc, char* argv[]) {
    char buffer[1024];

    StackState<> stack{ buffer };

    StackState<A>    s1 = stack.Push<A>();
    StackState<A, A> s2 = s1.Push<A>(34);

    F(s2.Push<A>(11));

    s2.Pop();
    s1.Pop();
    
    return 0;
}