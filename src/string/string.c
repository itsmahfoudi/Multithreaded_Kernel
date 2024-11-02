#include "string.h"

int strlen(const char* str)
{
  int len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

int strnlen(const char* str, int max_len)
{
  int len = 0;
  while (str[len] && len < max_len) {
    len++;
  }
  return len;
}

int is_digit(char c)
{
  return c >= '0' && c <= '9';
}

int to_numeric_digit(char c)
{
  return c - '0';
}