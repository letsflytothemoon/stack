#pragma once
#include <functional>
#include "net.h"
#include "stack.h"

template <unsigned int Size>
struct RingBuffer
{
	char bytes[Size];
};

struct State
{
	enum class UpdateResult {
		ok,
		done,
		error
	};

	virtual UpdateResult Update() = 0;
	virtual ~State() {}
};

class Connecting : public State
{
	int sock;
public:
	Connecting(const char* address, int sock) : sock{ sock } {
		::connect(sock, address);
	}

	UpdateResult Update() override {
		try {
			if (::checkSocketConnected(sock))
				return UpdateResult::done;
			else
				return UpdateResult::ok;
		}
		catch (...) {
			return UpdateResult::error;
		}
	}
};

template <unsigned int BufferSize>
class Connected : public State
{
	int& sock;
	RingBuffer<BufferSize>& buffer;
public:
	Connected(stack::StaticStack<RingBuffer<BufferSize>, int> stack) :
		sock{ stack.at<1>()},
		buffer{ stack.front } {
	}

	UpdateResult Update() override {
		return UpdateResult::ok;
	}

	~Connected() { }
};


