
#ifndef XIVELY_FEED_H
#define XIVELY_FEED_H

#include <Client.h>
#include <XivelyDatastream.h>
#include <Printable.h>

class XivelyFeed : public Printable
{
public:
  XivelyFeed(unsigned long aID, XivelyDatastream* aDatastreams, int aDatastreamsCount);

  virtual size_t printTo(Print&) const;
  unsigned long id() { return _id; };
  int size() { return _datastreamsCount; };
  XivelyDatastream& operator[] (unsigned i) { return _datastreams[i]; };
protected:
  unsigned long _id;
  XivelyDatastream* _datastreams;
  int _datastreamsCount;
};

#endif

