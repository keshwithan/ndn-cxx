/**
 * @author: Jeff Thompson
 * See COPYING for copyright and distribution information.
 */

#include <stdexcept>
#include "../c/encoding/BinaryXMLInterest.h"
#include "../c/encoding/binary-xml-data.h"
#include "../Interest.hpp"
#include "../data.hpp"
#include "BinaryXMLEncoder.hpp"
#include "BinaryXMLDecoder.hpp"
#include "BinaryXMLWireFormat.hpp"

using namespace std;

namespace ndn {

BinaryXmlWireFormat BinaryXmlWireFormat::instance_;

ptr_lib::shared_ptr<vector<unsigned char> > BinaryXmlWireFormat::encodeInterest(const Interest &interest) 
{
  struct ndn_NameComponent nameComponents[100];
  struct ndn_ExcludeEntry excludeEntries[100];
  struct ndn_Interest interestStruct;
  ndn_Interest_init
    (&interestStruct, nameComponents, sizeof(nameComponents) / sizeof(nameComponents[0]), 
     excludeEntries, sizeof(excludeEntries) / sizeof(excludeEntries[0]));
  interest.get(interestStruct);

  BinaryXmlEncoder encoder;
  ndn_encodeBinaryXmlInterest(&interestStruct, &encoder);
     
  return encoder.getOutput();
}

void BinaryXmlWireFormat::decodeInterest(Interest &interest, const unsigned char *input, unsigned int inputLength)
{
  struct ndn_NameComponent nameComponents[100];
  struct ndn_ExcludeEntry excludeEntries[100];
  struct ndn_Interest interestStruct;
  ndn_Interest_init
    (&interestStruct, nameComponents, sizeof(nameComponents) / sizeof(nameComponents[0]), 
     excludeEntries, sizeof(excludeEntries) / sizeof(excludeEntries[0]));
    
  BinaryXmlDecoder decoder(input, inputLength);  
  ndn_Error error;
  if (error = ndn_decodeBinaryXmlInterest(&interestStruct, &decoder))
    throw std::runtime_error(ndn_getErrorString(error));

  interest.set(interestStruct);
}

ptr_lib::shared_ptr<vector<unsigned char> > BinaryXmlWireFormat::encodeData(const Data &data) 
{
  struct ndn_NameComponent nameComponents[100];
  struct ndn_Data dataStruct;
  ndn_Data_init
    (&dataStruct, nameComponents, sizeof(nameComponents) / sizeof(nameComponents[0]));
  data.get(dataStruct);

  BinaryXmlEncoder encoder;
  ndn_encodeBinaryXmlData(&dataStruct, &encoder);
     
  return encoder.getOutput();
}

void BinaryXmlWireFormat::decodeData(Data &data, const unsigned char *input, unsigned int inputLength)
{
  struct ndn_NameComponent nameComponents[100];
  struct ndn_Data dataStruct;
  ndn_Data_init
    (&dataStruct, nameComponents, sizeof(nameComponents) / sizeof(nameComponents[0]));
    
  BinaryXmlDecoder decoder(input, inputLength);  
  ndn_Error error;
  if (error = ndn_decodeBinaryXmlData(&dataStruct, &decoder))
    throw std::runtime_error(ndn_getErrorString(error));

  data.set(dataStruct);
}

}
