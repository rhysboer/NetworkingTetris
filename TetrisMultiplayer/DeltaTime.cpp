#include "DeltaTime.h"

float DeltaTime::time;

DeltaTime::DeltaTime() {
}

DeltaTime::~DeltaTime() {
}

void DeltaTime::UpdateTime(float dT) {
	time = dT;
}

float DeltaTime::Time() {
	return time;
}
