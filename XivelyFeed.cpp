#include <Xively.h>

XivelyFeed::XivelyFeed(unsigned long aID, XivelyDatastream* aDatastreams, int aDatastreamsCount)
  : _id(aID), _datastreams(aDatastreams), _datastreamsCount(aDatastreamsCount)
{
  //strcpy(_id, aID);
}

size_t XivelyFeed::printTo(Print& aPrint) const
{
  int len = 0;
  len += aPrint.println("{");
  len += aPrint.println("\"version\":\"1.0.0\",");
  len += aPrint.println("\"datastreams\" : [");
  for (int j =0; j < _datastreamsCount; j++)
  {
    len += aPrint.print(_datastreams[j]);
    if (j == _datastreamsCount-1)
    {
      // Last time through
      len += aPrint.println();
    }
    else
    {
      len += aPrint.println(",");
    }
  }
  len += aPrint.println("]");
  len += aPrint.println("}");
  return len;
}


