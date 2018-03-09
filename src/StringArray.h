/*
	Copyright (c) 2016 Hristo Gochkov. All rights reserved.
	Modified by Zhenyu Wu <Adam_5Wu@hotmail.com> @ 2017.01

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/
#ifndef StringArray_H_
#define StringArray_H_

#include "WString.h"
#include "LinkedList.h"

class StringArray : public LinkedList<String> {
	public:
		StringArray(void) : LinkedList(nullptr) {}

		bool contains(String const &s, bool ignoreCase = false) const {
			return get_if([&](String const &v) {
				return ignoreCase? v.equalsIgnoreCase(s) : v == s;
			}) != nullptr;
		}

		bool contains(const char *cstr, bool ignoreCase = false) const {
			return get_if([&](String const &v) {
				return ignoreCase? v.equalsIgnoreCase(cstr) : v == cstr;
			}) != nullptr;
		}
	
		bool contains(const __FlashStringHelper *str,
			bool ignoreCase = false) const {
			return get_if([&](String const &v) {
				return ignoreCase? v.equalsIgnoreCase(str) : v == str;
			}) != nullptr;
		}
	
		bool containsIgnoreCase(String const &s) const
		{ return contains(s, true); }
		
		bool containsIgnoreCase(const char *cstr) const
		{ return contains(cstr, true); }

		bool containsIgnoreCase(const __FlashStringHelper *str) const
		{ return contains(str, true); }
		
};

#endif /* StringArray_H_ */
