#include <Xively.h>
#include <HttpClient.h>
#include <CountingStream.h>

XivelyClient::XivelyClient(Client& aClient)
  : _client(aClient)
{
}

int XivelyClient::put(XivelyFeed& aFeed, const char* aApiKey)
{
  HttpClient http(_client);
  char path[30];
  buildPath(path, aFeed.id(), "json");
  http.beginRequest();
  int ret = http.put("api.xively.com", path);
  if (ret == 0)
  {
    http.sendHeader("X-ApiKey", aApiKey);
    http.sendHeader("User-Agent", "Xively-Arduino-Lib/1.0");    

    CountingStream countingStream; // Used to work out how long that data will be
    for (int i =kCalculateDataLength; i <= kSendData; i++)
    {
      Print* s;
      int len =0;
      if (i == kCalculateDataLength)
      {
        s = &countingStream;
      }
      else
      {
        s = &http;
      }
      len = s->print(aFeed);
      if (i == kCalculateDataLength)
      {
        // We now know how long the data will be...
        http.sendHeader("Content-Length", len);
      }
    }
    // Now we're done sending the request
    http.endRequest();

    ret = http.responseStatusCode();
    if ((ret < 200) || (ret > 299))
    {
      // It wasn't a successful response, ensure it's -ve so the error is easy to spot
      if (ret > 0)
      {
        ret = ret * -1;
      }
    }
    http.flush();
    http.stop();
  }
  return ret;
}

void XivelyClient::buildPath(char* aDest, unsigned long aFeedId, const char* aFormat)
{
  char idstr[12]; 
  strcpy(aDest, "/v2/feeds/");
  char* p = &idstr[10];
  idstr[11] = 0;
  for(*p--=aFeedId%10+0x30;aFeedId/=10;*p--=aFeedId%10+0x30);
  strcat(aDest, p+1);
  strcat(aDest, ".");
  strcat(aDest, aFormat);
}

int XivelyClient::get(XivelyFeed& aFeed, const char* aApiKey)
{
  HttpClient http(_client);
  char path[30];
  buildPath(path, aFeed.id(), "csv");
  http.beginRequest();
  int ret = http.get("api.xively.com", path);
  if (ret == 0)
  {
    http.sendHeader("X-ApiKey", aApiKey);
    http.sendHeader("User-Agent", "Xively-Arduino-Lib/1.0");    
    http.endRequest();

    ret = http.responseStatusCode();
    if ((ret < 200) || (ret > 299))
    {
      // It wasn't a successful response, ensure it's -ve so the error is easy to spot
      if (ret > 0)
      {
        ret = ret * -1;
      }
    }
    else
    {
      http.skipResponseHeaders();
      // Now we need to run through each line, looking to see if it matches one
      // of the given datastreams.
      // So that we don't use any more memory than necessary, we'll keep track
      // of which character we're up to in the ID string, and have a bit-field
      // of the remaining datastreams that match.  This limits us (if we use
      // and unsigned long) to 32 datastreams in a feed, but that's probably ok
      int idIdx = 0;
      unsigned long idBitfield = 0;
      for (int i =0; i < aFeed.size(); i++)
      {
        idBitfield |= 1 << i;
      }
      // As long as we've got bitfields to read
// FIXME Need to time out if this hangs for too long
      while ((http.available() || http.connected()))
      {
        if (http.available())
        {
          char next = http.read();
          switch (next)
          {
          case ',':
            // We've reached the end of the ID string, see if it matches any of the
            // datastreams in the feed
            // But first skip the updated time, to get to the value
            http.find(",");
            for (int i =0; i < aFeed.size(); i++)
            {
              if ((idBitfield & 1<<i) && (aFeed[i].idLength() == idIdx))
              {
                // We've found a matching datastream
                // FIXME cope with any errors returned
                aFeed[i].updateValue(http);
                // When we get here we'll be at the end of the line, but if aFeed[i]
                // was a string or buffer type, we'll have consumed the '\n'
                next = '\n';
              }
            }
            // Need to run to the end of the line regardless now
            // And deliberately drop through into the next case
            while ((next != '\r')  && (next != '\n') && (http.available() || http.connected()))
            {
              next = http.read();
            }
          case '\r':
          case '\n':
            // We've hit the end of the line, reset everything
            idIdx = 0;
            for (int i =0; i < aFeed.size(); i++)
            {
              idBitfield |= 1 << i;
            }
            break;
          default:
            // Next character of the ID string
            for (int i =0; i < aFeed.size(); i++)
            {
              if (!(idBitfield & 1<<i) || (aFeed[i].idChar(idIdx) != next))
              {
                idBitfield &= ~(1<<i);
              }
              // else we're still matching
            }
            idIdx++; // onto the next character in the ID
            break;
          };
        }
      }
      delay(10);
    }
    http.stop();
  }
  return ret;
}


