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

#include "Misc.h"

char const HexLookup_UC[] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

char const HexLookup_LC[] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

#include "md5.h"

void calcMD5(void const *data, uint16_t len, uint8_t *out) {
	md5_context_t _ctx;
	MD5Init(&_ctx);
	MD5Update(&_ctx, (uint8_t const *)data, len);
	MD5Final(out, &_ctx);
}

void textMD5(void const *data, uint16_t len, char *text, char const *Lookup) {
	uint8_t buf[16];
	calcMD5(data,len,buf);
	for(uint8_t i = 0; i < 16; i++) {
		text[i*2+0] = Lookup[(buf[i] >> 4) & 0xF];
		text[i*2+1] = Lookup[(buf[i] >> 0) & 0xF];
	}
}

#if ESPZW_SHA256

#include "sha256.h"

void calcSHA256(void const *data, uint16_t len, uint8_t *out) {
	sha256_context_t _ctx;
	SHA256Init(&_ctx);
	SHA256Update(&_ctx, data, len);
	SHA256Final(out, &_ctx);
}

void textSHA256(void const *data, uint16_t len, char *text, char const *Lookup) {
	uint8_t buf[64];
	calcSHA256(data,len,buf);
	for(uint8_t i = 0; i < 64; i++) {
		text[i*2+0] = Lookup[(buf[i] >> 4) & 0xF];
		text[i*2+1] = Lookup[(buf[i] >> 0) & 0xF];
	}
}

#endif

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
			ESPZW_DEBUG("WARNING: Un-terminated quoted string\n",Ret.c_str());
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
	if (out && delimPfx) out.concat(delim);
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

bool pathIsAbsolute(String const &path) {
	return path? path[0] == '/' : false;
}

String pathGetParent(String const &path) {
	int delim_pos = path.lastIndexOf('/');
	if (delim_pos < 0) return String::EMPTY;
	if (delim_pos == 0) return "/";
	return String(path.begin(), delim_pos);
}

const char *pathGetEntryName(const char *path, uint16_t len) {
	const char* p = path+len;
	while (p > path && *p != '/') --p;
	return *p == '/'? p+1 : p;
}

const char *pathGetEntryName(String const &path) {
	return pathGetEntryName(path.c_str(), path.length());
}

void pathAppend(String &path, const char *token) {
	pathAppend(path, String(token));
}

void pathAppend(String &path, String const &token) {
	if (!path || path.end()[-1] != '/') path.concat('/');
	path.concat(token);
}

String pathJoin(String const &path, const char *token) {
	return pathJoin(path, String(token));
}

String pathJoin(String const &path, String const &token) {
	String Ret = path;
	pathAppend(Ret, token);
	return std::move(Ret);
}

fs::Dir mkdir(fs::FS &fs, String const &path) {
	if (!pathIsAbsolute(path)) {
		ESPZW_DEBUG("WARNING: Unable to create dir - not an absolute path\n");
		return fs::Dir();
	}
	return fs.openDir(path.c_str(), !fs.exists(path));
}

static fs::Dir mkdirs_recursive(fs::FS &fs, String const &path) {
	if (fs.exists(path)) {
		ESPZW_DEBUGV("Opening directory '%s'\n", path.c_str());
		return fs.openDir(path.c_str());
	}

	auto parentPath = pathGetParent(path);
	ESPZW_DEBUGV("Checking directory '%s'\n", parentPath.c_str());
	auto parentDir = mkdirs_recursive(fs, parentPath);
	if (!parentDir) {
		ESPZW_DEBUG("WARNING: Unable to create parent dir '%s'\n", parentPath.c_str());
		return parentDir;
	}
	ESPZW_DEBUGV("Creating directory '%s'\n", path.c_str());
	return parentDir.openDir(pathGetEntryName(path), true);
}

fs::Dir mkdirs(fs::FS &fs, String const &path) {
	if (!pathIsAbsolute(path)) {
		ESPZW_DEBUG("WARNING: Unable to create dir - not an absolute path\n");
		return fs::Dir();
	}
	return mkdirs_recursive(fs, path);
}
