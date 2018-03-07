/*
 * Misc Utilities
 * Zhenyu Wu (Adam_5Wu@hotmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
 */
#ifndef ZWUtils_Units_H
#define ZWUtils_Units_H

#include <pgmspace.h>
#include <WString.h>

enum class TimeUnit : uint64_t {
	USEC = 1,
	MSEC = USEC * 1000,
	SEC = MSEC * 1000,
	MIN = SEC * 60,
	HR = MIN * 60,
	DAY = HR * 24,

	__BEGIN = USEC,
	__END = DAY
};

uint64_t Convert(uint64_t const &Value, TimeUnit const &From, TimeUnit const &To);
uint64_t Convert(uint64_t &Value, TimeUnit const &From, TimeUnit const &To);
PGM_P UnitName(TimeUnit const &Unit, bool const &Abbrv = false);
String ToString(int64_t const &Value, TimeUnit const &DataUnit, bool Abbrv = false, bool OmitPlus = true,
	TimeUnit const &LoUnit = TimeUnit::MSEC, TimeUnit const &HiUnit = TimeUnit::__END);

inline TimeUnit operator++(TimeUnit& x) {
	switch (x) {
		case TimeUnit::USEC: return x = TimeUnit::MSEC;
		case TimeUnit::MSEC: return x = TimeUnit::SEC;
		case TimeUnit::SEC: return x = TimeUnit::MIN;
		case TimeUnit::MIN: return x = TimeUnit::HR;
		case TimeUnit::HR: return x = TimeUnit::DAY;
		case TimeUnit::DAY: return x = TimeUnit::__END;
	}
	return x = TimeUnit::__END;
}
inline TimeUnit operator--(TimeUnit& x) {
	switch (x) {
		case TimeUnit::USEC: return x = TimeUnit::__BEGIN;
		case TimeUnit::MSEC: return x = TimeUnit::USEC;
		case TimeUnit::SEC: return x = TimeUnit::MSEC;
		case TimeUnit::MIN: return x = TimeUnit::SEC;
		case TimeUnit::HR: return x = TimeUnit::MIN;
		case TimeUnit::DAY: return x = TimeUnit::HR;
	}
	return x = TimeUnit::__BEGIN;
}
inline TimeUnit operator*(TimeUnit r) { return r; }
inline TimeUnit begin(TimeUnit r) { return TimeUnit::__BEGIN; }
inline TimeUnit end(TimeUnit r) { return TimeUnit::__END; }

enum class SizeUnit : uint64_t {
	BYTE = 1,
	KB = BYTE * 1024,
	MB = KB * 1024,
	GB = MB * 1024,
	TB = GB * 1024,
	PB = TB * 1024,

	__BEGIN = BYTE,
	__END = PB
};

uint64_t Convert(uint64_t const &Value, SizeUnit const &From, SizeUnit const &To);
uint64_t Convert(uint64_t &Value, SizeUnit const &From, SizeUnit const &To);
PGM_P UnitName(SizeUnit const &Unit, bool const &Abbrv = false);
String ToString(int64_t const &Value, SizeUnit const &DataUnit, bool Abbrv = false, bool OmitPlus = true,
	SizeUnit const &LoUnit = SizeUnit::BYTE, SizeUnit const &HiUnit = SizeUnit::__END);

inline SizeUnit operator++(SizeUnit& x) {
	switch (x) {
		case SizeUnit::BYTE: return x = SizeUnit::KB;
		case SizeUnit::KB: return x = SizeUnit::MB;
		case SizeUnit::MB: return x = SizeUnit::GB;
		case SizeUnit::GB: return x = SizeUnit::TB;
		case SizeUnit::TB: return x = SizeUnit::PB;
		case SizeUnit::PB: return x = SizeUnit::__END;
	}
	return x = SizeUnit::__END;
}
inline SizeUnit operator--(SizeUnit& x) {
	switch (x) {
		case SizeUnit::BYTE: return x = SizeUnit::__BEGIN;
		case SizeUnit::KB: return x = SizeUnit::BYTE;
		case SizeUnit::MB: return x = SizeUnit::KB;
		case SizeUnit::GB: return x = SizeUnit::MB;
		case SizeUnit::TB: return x = SizeUnit::GB;
		case SizeUnit::PB: return x = SizeUnit::TB;
	}
	return x = SizeUnit::__BEGIN;
}
inline SizeUnit operator*(SizeUnit r) { return r; }
inline SizeUnit begin(SizeUnit r) { return SizeUnit::__BEGIN; }
inline SizeUnit end(SizeUnit r) { return SizeUnit::__END; }

#endif