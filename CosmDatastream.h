#ifndef COSM_DATASTREAM_H
#define COSM_DATASTREAM_H

#include <Stream.h>
#include <Printable.h>

#define DATASTREAM_STRING 0
#define DATASTREAM_BUFFER 1
#define DATASTREAM_INT 2
#define DATASTREAM_FLOAT 3

class CosmDatastream : public Printable {
  friend class CosmClient;

  typedef struct {
    char* _buffer;
    int _bufferSize;
  } tBuffer;
public:

  CosmDatastream(String& aId, int aType);
  CosmDatastream(char* aIdBuffer, int aIdBufferLength, int aType);
  CosmDatastream(char* aIdBuffer, int aIdBufferLength, int aType, char* aValueBuffer, int aValueBufferLength);
  int updateValue(Stream& aStream);
  void setInt(int aValue);
  void setFloat(float aValue);
  void setString(String& aValue);
  void setBuffer(const char* aValue);
  String& getString();
  int getInt();
  float getFloat();
  char* getBuffer();
  virtual size_t printTo(Print&) const;
protected:
  int idLength() { return (_idType == DATASTREAM_STRING ? _idString.length() : strlen(_idBuffer._buffer)); };
  char idChar(int idx) { return (_idType == DATASTREAM_STRING ? _idString[idx] : (idx > strlen(_idBuffer._buffer) ? '\0' : _idBuffer._buffer[idx])); };
  // FIXME Only needed until readStringUntil is available in core
  int timedRead(Stream& aStream);

  int _idType;
  String _idString;
  tBuffer _idBuffer;
  int _valueType;
  String _valueString;
  union {
    tBuffer _valueBuffer;
    int _valueInt;
    float _valueFloat;
  } _value;
};

#endif
