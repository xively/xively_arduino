
#ifndef XIVELYCLIENT_H
#define XIVELYCLIENT_H

#include <Client.h>
#include <XivelyFeed.h>

class XivelyClient
{
public:
  XivelyClient(Client& aClient);

  int get(XivelyFeed& aFeed, const char* aApiKey);
  int put(XivelyFeed& aFeed, const char* aApiKey);

protected:
  static const int kCalculateDataLength =0;
  static const int kSendData =1;
  void buildPath(char* aDest, unsigned long aFeedId, const char* aFormat);

  Client& _client;
};

#endif
