#include <Arduino.h>
#include <JsmnStream.h>
#include <JsonSanitizer.h>
#include <Streaming.h>


const long BAUD = 115200;
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

void setup()
{
  Serial.begin(BAUD);
}


void loop()
{
  Serial << "json_valid:" << endl;
  Serial << json_valid << endl;
  strcpy(buffer,json_valid);
  sanitizer.sanitizeBuffer(buffer);
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_partial_valid:" << endl;
  Serial << json_partial_valid << endl;
  strcpy(buffer,json_partial_valid);
  sanitizer.sanitizeBuffer(buffer);
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitized_array:" << endl;
  Serial << json_unsanitized_array << endl;
  strcpy(buffer,json_unsanitized_array);
  sanitizer.sanitizeBuffer(buffer);
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitized_object:" << endl;
  Serial << json_unsanitized_object << endl;
  strcpy(buffer,json_unsanitized_object);
  sanitizer.sanitizeBuffer(buffer);
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitizable_object:" << endl;
  Serial << json_unsanitizable_object << endl;
  strcpy(buffer,json_unsanitizable_object);
  sanitizer.sanitizeBuffer(buffer);
  Serial << "unsanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_partial_valid_with_cpp_comments:" << endl;
  Serial << json_partial_valid_with_cpp_comments << endl;
  strcpy(buffer,json_partial_valid_with_cpp_comments);
  sanitizer.sanitizeBuffer(buffer);
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitized_with_c_comments:" << endl;
  Serial << json_unsanitized_with_c_comments << endl;
  strcpy(buffer,json_unsanitized_with_c_comments);
  sanitizer.sanitizeBuffer(buffer);
  Serial << "sanitized:" << endl;
  Serial << buffer << endl;
  Serial << endl;
  delay(1000);

}
