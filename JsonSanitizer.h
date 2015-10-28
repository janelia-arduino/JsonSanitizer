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
#include "Streaming.h"

#include "utility/jsmn.h"


class JsonSanitizer
{
public:
  void sanitize(char *json);
private:
  char *skipCStyleComment(char *ptr);
  char *skipCppStyleComment(char *ptr);
  char *skipSpacesAndComments(char *ptr);
  char firstChar(char *ptr);
  bool colonInFirstTwoWords(char* ptr)
  int dump(const char *js, jsmntok_t *t, size_t count, int indent);
};
#endif
