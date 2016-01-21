#include "Arduino.h"
#include "JsmnStream.h"
#include "JsonSanitizer.h"
#include "Streaming.h"


const unsigned int BAUDRATE = 9600;
char json_valid[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
char json_partial_valid[] = "{\"sensor\":gps,test:null,truth:true,method:?,\"time\":1351824120,\"data\":[48.756080 2.302038]}";
char json_unsanitized_array[] = "  ? test, 1  ??   null true [1,2 3] \"nice string\" weird_\\\"string";
char json_unsanitized_object[] = "  server : example.com, post: 80, message: \"hello world\"";
char json_unsanitizable_object[] = "  server : example.com post: 80 message: \"hello world\"";
char json_partial_valid_with_cpp_comments[] = "// this is a c++ style comment {\"sensor\":gps,test:null,truth:true,method:?,\"time\":1351824120,\"data\":[48.756080 2.302038]}";
char json_unsanitized_with_c_comments[] = "/* this is a c style comment */ ? test, 1  ??   null true [1,2 3] \"nice string\" weird_\\\"string";
const unsigned int BUFFER_SIZE = 257;
char buffer[BUFFER_SIZE];
const unsigned int TOKEN_COUNT_MAX = 32;
JsonSanitizer<TOKEN_COUNT_MAX> sanitizer;
char *json;

void setup()
{
  Serial.begin(BAUDRATE);
}


void loop()
{
  Serial << "json_valid:" << endl;
  Serial << json_valid << endl;
  json = json_valid;
  sanitizer.reset();
  for (int i=0; i<strlen(json); ++i)
  {
    const char c = json[i];
    sanitizer.sanitizeCharIntoBuffer(c,buffer);
  }
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_partial_valid:" << endl;
  Serial << json_partial_valid << endl;
  json = json_partial_valid;
  sanitizer.reset();
  for (int i=0; i<strlen(json); ++i)
  {
    const char c = json[i];
    sanitizer.sanitizeCharIntoBuffer(c,buffer);
  }
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitized_array:" << endl;
  Serial << json_unsanitized_array << endl;
  json = json_unsanitized_array;
  sanitizer.reset();
  for (int i=0; i<strlen(json); ++i)
  {
    const char c = json[i];
    sanitizer.sanitizeCharIntoBuffer(c,buffer);
  }
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitized_object:" << endl;
  Serial << json_unsanitized_object << endl;
  json = json_unsanitized_object;
  sanitizer.reset();
  for (int i=0; i<strlen(json); ++i)
  {
    const char c = json[i];
    sanitizer.sanitizeCharIntoBuffer(c,buffer);
  }
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitizable_object:" << endl;
  Serial << json_unsanitizable_object << endl;
  json = json_unsanitizable_object;
  sanitizer.reset();
  for (int i=0; i<strlen(json); ++i)
  {
    const char c = json[i];
    sanitizer.sanitizeCharIntoBuffer(c,buffer);
  }
  Serial << "unsanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_partial_valid_with_cpp_comments:" << endl;
  Serial << json_partial_valid_with_cpp_comments << endl;
  json = json_partial_valid_with_cpp_comments;
  sanitizer.reset();
  for (int i=0; i<strlen(json); ++i)
  {
    const char c = json[i];
    sanitizer.sanitizeCharIntoBuffer(c,buffer);
  }
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitized_with_c_comments:" << endl;
  Serial << json_unsanitized_with_c_comments << endl;
  json = json_unsanitized_with_c_comments;
  sanitizer.reset();
  for (int i=0; i<strlen(json); ++i)
  {
    const char c = json[i];
    sanitizer.sanitizeCharIntoBuffer(c,buffer);
  }
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

}
