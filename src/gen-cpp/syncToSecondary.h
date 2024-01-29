/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef syncToSecondary_H
#define syncToSecondary_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "serviceSync_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class syncToSecondaryIf {
 public:
  virtual ~syncToSecondaryIf() {}
  virtual void sync(const std::vector<syncData> & data) = 0;
};

class syncToSecondaryIfFactory {
 public:
  typedef syncToSecondaryIf Handler;

  virtual ~syncToSecondaryIfFactory() {}

  virtual syncToSecondaryIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(syncToSecondaryIf* /* handler */) = 0;
};

class syncToSecondaryIfSingletonFactory : virtual public syncToSecondaryIfFactory {
 public:
  syncToSecondaryIfSingletonFactory(const ::std::shared_ptr<syncToSecondaryIf>& iface) : iface_(iface) {}
  virtual ~syncToSecondaryIfSingletonFactory() {}

  virtual syncToSecondaryIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(syncToSecondaryIf* /* handler */) {}

 protected:
  ::std::shared_ptr<syncToSecondaryIf> iface_;
};

class syncToSecondaryNull : virtual public syncToSecondaryIf {
 public:
  virtual ~syncToSecondaryNull() {}
  void sync(const std::vector<syncData> & /* data */) {
    return;
  }
};

typedef struct _syncToSecondary_sync_args__isset {
  _syncToSecondary_sync_args__isset() : data(false) {}
  bool data :1;
} _syncToSecondary_sync_args__isset;

class syncToSecondary_sync_args {
 public:

  syncToSecondary_sync_args(const syncToSecondary_sync_args&);
  syncToSecondary_sync_args& operator=(const syncToSecondary_sync_args&);
  syncToSecondary_sync_args() {
  }

  virtual ~syncToSecondary_sync_args() noexcept;
  std::vector<syncData>  data;

  _syncToSecondary_sync_args__isset __isset;

  void __set_data(const std::vector<syncData> & val);

  bool operator == (const syncToSecondary_sync_args & rhs) const
  {
    if (!(data == rhs.data))
      return false;
    return true;
  }
  bool operator != (const syncToSecondary_sync_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const syncToSecondary_sync_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class syncToSecondary_sync_pargs {
 public:


  virtual ~syncToSecondary_sync_pargs() noexcept;
  const std::vector<syncData> * data;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class syncToSecondary_sync_result {
 public:

  syncToSecondary_sync_result(const syncToSecondary_sync_result&);
  syncToSecondary_sync_result& operator=(const syncToSecondary_sync_result&);
  syncToSecondary_sync_result() {
  }

  virtual ~syncToSecondary_sync_result() noexcept;

  bool operator == (const syncToSecondary_sync_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const syncToSecondary_sync_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const syncToSecondary_sync_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class syncToSecondary_sync_presult {
 public:


  virtual ~syncToSecondary_sync_presult() noexcept;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class syncToSecondaryClient : virtual public syncToSecondaryIf {
 public:
  syncToSecondaryClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  syncToSecondaryClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void sync(const std::vector<syncData> & data);
  void send_sync(const std::vector<syncData> & data);
  void recv_sync();
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class syncToSecondaryProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<syncToSecondaryIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (syncToSecondaryProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_sync(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  syncToSecondaryProcessor(::std::shared_ptr<syncToSecondaryIf> iface) :
    iface_(iface) {
    processMap_["sync"] = &syncToSecondaryProcessor::process_sync;
  }

  virtual ~syncToSecondaryProcessor() {}
};

class syncToSecondaryProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  syncToSecondaryProcessorFactory(const ::std::shared_ptr< syncToSecondaryIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::std::shared_ptr< syncToSecondaryIfFactory > handlerFactory_;
};

class syncToSecondaryMultiface : virtual public syncToSecondaryIf {
 public:
  syncToSecondaryMultiface(std::vector<std::shared_ptr<syncToSecondaryIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~syncToSecondaryMultiface() {}
 protected:
  std::vector<std::shared_ptr<syncToSecondaryIf> > ifaces_;
  syncToSecondaryMultiface() {}
  void add(::std::shared_ptr<syncToSecondaryIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void sync(const std::vector<syncData> & data) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sync(data);
    }
    ifaces_[i]->sync(data);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class syncToSecondaryConcurrentClient : virtual public syncToSecondaryIf {
 public:
  syncToSecondaryConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  syncToSecondaryConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void sync(const std::vector<syncData> & data);
  int32_t send_sync(const std::vector<syncData> & data);
  void recv_sync(const int32_t seqid);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif



#endif
