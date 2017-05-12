/*
 * Misc Utilities
 * Zhenyu Wu (Adam_5Wu@hotmail.com)
 *
 * Inspired by ESPAsyncWebServer project by Hristo Gochkov.
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
#ifndef Misc_H_
#define Misc_H_

#include "WString.h"

#define ESPZW_LOG(...) Serial.printf(__VA_ARGS__)
#define ESPZW_DEBUG_LEVEL 3

#if ESPZW_DEBUG_LEVEL < 1
	#define ESPZW_DEBUGDO(...)
	#define ESPZW_DEBUG(...)
#else
	#define ESPZW_DEBUGDO(...) __VA_ARGS__
	#define ESPZW_DEBUG(...) Serial.printf(__VA_ARGS__)
#endif

#if ESPZW_DEBUG_LEVEL < 2
	#define ESPZW_DEBUGVDO(...)
	#define ESPZW_DEBUGV(...)
#else
	#define ESPZW_DEBUGVDO(...) __VA_ARGS__
	#define ESPZW_DEBUGV(...) Serial.printf(__VA_ARGS__)
#endif

#if ESPZW_DEBUG_LEVEL < 3
	#define ESPZW_DEBUGVVDO(...)
	#define ESPZW_DEBUGVV(...)
#else
	#define ESPZW_DEBUGVVDO(...) __VA_ARGS__
	#define ESPZW_DEBUGVV(...) Serial.printf(__VA_ARGS__)
#endif

extern String const EMPTY_STRING;
extern char const HexLookup_UC[];
extern char const HexLookup_LC[];

// out is a 16 byte buffer
void calcMD5(uint8_t* data, uint16_t len, uint8_t *out);
// text is a 32 char buffer, Lookup is a mapping of hex numbers
void textMD5(uint8_t* data, uint16_t len, char *text, char const *Lookup);

inline void textMD5_UC(uint8_t* data, uint16_t len, char *text)
{ textMD5(data,len,text,HexLookup_UC); }
inline void textMD5_LC(uint8_t* data, uint16_t len, char *text)
{ textMD5(data,len,text,HexLookup_LC); }

String getQuotedToken(char const *&ptr, char const delim = ';');
void putQuotedToken(String const &token, String &out, char const delim = ';', bool delimPfx = true, bool forceQuote = false);

#endif