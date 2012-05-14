
#ifndef COSMCLIENT_H
#define COSMCLIENT_H

#include <Client.h>
#include <CosmFeed.h>

class CosmClient
{
public:
  CosmClient(Client& aClient);

  int get(CosmFeed& aFeed, const char* aApiKey);
  int put(CosmFeed& aFeed, const char* aApiKey);

protected:
  static const int kCalculateDataLength =0;
  static const int kSendData =1;
  void buildPath(char* aDest, unsigned long aFeedId, const char* aFormat);

  Client& _client;
};

#endif
