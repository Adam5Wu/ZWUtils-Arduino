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

#include "Arduino.h"
#include "md5.h"

#include "Misc.h"

String const EMPTY_STRING;

char const HexLookup_UC[] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

char const HexLookup_LC[] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

void calcMD5(uint8_t* data, uint16_t len, uint8_t *out) {
  md5_context_t _ctx;
  MD5Init(&_ctx);
  MD5Update(&_ctx, data, len);
  MD5Final(out, &_ctx);
}

void textMD5(uint8_t* data, uint16_t len, char *text, char const *Lookup) {
	uint8_t buf[16];
	calcMD5(data,len,buf);
  for(uint8_t i = 0; i < 16; i++) {
		text[i*2+0] = Lookup[(buf[i] >> 4) & 0xF];
		text[i*2+1] = Lookup[(buf[i] >> 0) & 0xF];
  }
}

String getQuotedToken(char const *&ptr, char const delim) {
	String Ret;
	if (*ptr == '"') {
		bool unquote = false;
		while (*++ptr) {
			if (*ptr == '"') {
				unquote = true;
				if (*++ptr && *ptr != delim) {
					ESPZW_DEBUG("WARNING: Improperly terminated quoted string '%s'\n",Ret.c_str());
				} else ptr++;
				break;
			} else if (*ptr == '\\') {
				Ret.concat(*++ptr);
			} else Ret.concat(*ptr);
		}
		if (!unquote)
			ESPZW_DEBUG("WARNING: Unterminated quoted string\n",Ret.c_str());
	} else {
		while (*ptr) {
			if (*ptr == delim) {
				ptr++;
				break;
			} else Ret.concat(*ptr);
			ptr++;
		}
	}
	return Ret;
}

void putQuotedToken(String const &token, String &out, char const delim, bool delimPfx, bool forceQuote) {
	if (!out.empty() && delimPfx) out.concat(delim);
	if (forceQuote || token.indexOf(delim) >= 0 || token[0] == '"') {
		out.concat('"');
		for (int idx = 0; idx < token.length(); idx++) {
			char C = token[idx];
			if (C == '"') out.concat('\\');
			out.concat(C);
		}
		out.concat('"');
	} else out.concat(token);	
}