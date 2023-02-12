#pragma once

#include <intrin.h>
#include <corecrt_math_defines.h>"

template<typename T>
inline T Clamp(const T val, const T min, const T max) {
	const T t = (val < min) ? min : val;
	return (t > max) ? max : t;
}

template<typename T>
inline T Min(const T a, const T b) {
	return ((a > b) * b) + ((a <= b) * a);
}

template<typename T>
inline T Max(const T a, const T b) {
	return ((a > b) * a) + ((a <= b) * b);
}

Vector NormalizeAngle(Vector angles)
{
	if (angles.x > 89)
	{
		angles.x = 89;
	}
	else if (-89 > angles.x)
	{
		angles.x = -89;
	}

	if (angles.y > 180)
	{
		angles.y -= 360;
	}
	else if (-180 > angles.y)
	{
		angles.y += 360;
	}

	angles.z = 0;

	return angles;
}

ImColor GetHealthColor(const int nHealth, const int nMaxHealth)
{
	if (nHealth > nMaxHealth)
		return (0u, 255u, 0u, 255u);

	const int nCurHP = Max(0, Min(nHealth, nMaxHealth));

	return {
		Min((510 * (nMaxHealth - nCurHP)) / nMaxHealth, 200),
		Min((510 * nCurHP) / nMaxHealth, 200),
		0,
		255
	};
}