#include <Pachube.h>
#include <HttpClient.h>
#include <CountingStream.h>

PachubeFeed::PachubeFeed(char* aID, Client& aClient, Datastream* aDatastreams, int aDatastreamsCount)
  : /*_id(aID),*/ _client(aClient), _datastreams(aDatastreams), _datastreamsCount(aDatastreamsCount)
{
  strcpy(_id, aID);
}

int PachubeFeed::put(const char* aApiKey)
{
  HttpClient http(_client);
  char path[30];
  strcpy(path, "/v2/feeds/");
  strcat(path, _id);
  strcat(path, ".json");
  
  http.beginRequest();
  int ret = http.put("api.pachube.com", path);
  if (ret == 0)
  {
    http.sendHeader("X-PachubeApiKey", aApiKey);

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
      len += s->println("{");
      len += s->println("\"version\":\"1.0.0\",");
      len += s->println("\"datastreams\" : [");
      for (int j =0; j < _datastreamsCount; j++)
      {
        len += s->print("{ ");
        len += s->print(_datastreams[j]);
        len += s->print(" }");
        if (j == _datastreamsCount-1)
        {
          // Last time through
          len += s->println();
        }
        else
        {
          len += s->println(",");
        }
      }
      len += s->println("]");
      len += s->println("}");
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
    http.stop();
  }
  return ret;
}


