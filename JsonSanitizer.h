// ----------------------------------------------------------------------------
// JsonSanitizer.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _JSON_SANITIZER_H_
#define _JSON_SANITIZER_H_
#include "Arduino.h"
#include "JsmnStream.h"
#include "Streaming.h"


class JsonSanitizer
{
public:
  JsonSanitizer(size_t token_count=32);
  template <size_t BUFFER_SIZE>
  void sanitizeBuffer(char (&json)[BUFFER_SIZE]);
  bool firstCharIsValidJsonArray(char *json);
  bool firstCharIsValidJsonObject(char *json);
  bool firstCharIsValidJson(char *json);
private:
  char *skipCStyleComment(char *json);
  char *skipCppStyleComment(char *json);
  char *skipSpacesAndComments(char *json);
  char firstChar(char *json);
  bool colonInFirstTwoWords(char* json);
  bool isInRange(char c, char min, char max);
  bool isNumber(char c);
  void writePrimative(char *json, char *source, jsmntok_t *t);
  void writeString(char *json, char *source, jsmntok_t *t);
  size_t writeTokensToJson(char *json, char *source, jsmntok_t *t, size_t count);
  const size_t TOKEN_COUNT_;
  size_t json_index_;
};
#include "JsonSanitizerDefinitions.h"

#endif
