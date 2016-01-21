// ----------------------------------------------------------------------------
// JsonSanitizerDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef JSON_SANITIZER_DEFINITIONS_H
#define JSON_SANITIZER_DEFINITIONS_H


template <size_t TOKEN_COUNT_MAX>
JsonSanitizer<TOKEN_COUNT_MAX>::JsonSanitizer() :
  jsmn_stream_(tokens_)
{
  reset();
}

template <size_t TOKEN_COUNT_MAX>
void JsonSanitizer<TOKEN_COUNT_MAX>::reset()
{
  jsmn_stream_.resetParser();
  buffer_pos_ = 0;
  state_ = OUTSIDE_JSON;
}

template <size_t TOKEN_COUNT_MAX>
template <size_t BUFFER_SIZE>
void JsonSanitizer<TOKEN_COUNT_MAX>::sanitizeCharIntoBuffer(const char c, char (&buffer)[BUFFER_SIZE])
{
  switch (char_parse_result_)
  {
  }
  if (buffer_pos_ < (BUFFER_SIZE-1))
  {
    buffer[buffer_pos_++] = c;
    buffer[buffer_pos_] = '\0';
  }
}

template <size_t TOKEN_COUNT_MAX>
template <size_t BUFFER_SIZE>
void JsonSanitizer<TOKEN_COUNT_MAX>::sanitizeBuffer(char (&json_buffer)[BUFFER_SIZE])
{
  // Make sure json is proper null terminated string
  size_t json_length = strlen(json_buffer);
  if (json_length >= BUFFER_SIZE) return;

  // Trim initial whitespace
  char *json_trimmed = skipSpacesAndComments(json_buffer);
  json_length = strlen(json_trimmed);

  // Make intermediate buffer. todo: possible to remove?
  char source[BUFFER_SIZE];

  // Fill source, add [] or {} if necessary
  if (firstCharIsValidJson(json_buffer))
  {
    memcpy(source,json_trimmed,json_length+1);
  }
  else
  {
    // Make sure buffer is large enough to add characters
    if ((json_length + 2) >= BUFFER_SIZE) return;

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

  jsmn_stream_.resetParser();

  int ret;
  ret = jsmn_stream_.parseJson(source);
  ret = jsmn_stream_.checkParse();
  if (ret < 0) return;
  if (tokens_->type == JsmnStream::JSMN_PRIMITIVE) return;
  int tokens_printed_total = 0;
  int token_count = jsmn_stream_.getTokenCount();
  size_t count = token_count;
  json_index_ = 0;
  json_buffer[json_index_] = 0;
  while (tokens_printed_total < token_count)
  {
    size_t tokens_printed = writeTokensToJson(json_buffer,source,tokens_,count);
    count -= tokens_printed;
    tokens_printed_total += tokens_printed;
  }
  json_buffer[json_index_] = 0;
}

template <size_t TOKEN_COUNT_MAX>
bool JsonSanitizer<TOKEN_COUNT_MAX>::firstCharIsValidJsonArray(char *json)
{
  char first_char = firstChar(json);
  return (first_char == '[');
}

template <size_t TOKEN_COUNT_MAX>
bool JsonSanitizer<TOKEN_COUNT_MAX>::firstCharIsValidJsonObject(char *json)
{
  char first_char = firstChar(json);
  return (first_char == '{');
}

template <size_t TOKEN_COUNT_MAX>
bool JsonSanitizer<TOKEN_COUNT_MAX>::firstCharIsValidJson(char *json)
{
  char first_char = firstChar(json);
  return ((first_char == '[') || (first_char == '{'));
}

template <size_t TOKEN_COUNT_MAX>
char *JsonSanitizer<TOKEN_COUNT_MAX>::skipCStyleComment(char *json)
{
  json += 2;
  for (;;)
  {
    if (json[0] == '\0') return json;
    if (json[0] == '*' && json[1] == '/') return json + 2;
    json++;
  }
}

template <size_t TOKEN_COUNT_MAX>
char *JsonSanitizer<TOKEN_COUNT_MAX>::skipCppStyleComment(char *json)
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

template <size_t TOKEN_COUNT_MAX>
char *JsonSanitizer<TOKEN_COUNT_MAX>::skipSpacesAndComments(char *json)
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

template <size_t TOKEN_COUNT_MAX>
char JsonSanitizer<TOKEN_COUNT_MAX>::firstChar(char* json)
{
  json = skipSpacesAndComments(json);
  return json[0];
}

template <size_t TOKEN_COUNT_MAX>
bool JsonSanitizer<TOKEN_COUNT_MAX>::colonInFirstTwoWords(char* json)
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

template <size_t TOKEN_COUNT_MAX>
bool JsonSanitizer<TOKEN_COUNT_MAX>::isInRange(char c, char min, char max)
{
  return min <= c && c <= max;
}

template <size_t TOKEN_COUNT_MAX>
bool JsonSanitizer<TOKEN_COUNT_MAX>::isNumber(char c)
{
  return isInRange(c, '0', '9') || c == '-' || c == '.';
}

template <size_t TOKEN_COUNT_MAX>
void JsonSanitizer<TOKEN_COUNT_MAX>::writePrimative(char *json, char * source, JsmnStream::jsmntok_t *t)
{
  size_t str_len = t->end - t->start;
  memcpy(json+json_index_,source+t->start,str_len);
  json_index_ += str_len;
}

template <size_t TOKEN_COUNT_MAX>
void JsonSanitizer<TOKEN_COUNT_MAX>::writeString(char *json, char *source, JsmnStream::jsmntok_t *t)
{
  size_t str_len = t->end - t->start;
  json[json_index_] = '\"';
  json[json_index_+str_len+1] = '\"';
  memcpy(json+json_index_+1,source+t->start,str_len);
  json_index_ += str_len+2;
}

template <size_t TOKEN_COUNT_MAX>
size_t JsonSanitizer<TOKEN_COUNT_MAX>::writeTokensToJson(char *json, char *source, JsmnStream::jsmntok_t *t, size_t count)
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
#endif
