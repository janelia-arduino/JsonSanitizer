// ----------------------------------------------------------------------------
// JsonSanitizer.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "JsonSanitizer.h"


JsonSanitizer::JsonSanitizer(size_t token_count) :
  TOKEN_COUNT_(token_count)
{
}

void JsonSanitizer::sanitize(char json[], size_t size)
{
  // Make sure json is proper null terminated string
  size_t json_length = strlen(json);
  if (json_length >= size) return;

  // Trim initial whitespace
  char *json_trimmed = skipSpacesAndComments(json);
  json_length = strlen(json_trimmed);

  // Make intermediate buffer. todo: possible to remove?
  char source[size];

  // Fill source, add [] or {} if necessary
  if (firstCharIsValidJson(json))
  {
    memcpy(source,json_trimmed,json_length+1);
  }
  else
  {
    // Make sure buffer is large enough to add characters
    if ((json_length + 2) >= size) return;

    // Shift all characters over one
    memcpy(source+1,json_trimmed,++json_length);
    if (colonInFirstTwoWords(json_trimmed))
    {
      source[0] = '{';
      source[json_length++] = '}';
    }
    else
    {
      source[0] = '[';
      source[json_length++] = ']';
    }
    source[json_length] = 0;
  }

  int r;
  jsmn_parser p;
  jsmntok_t tok[TOKEN_COUNT_];

  jsmn_init(&p);
  r = jsmn_parse(&p, source, json_length, tok, sizeof(tok)/sizeof(tok[0]));
  if (r < 0) return;
  if (tok->type == JSMN_PRIMITIVE) return;
  size_t tokens_printed_total = 0;
  size_t count = p.toknext;
  json_index_ = 0;
  json[json_index_] = 0;
  while (tokens_printed_total < r)
  {
    size_t tokens_printed = writeTokensToJson(json,source,tok,count);
    count -= tokens_printed;
    tokens_printed_total += tokens_printed;
  }
  json[json_index_] = 0;
}

bool JsonSanitizer::firstCharIsValidJsonArray(char *json)
{
  char first_char = firstChar(json);
  return (first_char == '[');
}

bool JsonSanitizer::firstCharIsValidJsonObject(char *json)
{
  char first_char = firstChar(json);
  return (first_char == '{');
}

bool JsonSanitizer::firstCharIsValidJson(char *json)
{
  char first_char = firstChar(json);
  return ((first_char == '[') || (first_char == '{'));
}

char *JsonSanitizer::skipCStyleComment(char *json)
{
  json += 2;
  for (;;)
  {
    if (json[0] == '\0') return json;
    if (json[0] == '*' && json[1] == '/') return json + 2;
    json++;
  }
}

char *JsonSanitizer::skipCppStyleComment(char *json)
{
  json += 2;
  for (;;)
  {
    if (json[0] == '\0' || json[0] == '\n') return json;
    json++;
  }
}

char *JsonSanitizer::skipSpacesAndComments(char *json)
{
  for (;;)
  {
    switch (json[0])
    {
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        json++;
        continue;
      case '/':
        switch (json[1])
        {
          case '*':
            json = skipCStyleComment(json);
            break;
          case '/':
            json = skipCppStyleComment(json);
            break;
          default:
            return json;
        }
        break;
      default:
        return json;
    }
  }
}

char JsonSanitizer::firstChar(char* json)
{
  json = skipSpacesAndComments(json);
  return json[0];
}

bool JsonSanitizer::colonInFirstTwoWords(char* json)
{
  json = skipSpacesAndComments(json);
  char keys[] = " \t\r\n";
  size_t i = strcspn(json,keys);
  char first_word[i+1];
  memcpy(first_word,json,i);
  first_word[i] = 0;
  char *pch=strchr(first_word,':');
  if (pch!=NULL) return true;
  json += i;
  json = skipSpacesAndComments(json);
  i = strcspn(json,keys);
  char second_word[i+1];
  memcpy(second_word,json,i);
  second_word[i] = 0;
  pch=strchr(second_word,':');
  if (pch!=NULL) return true;
  return false;
}

bool JsonSanitizer::isInRange(char c, char min, char max)
{
  return min <= c && c <= max;
}

bool JsonSanitizer::isNumber(char c)
{
  return isInRange(c, '0', '9') || c == '-' || c == '.';
}

void JsonSanitizer::writePrimative(char *json, char * source, jsmntok_t *t)
{
  size_t str_len = t->end - t->start;
  memcpy(json+json_index_,source+t->start,str_len);
  json_index_ += str_len;
}

void JsonSanitizer::writeString(char *json, char *source, jsmntok_t *t)
{
  size_t str_len = t->end - t->start;
  json[json_index_] = '\"';
  json[json_index_+str_len+1] = '\"';
  memcpy(json+json_index_+1,source+t->start,str_len);
  json_index_ += str_len+2;
}

size_t JsonSanitizer::writeTokensToJson(char *json, char *source, jsmntok_t *t, size_t count)
{
  int i,j;
  if (count == 0) return 0;
  if (t->type == JSMN_PRIMITIVE)
  {
    char *primative = source+t->start;
    if ((memcmp(primative,"null",4) == 0) ||
        (memcmp(primative,"true",4) == 0) ||
        (memcmp(primative,"false",5) == 0) ||
        (isNumber(primative[0])))
    {
      writePrimative(json,source,t);
    }
    else
    {
      writeString(json,source,t);
    }
    return 1;
  }
  else if (t->type == JSMN_STRING)
  {
    writeString(json,source,t);
    return 1;
  }
  else if (t->type == JSMN_OBJECT)
  {
    json[json_index_++] = '{';
    j = 0;
    for (i = 0; i < t->size; i++)
    {
      j += writeTokensToJson(json, source, t+1+j, count-j);
      json[json_index_++] = ':';
      j += writeTokensToJson(json, source, t+1+j, count-j);
      json[json_index_++] = ',';
    }
    // Overwrite last comma
    if (t->size > 0)
    {
      json[json_index_-1] = '}';
    }
    else
    {
      json[json_index_++] = '}';
    }
    return j+1;
  }
  else if (t->type == JSMN_ARRAY)
  {
    j = 0;
    json[json_index_++] = '[';
    for (i = 0; i < t->size; i++)
    {
      j += writeTokensToJson(json, source, t+1+j, count-j);
      json[json_index_++] = ',';
    }
    // Overwrite last comma
    if (t->size > 0)
    {
      json[json_index_-1] = ']';
    }
    else
    {
      json[json_index_++] = ']';
    }
    return j+1;
  }
  return 0;
}
