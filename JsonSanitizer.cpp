// ----------------------------------------------------------------------------
// JsonSanitizer.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "JsonSanitizer.h"


void JsonSanitizer::sanitize(char *json)
{
  int i;
  int r;
  jsmn_parser p;
  jsmntok_t tok[32];

  char first_char = firstChar(json);
  if ((first_char != '[') && (first_char != '{'))
  {
    Serial << "not array or object!" << endl;
    return;
  }

  jsmn_init(&p);
  r = jsmn_parse(&p, json, strlen(json), tok, sizeof(tok)/sizeof(tok[0]));
  if (r < 0)
  {
    Serial << "Failed to parse JSON: " << r << endl;
    return;
  }
  Serial << "Parsed JSON into " << r << " tokens!" << endl;
  if (tok->type != JSMN_PRIMITIVE)
  {
    int tokens_printed_total = 0;
    int count = p.toknext;
    while (tokens_printed_total < r)
    {
      int tokens_printed = dump(json,tok,count,0);
      count -= tokens_printed;
      tokens_printed_total += tokens_printed;
    }
    Serial << "tokens printed: " << tokens_printed_total << endl;
  }
}

char *JsonSanitizer::skipCStyleComment(char *ptr)
{
  ptr += 2;
  for (;;)
  {
    if (ptr[0] == '\0') return ptr;
    if (ptr[0] == '*' && ptr[1] == '/') return ptr + 2;
    ptr++;
  }
}

char *JsonSanitizer::skipCppStyleComment(char *ptr)
{
  ptr += 2;
  for (;;)
  {
    if (ptr[0] == '\0' || ptr[0] == '\n') return ptr;
    ptr++;
  }
}

char *JsonSanitizer::skipSpacesAndComments(char *ptr) {
  for (;;) {
    switch (ptr[0]) {
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        ptr++;
        continue;
      case '/':
        switch (ptr[1]) {
          case '*':
            ptr = skipCStyleComment(ptr);
            break;
          case '/':
            ptr = skipCppStyleComment(ptr);
            break;
          default:
            return ptr;
        }
        break;
      default:
        return ptr;
    }
  }
}

int JsonSanitizer::dump(const char *js, jsmntok_t *t, size_t count, int indent)
{
  // Serial << "count: " << count << endl;
  int i, j, k;
  if (count == 0)
  {
    return 0;
  }
  if (t->type == JSMN_PRIMITIVE)
  {
    // printf("%.*s", t->end - t->start, js+t->start);
    int str_len = t->end - t->start;
    char str[str_len + 1];
    str[str_len] = 0;
    memcpy(str,js+t->start,str_len);
    // Serial << "primative:" << str << " count: " << count << endl;
    Serial << "primative: " << str << endl;
    return 1;
  }
  else if (t->type == JSMN_STRING)
  {
    // printf("'%.*s'", t->end - t->start, js+t->start);
    int str_len = t->end - t->start;
    char str[str_len + 3];
    str[str_len+2] = 0;
    str[0] = '\"';
    str[str_len+1] = '\"';
    memcpy(str+1,js+t->start,str_len);
    Serial << "string: " << str << endl;
    return 1;
  }
  else if (t->type == JSMN_OBJECT)
  {
    // printf("\n");
    Serial << "object:" << endl;
    j = 0;
    for (i = 0; i < t->size; i++)
    {
      for (k = 0; k < indent; k++)
      {
        // printf("  ");
        Serial << "  ";
      }
      j += dump(js, t+1+j, count-j, indent+1);
      // printf(": ");
      Serial << ": ";
      j += dump(js, t+1+j, count-j, indent+1);
      // printf("\n");
      Serial << endl;
    }
    return j+1;
  }
  else if (t->type == JSMN_ARRAY)
  {
    j = 0;
    // printf("\n");
    Serial << "array:" << endl;
    for (i = 0; i < t->size; i++)
    {
      for (k = 0; k < indent-1; k++)
      {
        // printf("  ");
        Serial << "  ";
      }
      // printf("   - ");
      Serial << "   - ";
      j += dump(js, t+1+j, count-j, indent+1);
      // printf("\n");
      Serial << endl;
    }
    return j+1;
  }
  return 0;
}

char JsonSanitizer::firstChar(char* ptr)
{
  ptr = skipSpacesAndComments(ptr);
  return ptr[0];
}
