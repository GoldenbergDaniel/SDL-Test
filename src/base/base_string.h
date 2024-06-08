#pragma once

#include "base_common.h"
#include "base_arena.h"

// @String ===============================================================================

typedef struct String String;
struct String
{
  char *data;
  u64 len;
};

typedef struct StringArray StringArray;
struct StringArray
{
  String *e;
  u64 count;
};

#define str(lit) ((String) {lit, sizeof (lit) - 1})

String alloc_str(u64 len, Arena *arena);
String str_from_cstring(char *cstr, Arena *arena);
bool str_equals(String s1, String s2);
bool str_contains(String s, String substr);
i64 str_find(String s, String substr, u64 start, u64 end);
i64 str_find_char(String s, char c, u64 start, u64 end);
String str_substr(String s, u64 start, u64 end);
String str_copy(String s, Arena *arena);
String str_copy_into(String src, String *dest);
String str_insert_at(String s, String substr, u64 loc, Arena *arena);
String str_concat(String s1, String s2, Arena *arena);
String str_strip_front(String s, String substr);
String str_strip_back(String s, String substr);
String str_to_lower(String s, Arena *arena);
String str_to_upper(String s, Arena *arena);
String str_join(StringArray arr, String delimiter, Arena *arena);
StringArray str_split(String s, String delimiter, Arena *arena);

// @StringArray ==========================================================================

StringArray create_str_array(u64 count, Arena *arena);
void clear_str_array(StringArray *arr, Arena *arena);

// @CStr =================================================================================

u64 cstr_len(char *cstr);
void cstr_copy(String *dest, char *src);
