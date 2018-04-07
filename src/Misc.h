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
#ifndef ZWUtils_Misc_H_
#define ZWUtils_Misc_H_

#ifndef ESPZW_LOG_TIME
#define ESPZW_LOG_TIME 0
#endif

#include "WString.h"
#include <inttypes.h>
#include <pgmspace.h>
#include <sys/time.h>

#ifndef ESPZW_DEBUG_LEVEL
#define ESPZW_DEBUG_LEVEL 0
#endif

#define LPROGMEM_C  __attribute__((section(".irom.text.log.common")))
#define LPROGMEM_T  __attribute__((section(".irom.text.log.template")))
#define LPROGMEM_L  __attribute__((section(".irom.text.log.local")))

#ifndef ESPZW_LOG
#if ESPZW_LOG_TIME
#define ESPZW_LOG_X(spfx, fmt, ...)											\
{ 																			\
	static const char __pfmt__[] LPROGMEM ## spfx = "%5d.%03d | " fmt;		\
	uint64_t __log_time__ = micros64()/1000;								\
	Serial.printf_P(__pfmt__, ((uint32_t)(__log_time__/1000) % 100000),		\
		(uint16_t)(__log_time__ % 1000), ## __VA_ARGS__);					\
}
#else
#define ESPZW_LOG_X(spfx, fmt, ...)							\
{ 															\
	static const char __pfmt__[] LPROGMEM ## spfx = fmt;	\
	Serial.printf_P(__pfmt__, ## __VA_ARGS__);				\
}
#endif
#define ESPZW_LOG_S(sect, fmt, ...)	ESPZW_LOG_X(_ ## sect,fmt, ## __VA_ARGS__)
#define ESPZW_LOG(fmt, ...) ESPZW_LOG_S(C,fmt, ## __VA_ARGS__)
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

#define SPROGMEM_C  __attribute__((section(".irom.text.str.common")))
#define SPROGMEM_T  __attribute__((section(".irom.text.str.template")))
#define SPROGMEM_L  __attribute__((section(".irom.text.str.local")))
#define SPROGMEM_S  __attribute__((section(".irom.text.str.static")))

#define SPSTR_X(spfx, s) (__extension__({ \
	static const char __c[] SPROGMEM_ ## spfx = (s); &__c[0]; \
}))

#define PSTR_C(s) SPSTR_X(C,s)
#define PSTR_T(s) SPSTR_X(T,s)
#define PSTR_L(s) SPSTR_X(L,s)

#define FC(string_literal) (FPSTR(PSTR_C(string_literal)))
#define FT(string_literal) (FPSTR(PSTR_T(string_literal)))
#define FL(string_literal) (FPSTR(PSTR_L(string_literal)))

#define SFPSTR(x) String(FPSTR(x)).c_str()

extern PGM_P HexLookup_UC;
extern PGM_P HexLookup_LC;

#define MD5_BINLEN	16
#define MD5_TXTLEN	(MD5_BINLEN*2)

// out is a 16 byte buffer
void calcMD5(void const *data, uint16_t len, uint8_t *out);
// text is a 32 char buffer, Lookup is a mapping of hex numbers
void textMD5(void const *data, uint16_t len, char *text, char const *Lookup);

inline void textMD5_UC(void const *data, uint16_t len, char *text)
{ textMD5(data,len,text,HexLookup_UC); }
inline void textMD5_LC(void const *data, uint16_t len, char *text)
{ textMD5(data,len,text,HexLookup_LC); }

#if ESPZW_SHA256

#define SHA256_BINLEN	64
#define SHA256_TXTLEN	(SHA256_BINLEN*2)

// out is a 64 byte buffer
void calcSHA256(void const *data, uint16_t len, uint8_t *out);
// text is a 128 char buffer, Lookup is a mapping of hex numbers
void textSHA256(void const *data, uint16_t len, char *text, char const *Lookup);

inline void textSHA256_UC(void const *data, uint16_t len, char *text)
{ textSHA256(data,len,text,HexLookup_UC); }
inline void textSHA256_LC(void const *data, uint16_t len, char *text)
{ textSHA256(data,len,text,HexLookup_LC); }

#endif

String getQuotedToken(char const *&ptr, char const delim = ';');
void putQuotedToken(String const &token, String &out, char const delim = ';', bool delimPfx = true, bool forceQuote = false);

String getBuildStamp(String const &date, String const &time);

#include "FS.h"

bool pathIsAbsolute(String const &path);
String pathGetParent(String const &path);
const char *pathGetEntryName(String const &path);
const char *pathGetEntryName(String const &path, uint16_t len);
void pathAppend(String &path, const char *token);
void pathAppend(String &path, String const &token);
String pathJoin(String const &path, const char *token);
String pathJoin(String const &path, String const &token);

fs::Dir mkdir(fs::FS &fs, String const &path);
fs::Dir mkdirs(fs::FS &fs, String const &path);

#endif