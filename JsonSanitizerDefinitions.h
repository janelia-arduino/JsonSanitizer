// ----------------------------------------------------------------------------
// JsonSanitizerDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef JSON_SANITIZER_DEFINITIONS_H
#define JSON_SANITIZER_DEFINITIONS_H


template <size_t TOKEN_COUNT, size_t BUFFER_SIZE>
void JsonSanitizer::sanitizeBuffer(char (&json_buffer)[BUFFER_SIZE])
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

  JsmnStream::jsmntok_t tokens[TOKEN_COUNT];
  JsmnStream jsmn_stream(tokens);

  int ret;
  ret = jsmn_stream.parseJson(source);
  ret = jsmn_stream.checkParse();
  if (ret < 0) return;
  if (tokens->type == JsmnStream::JSMN_PRIMITIVE) return;
  int tokens_printed_total = 0;
  int token_count = jsmn_stream.getTokenCount();
  size_t count = token_count;
  json_index_ = 0;
  json_buffer[json_index_] = 0;
  while (tokens_printed_total < token_count)
  {
    size_t tokens_printed = writeTokensToJson(json_buffer,source,tokens,count);
    count -= tokens_printed;
    tokens_printed_total += tokens_printed;
  }
  json_buffer[json_index_] = 0;
}

#endif
