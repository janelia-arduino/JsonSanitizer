// ----------------------------------------------------------------------------
// JsonSanitizer.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _JSON_SANITIZER_H_
#define _JSON_SANITIZER_H_
#include <Arduino.h>
#include <JsmnStream.h>


template <size_t TOKEN_COUNT_MAX>
class JsonSanitizer
{
public:
  enum States
    {
      OUTSIDE_JSON,
      INSIDE_FORWARD_SLASH,
      INSIDE_C_COMMENT,
      INSIDE_CPP_COMMENT,
      INSIDE_C_COMMENT_ASTERISK,
      INSIDE_JSON,
      INSIDE_UNKNOWN_JSON_STRING,
    };
  JsonSanitizer();
  void reset();
  template <size_t BUFFER_SIZE>
  void sanitizeCharIntoBuffer(const char c, char (&buffer)[BUFFER_SIZE]);
  // template <size_t BUFFER_SIZE>
  // void terminateBuffer(char (&buffer)[BUFFER_SIZE]);
  template <size_t BUFFER_SIZE>
  void sanitizeBuffer(char (&json_buffer)[BUFFER_SIZE]);
  bool firstCharIsValidJsonArray(char *json);
  bool firstCharIsValidJsonObject(char *json);
  bool firstCharIsValidJson(char *json);
private:
  JsmnStream::jsmntok_t tokens_[TOKEN_COUNT_MAX];
  JsmnStream jsmn_stream_;
  int parse_result_;
  unsigned int buffer_pos_;
  States state_;
  char *skipCStyleComment(char *json);
  char *skipCppStyleComment(char *json);
  char *skipSpacesAndComments(char *json);
  char firstChar(char *json);
  bool colonInFirstTwoWords(char* json);
  bool isInRange(char c, char min, char max);
  bool isNumber(char c);
  void writePrimative(char *json, char *source, JsmnStream::jsmntok_t *t);
  void writeString(char *json, char *source, JsmnStream::jsmntok_t *t);
  size_t writeTokensToJson(char *json, char *source, JsmnStream::jsmntok_t *t, size_t count);
  size_t json_index_;
};
#include "JsonSanitizer/JsonSanitizerDefinitions.h"

#endif
