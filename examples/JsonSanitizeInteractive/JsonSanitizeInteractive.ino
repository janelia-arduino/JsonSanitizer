#include "JsonSanitizer.h"
#include "Streaming.h"

namespace constants
{
enum{STRING_LENGTH_REQUEST=257};
const char eol = '\n';
const unsigned int TOKEN_COUNT = 32;
}

char request[constants::STRING_LENGTH_REQUEST];

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  JsonSanitizer sanitizer(constants::TOKEN_COUNT);

  while (Serial.available() > 0)
  {
    int request_length = Serial.readBytesUntil(constants::eol,request,constants::STRING_LENGTH_REQUEST);
    if (request_length == 0)
    {
      continue;
    }
    request[request_length] = 0;
    Serial << "unsanitized json:" << endl;
    Serial << request << endl;

    sanitizer.sanitize(request,constants::STRING_LENGTH_REQUEST);
    Serial << "sanitized json:" << endl;
    Serial << request << endl << endl;
  }
}
