
#ifndef COSM_FEED_H
#define COSM_FEED_H

#include <Client.h>
#include <CosmDatastream.h>
#include <Printable.h>

class CosmFeed : public Printable
{
public:
  CosmFeed(unsigned long aID, CosmDatastream* aDatastreams, int aDatastreamsCount);

  virtual size_t printTo(Print&) const;
  unsigned long id() { return _id; };
  int size() { return _datastreamsCount; };
  CosmDatastream& operator[] (unsigned i) { return _datastreams[i]; };
protected:
  unsigned long _id;
  CosmDatastream* _datastreams;
  int _datastreamsCount;
};

#endif

