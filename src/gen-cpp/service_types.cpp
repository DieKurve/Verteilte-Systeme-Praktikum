/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "service_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace serviceProvider {


sensorData::~sensorData() noexcept {
}


void sensorData::__set_id(const int32_t val) {
  this->id = val;
}

void sensorData::__set_value(const int32_t val) {
  this->value = val;
}

void sensorData::__set_timestamp(const std::string& val) {
  this->timestamp = val;
}
std::ostream& operator<<(std::ostream& out, const sensorData& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t sensorData::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->id);
          this->__isset.id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->value);
          this->__isset.value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->timestamp);
          this->__isset.timestamp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t sensorData::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("sensorData");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("timestamp", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->timestamp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(sensorData &a, sensorData &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.value, b.value);
  swap(a.timestamp, b.timestamp);
  swap(a.__isset, b.__isset);
}

sensorData::sensorData(const sensorData& other0) {
  id = other0.id;
  value = other0.value;
  timestamp = other0.timestamp;
  __isset = other0.__isset;
}
sensorData& sensorData::operator=(const sensorData& other1) {
  id = other1.id;
  value = other1.value;
  timestamp = other1.timestamp;
  __isset = other1.__isset;
  return *this;
}
void sensorData::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "sensorData(";
  out << "id=" << to_string(id);
  out << ", " << "value=" << to_string(value);
  out << ", " << "timestamp=" << to_string(timestamp);
  out << ")";
}

} // namespace
