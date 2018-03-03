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

#ifndef ESPZW_LOG_TIME
#define ESPZW_LOG_TIME 1
#endif

#include "WString.h"
#include <pgmspace.h>
#include <sys/time.h>

#define PROGMEM_C  __attribute__((section(".irom.text.log.common")))
#define PROGMEM_T  __attribute__((section(".irom.text.log.template")))
#define PROGMEM_L  __attribute__((section(".irom.text.log.local")))

#ifndef ESPZW_LOG
#if ESPZW_LOG_TIME
#define ESPZW_LOG_X(spfx, fmt, ...)															\
{ 																							\
	static const char pfmt[] PROGMEM ## spfx = "%5d.%0.3d | " fmt;							\
	timeval time; gettimeofday(&time, NULL);												\
	Serial.printf_P(pfmt, (time.tv_sec % 100000), (time.tv_usec / 1000), ## __VA_ARGS__);	\
}
#else
#define ESPZW_LOG_X(spfx, fmt, ...)					\
{ 													\
	static const char pfmt[] PROGMEM ## spfx = fmt;	\
	Serial.printf_P(pfmt, ## __VA_ARGS__);			\
}
#endif
#define ESPZW_LOG_S(sect, fmt, ...)	ESPZW_LOG_X(_ ## sect,fmt, ## __VA_ARGS__)
#define ESPZW_LOG(fmt, ...) ESPZW_LOG_S(C,fmt, ## __VA_ARGS__)
#endif

#ifndef ESPZW_DEBUG_LEVEL
#define ESPZW_DEBUG_LEVEL 0
#endif

#if ESPZW_DEBUG_LEVEL < 1
	#define ESPZW_DEBUGDO(...)
	#define ESPZW_DEBUG(...)
	#define ESPZW_DEBUG_S(...)
#else
	#define ESPZW_DEBUGDO(...) __VA_ARGS__
	#define ESPZW_DEBUG(...) ESPZW_LOG(__VA_ARGS__)
	#define ESPZW_DEBUG_S(...) ESPZW_LOG_S(__VA_ARGS__)
#endif

#if ESPZW_DEBUG_LEVEL < 2
	#define ESPZW_DEBUGVDO(...)
	#define ESPZW_DEBUGV(...)
	#define ESPZW_DEBUGV_S(...)
	#else
	#define ESPZW_DEBUGVDO(...) __VA_ARGS__
	#define ESPZW_DEBUGV(...) ESPZW_LOG(__VA_ARGS__)
	#define ESPZW_DEBUGV_S(...) ESPZW_LOG_S(__VA_ARGS__)
#endif

#if ESPZW_DEBUG_LEVEL < 3
	#define ESPZW_DEBUGVVDO(...)
	#define ESPZW_DEBUGVV(...)
	#define ESPZW_DEBUGVV_S(...)
#else
	#define ESPZW_DEBUGVVDO(...) __VA_ARGS__
	#define ESPZW_DEBUGVV(...) ESPZW_LOG(__VA_ARGS__)
	#define ESPZW_DEBUGVV_S(...) ESPZW_LOG_S(__VA_ARGS__)
#endif

extern char const HexLookup_UC[];
extern char const HexLookup_LC[];

#define MD5_BINLEN	16
#define MD5_TXTLEN	(MD5_BINLEN*2)

// out is a 16 byte buffer
void calcMD5(uint8_t* data, uint16_t len, uint8_t *out);
// text is a 32 char buffer, Lookup is a mapping of hex numbers
void textMD5(uint8_t* data, uint16_t len, char *text, char const *Lookup);

inline void textMD5_UC(uint8_t* data, uint16_t len, char *text)
{ textMD5(data,len,text,HexLookup_UC); }
inline void textMD5_LC(uint8_t* data, uint16_t len, char *text)
{ textMD5(data,len,text,HexLookup_LC); }

#if ESPZW_SHA256

#define SHA256_BINLEN	64
#define SHA256_TXTLEN	(SHA256_BINLEN*2)

// out is a 64 byte buffer
void calcSHA256(uint8_t* data, uint16_t len, uint8_t *out);
// text is a 128 char buffer, Lookup is a mapping of hex numbers
void textSHA256(uint8_t* data, uint16_t len, char *text, char const *Lookup);

inline void textSHA256_UC(uint8_t* data, uint16_t len, char *text)
{ textSHA256(data,len,text,HexLookup_UC); }
inline void textSHA256_LC(uint8_t* data, uint16_t len, char *text)
{ textSHA256(data,len,text,HexLookup_LC); }

#endif

String getQuotedToken(char const *&ptr, char const delim = ';');
void putQuotedToken(String const &token, String &out, char const delim = ';', bool delimPfx = true, bool forceQuote = false);

#include "FS.h"

bool pathIsAbsolute(String const &path);
String pathGetParent(String const &path);
String pathGetEntryName(String const &path);
String pathAppend(String const &path, const char *token);
String pathAppend(String const &path, String const &token);

fs::Dir mkdir(fs::FS &fs, String const &path);
fs::Dir mkdirs(fs::FS &fs, String const &path);

#endif