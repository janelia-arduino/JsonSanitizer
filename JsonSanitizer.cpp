// ----------------------------------------------------------------------------
// JsonSanitizer.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "JsonSanitizer.h"


JsonSanitizer::JsonSanitizer()
{
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
    if (json[0] == '[' || json[0] == '{' || json[0] == '\n' || json[0] == '\0')
    {
      return json;
    }
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

void JsonSanitizer::writePrimative(char *json, char * source, JsmnStream::jsmntok_t *t)
{
  size_t str_len = t->end - t->start;
  memcpy(json+json_index_,source+t->start,str_len);
  json_index_ += str_len;
}

void JsonSanitizer::writeString(char *json, char *source, JsmnStream::jsmntok_t *t)
{
  size_t str_len = t->end - t->start;
  json[json_index_] = '\"';
  json[json_index_+str_len+1] = '\"';
  memcpy(json+json_index_+1,source+t->start,str_len);
  json_index_ += str_len+2;
}

size_t JsonSanitizer::writeTokensToJson(char *json, char *source, JsmnStream::jsmntok_t *t, size_t count)
{
  int i,j;
  if (count == 0) return 0;
  if (t->type == JsmnStream::JSMN_PRIMITIVE)
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
  else if (t->type == JsmnStream::JSMN_STRING)
  {
    writeString(json,source,t);
    return 1;
  }
  else if (t->type == JsmnStream::JSMN_OBJECT)
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
  else if (t->type == JsmnStream::JSMN_ARRAY)
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
