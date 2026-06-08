#pragma once

#include <string>
#include <sstream>
#include <iomanip>

namespace Strings
{
	inline std::string Format(float value, int decimals = 1)
	{
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(decimals) << value;
		return ss.str();
	}

	inline std::string Pad(const std::string& label, const std::string& value, int width = 20)
	{
		std::string line = label;
		while ((int)line.size() < width) line += ' ';
		return line + value;
	}
}