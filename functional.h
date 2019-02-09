#pragma once

namespace DLib
{
	template<typename T>
	class less
	{
	public:
		bool operator()(const T& r, const T& l)
		{
			return r < l;
		}
	};



}