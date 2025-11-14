#pragma once

/*
	Заглушки эмулираующие какой-то сетевой уровень
*/

#include <thread>
#include <vector>

int sockscounter = 100;

int socket() { return sockscounter++; }
void connect(int sock, const char* address) {

}

bool checkSocketConnected(int sock) {
	return true;
}