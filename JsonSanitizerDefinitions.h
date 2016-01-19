// ----------------------------------------------------------------------------
// JsonSanitizerDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef JSON_SANITIZER_DEFINITIONS_H
#define JSON_SANITIZER_DEFINITIONS_H


template <size_t BUFFER_SIZE>
void JsonSanitizer::sanitizeBuffer(char (&json)[BUFFER_SIZE])
{
  // Make sure json is proper null terminated string
  size_t json_length = strlen(json);
  if (json_length >= BUFFER_SIZE) return;

  // Trim initial whitespace
  char *json_trimmed = skipSpacesAndComments(json);
  json_length = strlen(json_trimmed);

  // Make intermediate buffer. todo: possible to remove?
  char source[BUFFER_SIZE];

  // Fill source, add [] or {} if necessary
  if (firstCharIsValidJson(json))
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

  int r;
  jsmn_parser p;
  jsmntok_t tok[TOKEN_COUNT_];

  jsmn_init(&p);
  r = jsmn_parse(&p, source, json_length, tok, sizeof(tok)/sizeof(tok[0]));
  if (r < 0) return;
  if (tok->type == JSMN_PRIMITIVE) return;
  int tokens_printed_total = 0;
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

#endif
