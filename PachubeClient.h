
#ifndef PACHUBECLIENT_H
#define PACHUBECLIENT_H

#include <Client.h>
#include <PachubeFeed.h>

class PachubeClient
{
public:
  PachubeClient(Client& aClient);

  int get(PachubeFeed& aFeed, const char* aApiKey);
  int put(PachubeFeed& aFeed, const char* aApiKey);

protected:
  static const int kCalculateDataLength =0;
  static const int kSendData =1;
  void buildPath(char* aDest, unsigned long aFeedId, const char* aFormat);

  Client& _client;
};

#endif
