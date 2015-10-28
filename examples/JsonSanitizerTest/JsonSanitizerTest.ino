#include "Arduino.h"
#include "Streaming.h"
#include "JsonSanitizer.h"


const unsigned int BAUDRATE = 9600;
// char json_valid[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
char json_valid[] = "{\"sensor\":gps,test:null,truth:true,method:?,\"time\":1351824120,\"data\":[48.756080,2.302038]}";
char json_unsanitized_array[] = "  ? test 1  ??   null true [1,2,3]";
char json_unsanitized_object[] = "  server : example.com post: 80 message: \"hello world\"";
char json_valid_request[] = "[\"setSerialNumber\",\"serial_number\",\"?\"]";


void setup()
{
  Serial.begin(BAUDRATE);
}


void loop()
{
  JsonSanitizer sanitizer;

  // Serial << "json_unsanitized_array: " << json_unsanitized_array << endl;
  // char first_char = sanitizer.firstChar(json_unsanitized_array);
  // Serial << "first_char: " << first_char << endl;
  // char * json_sanitized_array = sanitizer.skipSpacesAndComments(json_unsanitized_array);
  // Serial << "json_sanitzed_array: " << json_sanitized_array << endl;
  // delay(1000);

  Serial << "json_valid: " << json_valid << endl;
  char request0[strlen(json_valid) + 1];
  strcpy(request0,json_valid);
  sanitizer.sanitize(request0);
  Serial << "sanitized: " << request0 << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitized_array: " << json_unsanitized_array << endl;
  char request1[strlen(json_unsanitized_array) + 1];
  strcpy(request1,json_unsanitized_array);
  sanitizer.sanitize(request1);
  Serial << "sanitized: " << request1 << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_unsanitized_object: " << json_unsanitized_object << endl;
  char request2[strlen(json_unsanitized_object) + 1];
  strcpy(request2,json_unsanitized_object);
  sanitizer.sanitize(request2);
  Serial << "sanitized: " << request2 << endl;
  Serial << endl;
  delay(1000);

  Serial << "json_valid_request: " << json_valid_request << endl;
  char request3[strlen(json_valid_request) + 1];
  strcpy(request3,json_valid_request);
  sanitizer.sanitize(request3);
  Serial << "sanitized: " << request3 << endl;
  Serial << endl;
  delay(1000);
}
