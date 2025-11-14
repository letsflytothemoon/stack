#include <iostream>
#include "stack.h"
#include "testclasses.h"
#include "states.h"

int main(int argc, char* argv[]) {
    char stateBuffer[4096];
    char contextBuffer[4096];

    auto contextStack = stack::StaticStack<> { contextBuffer }
    .Push<int>(::socket);

    stack::DynamicStack<State> stateStack{ stateBuffer };
    

    
    stateStack.Push<Connecting>("127.0.0.1", contextStack.front);

    State::UpdateResult result;
    while ((result = stateStack.Top().Update()) == State::UpdateResult::ok);
    if (result == State::UpdateResult::error) throw ":-(";
    std::cout << "connected" << std::endl;
    stateStack.Pop();
    auto connectedContextStack = contextStack.Push<RingBuffer<1024>>();
    stateStack.Push<Connected<1024>>(connectedContextStack);
    while ((result = stateStack.Top().Update()) == State::UpdateResult::ok);
    if (result == State::UpdateResult::error) throw ":-(";
    stateStack.Pop();

    contextStack.Pop(); //socket


    return 0;
}