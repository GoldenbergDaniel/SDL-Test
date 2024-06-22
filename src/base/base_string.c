#include "base.h"

// @String ===============================================================================

String alloc_str(u64 len, Arena *arena)
{
  String result;
  result.data = arena_push(arena, char, len);
  // logger_debug(str("%i\n"), result.len);
  result.len = len;

  for (u64 i = 0; i < len; i++)
  {
    result.data[i] = '\0';
  }

  return result;
}

inline
String str_from_cstring(char *cstr, Arena *arena)
{
  return str_copy((String) {cstr, cstr_len(cstr)-1}, arena);
}

bool str_equals(String s1, String s2)
{
  if (s1.len != s2.len) return FALSE;

  bool result = TRUE;

  for (u64 i = 0; i < s1.len; i++)
  {
    if (s1.data[i] != s2.data[i])
    {
      result = FALSE;
      break;
    }
  }

  return result;
}

bool str_contains(String s, String substr)
{
  if (s.len < substr.len) return FALSE;

  bool result = FALSE;

  for (u64 i = 0; i < s.len-substr.len+1; i++)
  {
    if (s.data[i] == substr.data[0])
    {
      for (u64 j = 0; j < substr.len; j++)
      {
        if (s.data[i+j] != substr.data[j]) break;

        if (j == substr.len-1)
        {
          result = TRUE;
          return result;
        }
      }
    }
  }

  return result;
}

i64 str_find(String s, String substr, u64 start, u64 end)
{
  if (s.len < substr.len || start >= s.len) return -1;

  i64 result = -1;

  for (u64 i = start; i < s.len-substr.len+1; i++)
  {
    if (s.data[i] == substr.data[0])
    {
      for (u64 j = 0; j < substr.len; j++)
      {
        if (s.data[i+j] != substr.data[j]) break;

        if (j == substr.len-1)
        {
          result = i;
          return result;
        }
      }
    }
  }

  return result;
}

i64 str_find_char(String s, char c, u64 start, u64 end)
{
  if (start >= s.len) return FALSE;

  i64 result = -1;

  for (u64 i = start; i < s.len; i++)
  {
    if (s.data[i] == c)
    {
      result = i;
      break;
    }
  }

  return result;
}

String str_copy(String str, Arena *arena)
{
  String result = {0};
  result.data = arena_push(arena, char, str.len+1);

  for (u64 i = 0; i < str.len; i++)
  {
    result.data[i] = str.data[i];
  }
  
  result.len = str.len;
  result.data[result.len] = '\0';

  return result;
}

String str_copy_into(String src, String *dest)
{
  for (u64 i = 0; i < src.len; i++)
  {
    dest->data[i] = src.data[i];
  }
  
  dest->len = src.len;

  return *dest;
}

String str_insert_at(String str, String substr, u64 loc, Arena *arena)
{ 
  u64 len = str.len >= substr.len + loc ? str.len : substr.len + loc;
  String result = alloc_str(len, arena);

  u64 substr_idx = 0;
  for (u64 i = 0; i < len; i++)
  {
    if (i >= loc && i < substr.len + loc)
    {
      result.data[i] = substr.data[substr_idx];
      substr_idx++;
    }
    else
    {
      result.data[i] = str.data[i];
    }
  }

  return result;
}

String str_concat(String s1, String s2, Arena *arena)
{
  String result = alloc_str(s1.len + s2.len + 1, arena);
  result.len -= 1;

  for (u64 i = 0; i < s1.len; i++)
  {
    result.data[i] = s1.data[i];
  }

  for (u64 i = 0; i < s2.len; i++)
  {
    result.data[i+s1.len] = s2.data[i];
  }
  
  result.data[result.len] = '\0';

  return result;
}

String str_substr(String str, u64 start, u64 end)
{
  assert(start >= 0 && start < str.len && end > 0 && end <= str.len && start < end);

  String result = {0};
  result.data = str.data + start;
  result.len = end - start;

  return result;
}

String str_strip_front(String s, String substr)
{
  assert(substr.len <= s.len);

  String result = {0};

  u64 front_len = substr.len;
  String front = str_substr(s, 0, front_len);
  if (str_equals(front, substr))
  {
    result = str_substr(s, front_len, s.len);
    result.len = s.len-front_len;
  }
  
  return result;
}

String str_strip_back(String s, String substr)
{
  assert(substr.len <= s.len);

  String result = {0};

  u64 back_len = s.len - substr.len;
  String back = str_substr(s, back_len, s.len);
  if (str_equals(back, substr))
  {
    result = str_substr(s, 0, back_len);
    result.len = back_len;
  }
  
  return result;
}

String str_to_lower(String s, Arena *arena)
{
  String result = alloc_str(s.len, arena);

  for (u64 i = 0; i < s.len; i++)
  {
    if (s.data[i] >= 'A' && s.data[i] <= 'Z')
    {
      result.data[i] = s.data[i] + 32;
    }
    else
    {
      result.data[i] = s.data[i];
    }
  }

  return result;
}

String str_to_upper(String s, Arena *arena)
{
  String result = alloc_str(s.len, arena);

  for (u64 i = 0; i < s.len; i++)
  {
    if (s.data[i] >= 'a' && s.data[i] <= 'z')
    {
      result.data[i] = s.data[i] - 32;
    }
    else
    {
      result.data[i] = s.data[i];
    }
  }

  return result;
}

String str_join(StringArray arr, String delimiter, Arena *arena)
{
  String result = {0};
  String temp = {0};

  u64 total_len = (arr.count-1) * delimiter.len;
  for (u64 i = 0; i < arr.count; i++)
  {
    total_len += arr.e[i].len;
  }

  result = alloc_str(total_len, arena);

  Arena scratch = get_scratch_arena(arena);

  u64 start_offset = 0;
  for (u64 i = 0; i < arr.count; i++)
  {
    temp = str_insert_at(temp, arr.e[i], start_offset, &scratch);
    start_offset += arr.e[i].len;

    if (i != arr.count-1)
    {
      temp = str_insert_at(temp, delimiter, start_offset, &scratch);
      start_offset += delimiter.len;
    }
  }

  result = str_copy(temp, arena);
  
  arena_clear(&scratch);

  return result;
}

StringArray str_split(String s, String delimiter, Arena *arena)
{
  StringArray result = {0};

  return result;
}

// @StringArray ==========================================================================

StringArray create_str_array(u64 count, Arena *arena)
{
  StringArray arr = {0};
  arr.count = count;
  arr.e = arena_push(arena, String, count);

  return arr;
}

void clear_str_array(StringArray *arr, Arena *arena)
{
  for (u64 i = 0; i < arr->count; i++)
  {
    arr->e[i] = str("");
  }

  arr->count = 0;
}

// @CString ==============================================================================

inline
u64 cstr_len(char *s)
{
  u64 len = 0;
  for (; s[len]; len++);
  return len+1;
}

void copy_cstr_into_str(String *dest, char *src)
{
  u64 len = cstr_len(src)-1;
  for (u64 i = 0; i < len; i++)
  {
    dest->data[i] = src[i];
  }
  
  dest->len = len;
}
