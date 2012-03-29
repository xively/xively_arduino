
#ifndef PACHUBE_FEED_H
#define PACHUBE_FEED_H

#include <Client.h>
#include <PachubeDatastream.h>

class PachubeFeed
{
public:
  PachubeFeed(char* aID, Client& aClient, Datastream* aDatastreams, int aDatastreamsCount);

  int put(const char* aApiKey);
protected:
  static const int kCalculateDataLength =0;
  static const int kSendData =1;

  //unsigned long _id;
  // FIXME Need to pick right sizes, not have magic numbers here
  char _id[15];
  Client& _client;
  Datastream* _datastreams;
  int _datastreamsCount;
};

#endif

