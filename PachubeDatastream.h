#ifndef PACHUBE_DATASTREAM_H
#define PACHUBE_DATASTREAM_H

#include <Print.h>
#include <Printable.h>

class Datastream : public Printable {
public:
  enum DataType { eString, eInt, eFloat };
  virtual DataType getDataType() const =0;
  virtual void value(int aValue) =0;
  virtual void value(float aValue) =0;
  virtual void value(String aValue) =0;
};

class DatastreamBufferInt : public Datastream {
public:
  DatastreamBufferInt(char* aId, int aIdLength, int aValue);
  DatastreamBufferInt(char* aId, int aIdLength);
  virtual DataType getDataType() const { return eInt; };
  virtual size_t printTo(Print&) const;
  int value() const { return _value; };
  virtual void value(int aValue) { _value = aValue; };
  virtual void value(float aValue) { _value = aValue; };
  virtual void value(String aValue) { /* Can't do anything with a string */ };

  char* _id;
  int _idLength;
  int _value;
};

class DatastreamStringString : public Datastream {
public:
  DatastreamStringString(String aId, String aValue);
  DatastreamStringString();
  virtual DataType getDataType() const { return eString; };
  virtual size_t printTo(Print&) const;
  String value() const { return _value; };
  virtual void value(int aValue) { _value = ""; _value += aValue; };
  virtual void value(float aValue) { /* can't convert from a float */ };
  virtual void value(String aValue) { _value = aValue; };
  
  String _id;
  String _value;
};

#endif
