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
#include "Units.h"

uint64_t _Convert(uint64_t &Value, uint64_t const &FromBase, uint64_t const &ToBase) {
	uint64_t Factor;
	uint64_t Rem;
	if (FromBase >= ToBase) {
		Factor = FromBase / ToBase;
		Rem = 0;
		Value *= Factor;
	} else {
		Factor = ToBase / FromBase;
		Rem = Value % Factor;
		Value /= Factor;
	}
	return Rem;
}

uint64_t Convert(uint64_t &Value, TimeUnit const &From, TimeUnit const &To) {
	return _Convert(Value, (uint64_t) From, (uint64_t) To);
}

uint64_t Convert(uint64_t const &Value, TimeUnit const &From, TimeUnit const &To) {
	uint64_t iValue = Value;
	return _Convert(iValue, (uint64_t) From, (uint64_t) To), iValue;
}

uint64_t Convert(uint64_t &Value, SizeUnit const &From, SizeUnit const &To) {
	return _Convert(Value, (uint64_t) From, (uint64_t) To);
}

uint64_t Convert(uint64_t const &Value, SizeUnit const &From, SizeUnit const &To) {
	uint64_t iValue = Value;
	return _Convert(iValue, (uint64_t) From, (uint64_t) To), iValue;
}

PGM_P UnitName(TimeUnit const &Unit, bool const &Abbrv) {
	switch (Unit) {
		case TimeUnit::USEC: return Abbrv ? PSTR("us") : PSTR("microsecond");
		case TimeUnit::MSEC: return Abbrv ? PSTR("ms") : PSTR("millisecond");
		case TimeUnit::SEC: return Abbrv ? PSTR("s") : PSTR("second");
		case TimeUnit::MIN: return Abbrv ? PSTR("m") : PSTR("minute");
		case TimeUnit::HR: return Abbrv ? PSTR("h") : PSTR("hour");
		case TimeUnit::DAY: return Abbrv ? PSTR("d") : PSTR("day");
		default: return Abbrv ? PSTR("?") : PSTR("(unknown time-unit)");
	}
}

PGM_P UnitName(SizeUnit const &Unit, bool const &Abbrv) {
	switch (Unit) {
		case SizeUnit::BYTE: return Abbrv ? PSTR("B") : PSTR("byte");
		case SizeUnit::KB: return Abbrv ? PSTR("KB") : PSTR("kilobyte");
		case SizeUnit::MB: return Abbrv ? PSTR("MB") : PSTR("megabyte");
		case SizeUnit::GB: return Abbrv ? PSTR("GB") : PSTR("gigabyte");
		case SizeUnit::TB: return Abbrv ? PSTR("TB") : PSTR("terabyte");
		case SizeUnit::PB: return Abbrv ? PSTR("PB") : PSTR("petabyte");
		default: return Abbrv ? PSTR("?") : PSTR("(unknown size-unit)");
	}
}

template <class T>
String _ToString(int64_t const &Value, T const &DataUnit, bool Abbrv, bool OmitPlus,
	T const &HiUnit, T const &LoUnit) {
	String Ret;

	uint64_t Rem = abs(Value);
	uint64_t RValue = 0;
	auto CurRes = (HiUnit > LoUnit) ? HiUnit : LoUnit;
	while (CurRes >= LoUnit) {
		if ((RValue = Rem) == 0) {
			if (!Ret.empty()) break;
		}
		Rem = Convert(RValue, DataUnit, CurRes);
		if ((RValue != 0) || (CurRes <= LoUnit)) {
			if (Ret.empty()) {
				if (Value > 0 && !OmitPlus) Ret.concat('+');
				else if (Value < 0) Ret.concat('-');
			} else Ret.concat(' ');

			Ret.concat((unsigned long)RValue);
			if (!Abbrv) Ret.concat(' ');
			Ret.concat(FPSTR(UnitName(CurRes, Abbrv)));
			if (!Abbrv && (RValue > 1)) Ret.concat('s');
		}
		--CurRes;
	}
	if (Rem) Ret.concat('~');
	return Ret;
}

String ToString(int64_t const &Value, TimeUnit const &DataUnit, bool Abbrv,
	bool OmitPlus, TimeUnit const &LoUnit, TimeUnit const &HiUnit) {
	return _ToString(Value, DataUnit, Abbrv, OmitPlus, HiUnit, LoUnit);
}

String ToString(int64_t const &Value, SizeUnit const &DataUnit, bool Abbrv,
	bool OmitPlus, SizeUnit const &LoUnit, SizeUnit const &HiUnit) {
	return _ToString(Value, DataUnit, Abbrv, OmitPlus, HiUnit, LoUnit);
}
