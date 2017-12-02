#pragma once

template <typename FlagsType>
class Flags
{
public:
	Flags() = default;
	Flags(const Flags &b) = default;
	explicit constexpr Flags(unsigned value) : underlying(value) {}

	operator bool() const
	{
		return underlying != 0;
	}

	bool operator! () const
	{
		return underlying == 0;
	}

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

private:
	unsigned underlying = 0;
};
