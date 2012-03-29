#include <PachubeDatastream.h>

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

