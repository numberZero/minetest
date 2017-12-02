#pragma once

template <typename FlagsType>
class Flags
{
public:
	Flags() = default;
	Flags(const Flags &b) = default;

	Flags operator~ () const
	{
		return {~underlying};
	}

	Flags operator& (const Flags &b) const
	{
		return {underlying & b.underlying};
	}

	Flags operator| (const Flags &b) const
	{
		return {underlying | b.underlying};
	}

	Flags &operator&= (const Flags &b)
	{
		underlying &= b.underlying;
		return *this;
	}

	Flags &operator|= (const Flags &b)
	{
		underlying |= b.underlying;
		return *this;
	}

// protected:
	explicit constexpr Flags(unsigned value) : underlying(value) {}

private:
	unsigned underlying = 0;
};
