#pragma once
#include "../Utils.h"

namespace CWeather
{
	enum eWeatherType : uint32_t
	{
		EXTRASUNNY,
		SUNNY,
		SUNNY_WINDY,
		CLOUDY,
		RAIN,
		DRIZZLE,
		FOGGY,
		LIGHTNING
	};

	static eWeatherType GetCurrentWeather()
	{
		static eWeatherType *currentWeather = *(eWeatherType**)Utils::ReadMemory(0x74E6A7);
		return *currentWeather;
	}

	static eWeatherType GetNextWeather()
	{
		static eWeatherType *nextWeather = *(eWeatherType**)Utils::ReadMemory(0x74E6B3);
		return *nextWeather;
	}

	static float GetNextWeatherPercentage()
	{
		static float *nextWeatherPctg = *(float**)Utils::ReadMemory(0x74E69B);
		return *nextWeatherPctg;
	}
}