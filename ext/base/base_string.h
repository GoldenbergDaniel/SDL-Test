#pragma once

#include "base_common.h"
#include "base_arena.h"

typedef struct String String;
typedef struct StringNode StringNode;
typedef struct StringArray StringArray;
typedef struct StringStack StringStack;
typedef struct StringQueue StringQueue;

// @String =====================================================================================

struct String
{
  char *str;
  u32 len;
};

#define str(s) ((String) {s, cstr_len(s)-1})

String alloc_str(u32 len, Arena *arena);
String str_from_cstring(char *cstr, Arena *arena);
bool str_equals(String s1, String s2);
bool str_contains(String s, String substr);
i64 str_find(String s, String substr, u64 start, u64 end);
i64 str_find_char(String s, char c, u32 start, u64 end);
String str_copy(String s, Arena *arena);
String str_copy_into(String src, String *dest);
String str_insert_at(String s, String substr, u32 loc, Arena *arena);
String str_concat(String s1, String s2, Arena *arena);
String str_substr(String s, u32 start, u32 end, Arena *arena);
String str_strip_front(String s, String substr, Arena *arena);
String str_strip_back(String s, String substr, Arena *arena);
String str_nullify(String s, Arena *arena);
String str_to_lower(String s, Arena *arena);
String str_to_upper(String s, Arena *arena);
String str_join(StringArray arr, String delimiter, Arena *arena);
StringArray str_split(String s, String delimiter, Arena *arena);
void print_str(String s);

// @StringArray ================================================================================

struct StringArray
{
  String *e;
  u64 count;
};

StringArray create_str_array(u64 count, Arena *arena);
void clear_str_array(StringArray *arr, Arena *arena);

// @StringStack ================================================================================

struct StringNode
{
  StringNode *next;
  StringNode *prev;
  String data;
};

struct StringStack
{
  StringNode *top;
  u64 count;
};

void str_stack_push(StringStack *stack, String data, Arena *arena);
String str_stack_pop(StringStack *stack, Arena *arena);
String str_stack_peek(StringStack *stack, Arena *arena);
void str_stack_clear(StringStack *stack, Arena *arena);

// @StringQueue ================================================================================

struct StringQueue
{
  StringNode *front;
  StringNode *back;
  u64 count;
};

void str_queue_push(StringQueue *queue, String data, Arena *arena);
String str_queue_pop(StringQueue *queue, Arena *arena);
String str_queue_peek(StringQueue *queue, Arena *arena);
void str_queue_clear(StringQueue *queue, Arena *arena);

// @CStr =======================================================================================

u32 cstr_len(char *cstr);
void cstr_copy(String *dest, char *src);
