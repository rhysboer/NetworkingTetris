#pragma once
class DeltaTime {
public:
	DeltaTime();
	~DeltaTime();

	static void UpdateTime(float dT);
	static float Time();

private:
	static float time;
};

