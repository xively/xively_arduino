#include <XivelyDatastream.h>
// FIXME Only needed until readStringUntil is available in Stream
#include <Arduino.h>

XivelyDatastream::XivelyDatastream(String& aId, int aType)
  : _idType(DATASTREAM_STRING), _valueType(aType), _idString(aId)
{
}

XivelyDatastream::XivelyDatastream(char* aIdBuffer, int aIdBufferSize, int aType)
  : _idType(DATASTREAM_BUFFER), _valueType(aType), _idString(), _valueString()
{
  _idBuffer._buffer = aIdBuffer;
  _idBuffer._bufferSize = aIdBufferSize;
}

XivelyDatastream::XivelyDatastream(char* aIdBuffer, int aIdBufferSize, int aType, char* aValueBuffer, int aValueBufferSize)
  : _idType(DATASTREAM_BUFFER), _valueType(aType)
{
  _idBuffer._buffer = aIdBuffer;
  _idBuffer._bufferSize = aIdBufferSize;
  _value._valueBuffer._buffer = aValueBuffer;
  _value._valueBuffer._bufferSize = aValueBufferSize;
}

int XivelyDatastream::updateValue(Stream& aStream)
{
  switch (_valueType)
  {
  case DATASTREAM_INT:
    _value._valueInt = aStream.parseInt();
    break;
  case DATASTREAM_FLOAT:
    _value._valueFloat = aStream.parseFloat();
    break;
  case DATASTREAM_BUFFER:
    {
      int len = aStream.readBytesUntil('\n', _value._valueBuffer._buffer, _value._valueBuffer._bufferSize);
      _value._valueBuffer._buffer[len] = '\0';
    }
    break;
  case DATASTREAM_STRING:
    // FIXME Change this to use readStringUntil once that's in the core
    // FIMXE and remove the timedRead method in here then too
    _valueString = "";
    int c = timedRead(aStream);
    while (c >= 0 && c != '\n')
    {
      _valueString += (char)c;
      c = timedRead(aStream);
    }
    break;
  };
}

int XivelyDatastream::timedRead(Stream& aStream)
{
  int c;
  long _startMillis = millis();
  do {
    c = aStream.read();
    if (c >= 0) return c;
  } while(millis() - _startMillis < 10000UL);
  return -1;     // -1 indicates timeout
}


void XivelyDatastream::setInt(int aValue)
{
  if (_valueType == DATASTREAM_INT)
  {
    _value._valueInt = aValue;
  }
}

void XivelyDatastream::setFloat(float aValue)
{
  if (_valueType == DATASTREAM_FLOAT)
  {
    _value._valueFloat = aValue;
  }
}

void XivelyDatastream::setString(String& aValue)
{
  if (_valueType == DATASTREAM_STRING)
  {
    _valueString = aValue;
  }
}

void XivelyDatastream::setBuffer(const char* aBuffer)
{
  if (_valueType == DATASTREAM_BUFFER)
  {
    strncpy(_value._valueBuffer._buffer, aBuffer, _value._valueBuffer._bufferSize);
  }
}

int XivelyDatastream::getInt()
{
  if (_valueType == DATASTREAM_INT)
  {
    return _value._valueInt;
  }
  else
  {
    return 0;
  }
}

float XivelyDatastream::getFloat()
{
  if (_valueType == DATASTREAM_FLOAT)
  {
    return _value._valueFloat;
  }
  else
  {
    return 0.0;
  }
}

String& XivelyDatastream::getString()
{
  return _valueString;
}

char* XivelyDatastream::getBuffer()
{
  if (_valueType == DATASTREAM_BUFFER)
  {
    return _value._valueBuffer._buffer;
  }
  else
  {
    return NULL;
  }
}

size_t XivelyDatastream::printTo(Print& aPrint) const
{
  size_t count =0;
  count += aPrint.print("{ \"id\" : \"");
  if (_idType == DATASTREAM_STRING)
  {
    count += aPrint.print(_idString);
  }
  else
  {
    count += aPrint.print(_idBuffer._buffer);
  }
  count += aPrint.print("\", \"current_value\" : \"");
  switch (_valueType)
  {
  case DATASTREAM_STRING:
    count += aPrint.print(_valueString);
    break;
  case DATASTREAM_BUFFER:
    count += aPrint.print(_value._valueBuffer._buffer);
    break;
  case DATASTREAM_INT:
    count += aPrint.print(_value._valueInt);
    break;
  case DATASTREAM_FLOAT:
    count += aPrint.print(_value._valueFloat);
    break;
  };
  count += aPrint.print("\" }");
  return count;
}

#if 0
DatastreamBufferInt::DatastreamBufferInt(char* aId, int aIdLength, int aValue)
  : _id(aId), _idLength(aIdLength), _value(aValue)
{};

size_t DatastreamBufferInt::printTo(Print& aPrint) const
{
  size_t count =0;
  count += aPrint.print("\"id\" : \"");
  count += aPrint.print(_id);
  count += aPrint.print("\", \"current_value\" : \"");
  count += aPrint.print(_value);
  count += aPrint.print("\"");
  return count;
}

DatastreamStringString::DatastreamStringString(String aId, String aValue)
  : _id(aId), _value(aValue)
{};

size_t DatastreamStringString::printTo(Print& aPrint) const
{
  // Output the datastream in JSON
  size_t count =0;
  count += aPrint.print("\"id\" : \"");
  count += aPrint.print(_id);
  count += aPrint.print("\", \"current_value\" : \"");
  count += aPrint.print(_value);
  count += aPrint.print("\"");
  return count;
}
#endif
