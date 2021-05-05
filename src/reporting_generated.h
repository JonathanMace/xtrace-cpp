// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_REPORTING_XTRACEFLATBUFFERS_H_
#define FLATBUFFERS_GENERATED_REPORTING_XTRACEFLATBUFFERS_H_

#include "flatbuffers/flatbuffers.h"


namespace xtraceflatbuffers {

struct XTraceReportCustomField;
struct XTraceReportv4;

struct XTraceReportCustomField FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_KEY = 4,
    VT_VALUE = 6
  };
  const flatbuffers::String *key() const { return GetPointer<const flatbuffers::String *>(VT_KEY); }
  const flatbuffers::String *value() const { return GetPointer<const flatbuffers::String *>(VT_VALUE); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_KEY) &&
           verifier.Verify(key()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_VALUE) &&
           verifier.Verify(value()) &&
           verifier.EndTable();
  }
};

struct XTraceReportCustomFieldBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_key(flatbuffers::Offset<flatbuffers::String> key) { fbb_.AddOffset(XTraceReportCustomField::VT_KEY, key); }
  void add_value(flatbuffers::Offset<flatbuffers::String> value) { fbb_.AddOffset(XTraceReportCustomField::VT_VALUE, value); }
  XTraceReportCustomFieldBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  XTraceReportCustomFieldBuilder &operator=(const XTraceReportCustomFieldBuilder &);
  flatbuffers::Offset<XTraceReportCustomField> Finish() {
    auto o = flatbuffers::Offset<XTraceReportCustomField>(fbb_.EndTable(start_, 2));
    return o;
  }
};

inline flatbuffers::Offset<XTraceReportCustomField> CreateXTraceReportCustomField(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> key = 0,
   flatbuffers::Offset<flatbuffers::String> value = 0) {
  XTraceReportCustomFieldBuilder builder_(_fbb);
  builder_.add_value(value);
  builder_.add_key(key);
  return builder_.Finish();
}

struct XTraceReportv4 FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_TASK_ID = 4,
    VT_EVENT_ID = 6,
    VT_PARENT_EVENT_ID = 8,
    VT_TENANT_CLASS = 10,
    VT_TIMESTAMP = 12,
    VT_HRT = 14,
    VT_CYCLES = 16,
    VT_PROCESS_ID = 18,
    VT_THREAD_ID = 20,
    VT_HOST = 22,
    VT_PROCESS_NAME = 24,
    VT_THREAD_NAME = 26,
    VT_AGENT = 28,
    VT_SOURCE = 30,
    VT_LABEL = 32,
    VT_CUSTOM_FIELDS = 34,
    VT_TAGS = 36
  };
  int64_t task_id() const { return GetField<int64_t>(VT_TASK_ID, 0); }
  int64_t event_id() const { return GetField<int64_t>(VT_EVENT_ID, 0); }
  const flatbuffers::Vector<int64_t> *parent_event_id() const { return GetPointer<const flatbuffers::Vector<int64_t> *>(VT_PARENT_EVENT_ID); }
  int32_t tenant_class() const { return GetField<int32_t>(VT_TENANT_CLASS, 0); }
  int64_t timestamp() const { return GetField<int64_t>(VT_TIMESTAMP, 0); }
  int64_t hrt() const { return GetField<int64_t>(VT_HRT, 0); }
  int64_t cycles() const { return GetField<int64_t>(VT_CYCLES, 0); }
  int32_t process_id() const { return GetField<int32_t>(VT_PROCESS_ID, 0); }
  int32_t thread_id() const { return GetField<int32_t>(VT_THREAD_ID, 0); }
  const flatbuffers::String *host() const { return GetPointer<const flatbuffers::String *>(VT_HOST); }
  const flatbuffers::String *process_name() const { return GetPointer<const flatbuffers::String *>(VT_PROCESS_NAME); }
  const flatbuffers::String *thread_name() const { return GetPointer<const flatbuffers::String *>(VT_THREAD_NAME); }
  const flatbuffers::String *agent() const { return GetPointer<const flatbuffers::String *>(VT_AGENT); }
  const flatbuffers::String *source() const { return GetPointer<const flatbuffers::String *>(VT_SOURCE); }
  const flatbuffers::String *label() const { return GetPointer<const flatbuffers::String *>(VT_LABEL); }
  const flatbuffers::Vector<flatbuffers::Offset<XTraceReportCustomField>> *custom_fields() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<XTraceReportCustomField>> *>(VT_CUSTOM_FIELDS); }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *tags() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_TAGS); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_TASK_ID) &&
           VerifyField<int64_t>(verifier, VT_EVENT_ID) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_PARENT_EVENT_ID) &&
           verifier.Verify(parent_event_id()) &&
           VerifyField<int32_t>(verifier, VT_TENANT_CLASS) &&
           VerifyField<int64_t>(verifier, VT_TIMESTAMP) &&
           VerifyField<int64_t>(verifier, VT_HRT) &&
           VerifyField<int64_t>(verifier, VT_CYCLES) &&
           VerifyField<int32_t>(verifier, VT_PROCESS_ID) &&
           VerifyField<int32_t>(verifier, VT_THREAD_ID) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_HOST) &&
           verifier.Verify(host()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_PROCESS_NAME) &&
           verifier.Verify(process_name()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_THREAD_NAME) &&
           verifier.Verify(thread_name()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_AGENT) &&
           verifier.Verify(agent()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_SOURCE) &&
           verifier.Verify(source()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_LABEL) &&
           verifier.Verify(label()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_CUSTOM_FIELDS) &&
           verifier.Verify(custom_fields()) &&
           verifier.VerifyVectorOfTables(custom_fields()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TAGS) &&
           verifier.Verify(tags()) &&
           verifier.VerifyVectorOfStrings(tags()) &&
           verifier.EndTable();
  }
};

struct XTraceReportv4Builder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_task_id(int64_t task_id) { fbb_.AddElement<int64_t>(XTraceReportv4::VT_TASK_ID, task_id, 0); }
  void add_event_id(int64_t event_id) { fbb_.AddElement<int64_t>(XTraceReportv4::VT_EVENT_ID, event_id, 0); }
  void add_parent_event_id(flatbuffers::Offset<flatbuffers::Vector<int64_t>> parent_event_id) { fbb_.AddOffset(XTraceReportv4::VT_PARENT_EVENT_ID, parent_event_id); }
  void add_tenant_class(int32_t tenant_class) { fbb_.AddElement<int32_t>(XTraceReportv4::VT_TENANT_CLASS, tenant_class, 0); }
  void add_timestamp(int64_t timestamp) { fbb_.AddElement<int64_t>(XTraceReportv4::VT_TIMESTAMP, timestamp, 0); }
  void add_hrt(int64_t hrt) { fbb_.AddElement<int64_t>(XTraceReportv4::VT_HRT, hrt, 0); }
  void add_cycles(int64_t cycles) { fbb_.AddElement<int64_t>(XTraceReportv4::VT_CYCLES, cycles, 0); }
  void add_process_id(int32_t process_id) { fbb_.AddElement<int32_t>(XTraceReportv4::VT_PROCESS_ID, process_id, 0); }
  void add_thread_id(int32_t thread_id) { fbb_.AddElement<int32_t>(XTraceReportv4::VT_THREAD_ID, thread_id, 0); }
  void add_host(flatbuffers::Offset<flatbuffers::String> host) { fbb_.AddOffset(XTraceReportv4::VT_HOST, host); }
  void add_process_name(flatbuffers::Offset<flatbuffers::String> process_name) { fbb_.AddOffset(XTraceReportv4::VT_PROCESS_NAME, process_name); }
  void add_thread_name(flatbuffers::Offset<flatbuffers::String> thread_name) { fbb_.AddOffset(XTraceReportv4::VT_THREAD_NAME, thread_name); }
  void add_agent(flatbuffers::Offset<flatbuffers::String> agent) { fbb_.AddOffset(XTraceReportv4::VT_AGENT, agent); }
  void add_source(flatbuffers::Offset<flatbuffers::String> source) { fbb_.AddOffset(XTraceReportv4::VT_SOURCE, source); }
  void add_label(flatbuffers::Offset<flatbuffers::String> label) { fbb_.AddOffset(XTraceReportv4::VT_LABEL, label); }
  void add_custom_fields(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<XTraceReportCustomField>>> custom_fields) { fbb_.AddOffset(XTraceReportv4::VT_CUSTOM_FIELDS, custom_fields); }
  void add_tags(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> tags) { fbb_.AddOffset(XTraceReportv4::VT_TAGS, tags); }
  XTraceReportv4Builder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  XTraceReportv4Builder &operator=(const XTraceReportv4Builder &);
  flatbuffers::Offset<XTraceReportv4> Finish() {
    auto o = flatbuffers::Offset<XTraceReportv4>(fbb_.EndTable(start_, 17));
    return o;
  }
};

inline flatbuffers::Offset<XTraceReportv4> CreateXTraceReportv4(flatbuffers::FlatBufferBuilder &_fbb,
   int64_t task_id = 0,
   int64_t event_id = 0,
   flatbuffers::Offset<flatbuffers::Vector<int64_t>> parent_event_id = 0,
   int32_t tenant_class = 0,
   int64_t timestamp = 0,
   int64_t hrt = 0,
   int64_t cycles = 0,
   int32_t process_id = 0,
   int32_t thread_id = 0,
   flatbuffers::Offset<flatbuffers::String> host = 0,
   flatbuffers::Offset<flatbuffers::String> process_name = 0,
   flatbuffers::Offset<flatbuffers::String> thread_name = 0,
   flatbuffers::Offset<flatbuffers::String> agent = 0,
   flatbuffers::Offset<flatbuffers::String> source = 0,
   flatbuffers::Offset<flatbuffers::String> label = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<XTraceReportCustomField>>> custom_fields = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> tags = 0) {
  XTraceReportv4Builder builder_(_fbb);
  builder_.add_cycles(cycles);
  builder_.add_hrt(hrt);
  builder_.add_timestamp(timestamp);
  builder_.add_event_id(event_id);
  builder_.add_task_id(task_id);
  builder_.add_tags(tags);
  builder_.add_custom_fields(custom_fields);
  builder_.add_label(label);
  builder_.add_source(source);
  builder_.add_agent(agent);
  builder_.add_thread_name(thread_name);
  builder_.add_process_name(process_name);
  builder_.add_host(host);
  builder_.add_thread_id(thread_id);
  builder_.add_process_id(process_id);
  builder_.add_tenant_class(tenant_class);
  builder_.add_parent_event_id(parent_event_id);
  return builder_.Finish();
}

}  // namespace xtraceflatbuffers

#endif  // FLATBUFFERS_GENERATED_REPORTING_XTRACEFLATBUFFERS_H_