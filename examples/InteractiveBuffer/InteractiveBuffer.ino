#include <JsmnStream.h>
#include <JsonSanitizer.h>
#include <ArduinoJson.h>
#include <Streaming.h>

namespace constants
{
enum{STRING_LENGTH_REQUEST=257};
const char eol = '\n';
const unsigned int TOKEN_COUNT_MAX = 32;
}

char request[constants::STRING_LENGTH_REQUEST];
JsonSanitizer<constants::TOKEN_COUNT_MAX> sanitizer;

void setup()
{
  Serial.begin(115200);
}

void loop() {
  while (Serial.available() > 0)
  {
    int request_length = Serial.readBytesUntil(constants::eol,request,constants::STRING_LENGTH_REQUEST);
    if (request_length == 0)
    {
      continue;
    }
    request[request_length] = 0;
    Serial << "received:" << endl;
    Serial << request << endl;
    // Must sanitize all requests, even though it adds delay for valid requests
    // ArduinoJson modifies request so cannot use for testing without making copy
    // If making copy, might as well sanitize
    sanitizer.reset();
    sanitizer.sanitizeBuffer(request);
    Serial << "sanitized:" << endl;
    Serial << request << endl;
    if (sanitizer.firstCharIsValidJsonObject(request))
    {
      StaticJsonDocument<constants::STRING_LENGTH_REQUEST> json_document;
      DeserializationError error = deserializeJson(json_document,request);
      if (!error)
      {
        Serial << "ArduinoJson successfully parsed object!" << endl;
      }
      else
      {
        Serial << "ArduinoJson unable to parse sanitized object! :(" << endl;
      }
    }
    if (sanitizer.firstCharIsValidJsonArray(request))
    {
      StaticJsonDocument<constants::STRING_LENGTH_REQUEST> json_document;
      DeserializationError error = deserializeJson(json_document,request);
      if (!error)
      {
        Serial << "ArduinoJson successfully parsed array!" << endl;
      }
      else
      {
        Serial << "ArduinoJson unable to parse sanitized array! :(" << endl;
      }
    }
    Serial << endl;
  }
}
