
#ifndef PACHUBE_FEED_H
#define PACHUBE_FEED_H

#include <Client.h>
#include <PachubeDatastream.h>
#include <Printable.h>

class PachubeFeed : public Printable
{
public:
  PachubeFeed(unsigned long aID, Datastream* aDatastreams, int aDatastreamsCount);

  virtual size_t printTo(Print&) const;
  unsigned long id() { return _id; };
  int size() { return _datastreamsCount; };
  Datastream& operator[] (unsigned i) { return _datastreams[i]; };
protected:
  unsigned long _id;
  // FIXME Need to pick right sizes, not have magic numbers here
  //char _id[15];
  Datastream* _datastreams;
  int _datastreamsCount;
};

#endif

