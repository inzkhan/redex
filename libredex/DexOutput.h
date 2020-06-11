/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <unordered_map>

#include <boost/optional/optional.hpp>

#include "ConfigFiles.h"
#include "DexClass.h"
#include "DexStats.h"
#include "DexUtil.h"
#include "Pass.h"
#include "PostLowering.h"
#include "ProguardMap.h"
#include "Trace.h"

#include <locator.h>
using facebook::Locator;

class DexCallSite;
class DexMethodHandle;

using dexstring_to_idx = std::unordered_map<DexString*, uint32_t>;
using dextype_to_idx = std::unordered_map<DexType*, uint16_t>;
using dexproto_to_idx = std::unordered_map<DexProto*, uint32_t>;
using dexfield_to_idx = std::unordered_map<DexFieldRef*, uint32_t>;
using dexmethod_to_idx = std::unordered_map<DexMethodRef*, uint32_t>;
using dexcallsite_to_idx = std::unordered_map<DexCallSite*, uint32_t>;
using dexmethodhandle_to_idx = std::unordered_map<DexMethodHandle*, uint32_t>;

using LocatorIndex = std::unordered_map<DexString*, Locator>;
LocatorIndex make_locator_index(DexStoresVector& stores);

enum class SortMode {
  CLASS_ORDER,
  CLASS_STRINGS,
  CLINIT_FIRST,
  METHOD_PROFILED_ORDER,
  DEFAULT
};

class DexOutputIdx {
 private:
  dexstring_to_idx* m_string;
  dextype_to_idx* m_type;
  dexproto_to_idx* m_proto;
  dexfield_to_idx* m_field;
  dexmethod_to_idx* m_method;
  std::vector<DexTypeList*>* m_typelist;
  dexcallsite_to_idx* m_callsite;
  dexmethodhandle_to_idx* m_methodhandle;
  const uint8_t* m_base;

 public:
  DexOutputIdx(dexstring_to_idx* string,
               dextype_to_idx* type,
               dexproto_to_idx* proto,
               dexfield_to_idx* field,
               dexmethod_to_idx* method,
               std::vector<DexTypeList*>* typelist,
               dexcallsite_to_idx* callsite,
               dexmethodhandle_to_idx* methodhandle,
               const uint8_t* base) {
    m_string = string;
    m_type = type;
    m_proto = proto;
    m_field = field;
    m_method = method;
    m_typelist = typelist;
    m_callsite = callsite;
    m_methodhandle = methodhandle;
    m_base = base;
  }

  ~DexOutputIdx() {
    delete m_string;
    delete m_type;
    delete m_proto;
    delete m_field;
    delete m_method;
    delete m_typelist;
    delete m_callsite;
    delete m_methodhandle;
  }

  dexstring_to_idx& string_to_idx() const { return *m_string; }
  dextype_to_idx& type_to_idx() const { return *m_type; }
  dexproto_to_idx& proto_to_idx() const { return *m_proto; }
  dexfield_to_idx& field_to_idx() const { return *m_field; }
  dexmethod_to_idx& method_to_idx() const { return *m_method; }
  std::vector<DexTypeList*>& typelist_list() const { return *m_typelist; }
  dexcallsite_to_idx& callsite_to_idx() const { return *m_callsite; }
  dexmethodhandle_to_idx& methodhandle_to_idx() const {
    return *m_methodhandle;
  }

  uint32_t stringidx(DexString* s) const { return m_string->at(s); }
  uint16_t typeidx(DexType* t) const { return m_type->at(t); }
  uint16_t protoidx(DexProto* p) const { return m_proto->at(p); }
  uint32_t fieldidx(DexFieldRef* f) const { return m_field->at(f); }
  uint32_t methodidx(DexMethodRef* m) const { return m_method->at(m); }
  uint32_t callsiteidx(DexCallSite* c) const { return m_callsite->at(c); }
  uint32_t methodhandleidx(DexMethodHandle* c) const {
    return m_methodhandle->at(c);
  }

  size_t stringsize() const { return m_string->size(); }
  size_t typesize() const { return m_type->size(); }
  size_t protosize() const { return m_proto->size(); }
  size_t fieldsize() const { return m_field->size(); }
  size_t methodsize() const { return m_method->size(); }
  size_t callsitesize() const { return m_callsite->size(); }
  size_t methodhandlesize() const { return m_methodhandle->size(); }

  uint32_t get_offset(uint8_t* ptr) { return (uint32_t)(ptr - m_base); }

  uint32_t get_offset(uint32_t* ptr) { return get_offset((uint8_t*)ptr); }
};

class IODIMetadata;

dex_stats_t write_classes_to_dex(
    const RedexOptions&,
    const std::string& filename,
    DexClasses* classes,
    LocatorIndex* locator_index /* nullable */,
    size_t store_number,
    size_t dex_number,
    const ConfigFiles& conf,
    PositionMapper* line_mapper,
    std::unordered_map<DexMethod*, uint64_t>* method_to_id,
    std::unordered_map<DexCode*, std::vector<DebugLineItem>>* code_debug_lines,
    IODIMetadata* iodi_metadata,
    const std::string& dex_magic,
    PostLowering const* post_lowering = nullptr,
    int min_sdk = 0);

using cmp_dstring = bool (*)(const DexString*, const DexString*);
using cmp_dtype = bool (*)(const DexType*, const DexType*);
using cmp_dproto = bool (*)(const DexProto*, const DexProto*);
using cmp_dfield = bool (*)(const DexFieldRef*, const DexFieldRef*);
using cmp_dmethod = bool (*)(const DexMethodRef*, const DexMethodRef*);
using cmp_dtypelist = bool (*)(const DexTypeList*, const DexTypeList*);
using cmp_callsite = bool (*)(const DexCallSite*, const DexCallSite*);
using cmp_methodhandle = bool (*)(const DexMethodHandle*,
                                  const DexMethodHandle*);

inline bool compare_dexcallsites(const DexCallSite* a, const DexCallSite* b) {
  if (a == nullptr) {
    return b != nullptr;
  } else if (b == nullptr) {
    return false;
  }
  // TODO(T59683693) -- need real ordering
  return a < b;
}

inline bool compare_dexmethodhandles(const DexMethodHandle* a,
                                     const DexMethodHandle* b) {
  if (a == nullptr) {
    return b != nullptr;
  } else if (b == nullptr) {
    return false;
  }
  if (a->type() != b->type()) {
    return a->type() < b->type();
  }
  if (DexMethodHandle::isInvokeType(a->type())) {
    return compare_dexmethods(a->methodref(), b->methodref());
  } else {
    return compare_dexfields(a->fieldref(), b->fieldref());
  }
}

/*
 * This API gathers all of the data referred to by a set of DexClasses in
 * preparation for emitting a dex file and provides the symbol tables in indexed
 * form for encoding.
 *
 * The gather algorithm implemented in gather_components() traverses the tree of
 * DexFoo objects rooted at each DexClass.  The individual gather methods,
 * gather_{strings,types,fields,methods}, (see Gatherable.h and DexClass.h) find
 * references to each type, respectively, that the object needs.
 *
 * Fields and methods need special consideration: those that are defined by a
 * DexClass need to emit more data (for example, methods must emit their code).
 * Fields and methods that are merely referenced by this DexClass (for example,
 * a call into the Android library) only need the types and strings necessary to
 * represent the reference.  To handle these divergent cases, gather_foo gathers
 * all of the data, while gather_foo_shallow gathers only what is needed for
 * references.
 *
 * Another subtlety is that gather_foo only follows fields of type foo.  For
 * example, DexField contains both a DexType (m_type) and a DexString (m_name).
 * Even though DexType also contains a string, DexField::gather_strings will
 * only gather m_name; it does not follow m_type to find more strings.  This
 * design simplifies the implentation of the gather methods since it breaks
 * cycles in the reference graph, but it makes finding a "complete" set more
 * involved.  To gather all strings, for instance, one must not only gather all
 * strings at the class level, but also gather strings for all types discovered
 * at the class level.
 */

class GatheredTypes {
 private:
  std::vector<DexString*> m_lstring;
  std::vector<DexType*> m_ltype;
  std::vector<DexFieldRef*> m_lfield;
  std::vector<DexMethodRef*> m_lmethod;
  std::vector<DexTypeList*> m_additional_ltypelists;
  std::vector<DexCallSite*> m_lcallsite;
  std::vector<DexMethodHandle*> m_lmethodhandle;
  DexClasses* m_classes;
  std::unordered_map<const DexString*, unsigned int> m_cls_load_strings;
  std::unordered_map<const DexString*, unsigned int> m_cls_strings;
  std::unordered_map<const DexMethod*, unsigned int> m_methods_in_cls_order;
  std::unordered_map<std::string, unsigned int> m_method_to_weight;
  std::unordered_set<std::string> m_method_sorting_whitelisted_substrings;

  void gather_components(PostLowering const* post_lowering);
  dexstring_to_idx* get_string_index(cmp_dstring cmp = compare_dexstrings);
  dextype_to_idx* get_type_index(cmp_dtype cmp = compare_dextypes);
  dexproto_to_idx* get_proto_index(cmp_dproto cmp = compare_dexprotos);
  dexfield_to_idx* get_field_index(cmp_dfield cmp = compare_dexfields);
  dexmethod_to_idx* get_method_index(cmp_dmethod cmp = compare_dexmethods);
  std::vector<DexTypeList*>* get_typelist_list(
      dexproto_to_idx* protos, cmp_dtypelist cmp = compare_dextypelists);
  dexcallsite_to_idx* get_callsite_index(
      cmp_callsite cmp = compare_dexcallsites);
  dexmethodhandle_to_idx* get_methodhandle_index(
      cmp_methodhandle cmp = compare_dexmethodhandles);

  void build_cls_load_map();
  void build_cls_map();
  void build_method_map();

 public:
  explicit GatheredTypes(DexClasses* classes,
                         PostLowering const* post_lowering = nullptr);

  DexOutputIdx* get_dodx(const uint8_t* base);
  template <class T = decltype(compare_dexstrings)>
  std::vector<DexString*> get_dexstring_emitlist(T cmp = compare_dexstrings);
  std::vector<DexString*> get_cls_order_dexstring_emitlist();
  std::vector<DexString*> keep_cls_strings_together_emitlist();
  std::vector<DexMethod*> get_dexmethod_emitlist();
  std::vector<DexMethodHandle*> get_dexmethodhandle_emitlist();
  std::vector<DexCallSite*> get_dexcallsite_emitlist();

  void gather_class(int num);

  void sort_dexmethod_emitlist_default_order(std::vector<DexMethod*>& lmeth);
  void sort_dexmethod_emitlist_cls_order(std::vector<DexMethod*>& lmeth);
  void sort_dexmethod_emitlist_clinit_order(std::vector<DexMethod*>& lmeth);
  void sort_dexmethod_emitlist_profiled_order(std::vector<DexMethod*>& lmeth);
  void set_method_sorting_whitelisted_substrings(
      const std::unordered_set<std::string>& whitelisted_substrings);
  void set_method_to_weight(
      const std::unordered_map<std::string, unsigned int>& method_to_weight);

  std::unordered_set<DexString*> index_type_names();
};

template <class T>
std::vector<DexString*> GatheredTypes::get_dexstring_emitlist(T cmp) {
  std::vector<DexString*> strlist(m_lstring);
  std::sort(strlist.begin(), strlist.end(), std::cref(cmp));
  return strlist;
}

using annomap_t = std::map<DexAnnotation*, uint32_t>;
using asetmap_t = std::map<DexAnnotationSet*, uint32_t>;
using xrefmap_t = std::map<ParamAnnotations*, uint32_t>;
using adirmap_t = std::map<DexAnnotationDirectory*, uint32_t>;

struct CodeItemEmit {
  DexMethod* method;
  DexCode* code;
  dex_code_item* code_item;

  CodeItemEmit(DexMethod* meth, DexCode* c, dex_code_item* ci);
};

struct DexOutputTestHelper;

class DexOutput {
 public:
  dex_stats_t m_stats;

 private:
  DexClasses* m_classes;
  DexOutputIdx* dodx;
  GatheredTypes* m_gtypes;
  uint8_t* m_output;
  uint32_t m_offset;
  const char* m_filename;
  size_t m_store_number;
  size_t m_dex_number;
  DebugInfoKind m_debug_info_kind;
  IODIMetadata* m_iodi_metadata;
  PositionMapper* m_pos_mapper;
  std::string m_method_mapping_filename;
  std::string m_class_mapping_filename;
  std::string m_pg_mapping_filename;
  std::string m_bytecode_offset_filename;
  std::unordered_map<DexTypeList*, uint32_t> m_tl_emit_offsets;
  std::vector<CodeItemEmit> m_code_item_emits;
  std::unordered_map<DexMethod*, uint64_t>* m_method_to_id;
  std::unordered_map<DexCode*, std::vector<DebugLineItem>>* m_code_debug_lines;
  std::vector<std::pair<std::string, uint32_t>> m_method_bytecode_offsets;
  std::unordered_map<DexClass*, uint32_t> m_static_values;
  std::unordered_map<DexCallSite*, uint32_t> m_call_site_items;
  dex_header hdr;
  std::vector<dex_map_item> m_map_items;
  LocatorIndex* m_locator_index;
  bool m_normal_primary_dex;
  const ConfigFiles& m_config_files;
  std::unordered_set<std::string> m_method_sorting_whitelisted_substrings;
  bool m_force_class_data_end_of_file;
  int m_min_sdk;

  void insert_map_item(uint16_t typeidx,
                       uint32_t size,
                       uint32_t offset,
                       uint32_t bytes);
  void generate_string_data(SortMode mode = SortMode::DEFAULT);
  void generate_type_data();
  void generate_proto_data();
  void generate_field_data();
  void generate_method_data();
  void generate_class_data();
  void generate_class_data_items();
  void generate_callsite_data();
  void generate_methodhandle_data();

  // Sort code according to a sequence of sorting modes, ordered by precedence.
  // e.g. passing {SortMode::CLINIT_FIRST, SortMode::CLASS_ORDER} means that
  // clinit methods come before all other methods, and remaining methods are
  // sorted by class.
  void generate_code_items(const std::vector<SortMode>& modes);
  void generate_static_values();
  void unique_annotations(annomap_t& annomap,
                          std::vector<DexAnnotation*>& annolist);
  void unique_asets(annomap_t& annomap,
                    asetmap_t& asetmap,
                    std::vector<DexAnnotationSet*>& asetlist);
  void unique_xrefs(asetmap_t& asetmap,
                    xrefmap_t& xrefmap,
                    std::vector<ParamAnnotations*>& xreflist);
  void unique_adirs(asetmap_t& asetmap,
                    xrefmap_t& xrefmap,
                    adirmap_t& adirmap,
                    std::vector<DexAnnotationDirectory*>& adirlist);
  void generate_annotations();
  void generate_debug_items();
  void generate_typelist_data();
  void generate_map();
  void finalize_header();
  void init_header_offsets(const std::string& dex_magic);
  void write_symbol_files();
  void align_output() { m_offset = (m_offset + 3) & ~3; }
  void emit_locator(Locator locator);
  void emit_magic_locators();
  std::unique_ptr<Locator> locator_for_descriptor(
      const std::unordered_set<DexString*>& type_names, DexString* descriptor);

  friend struct DexOutputTestHelper;

 public:
  DexOutput(const char* path,
            DexClasses* classes,
            LocatorIndex* locator_index,
            bool normal_primary_dex,
            size_t store_number,
            size_t dex_number,
            DebugInfoKind debug_info_kind,
            IODIMetadata* iodi_metadata,
            const ConfigFiles& config_files,
            PositionMapper* pos_mapper,
            std::unordered_map<DexMethod*, uint64_t>* method_to_id,
            std::unordered_map<DexCode*, std::vector<DebugLineItem>>*
                code_debug_lines,
            PostLowering const* post_lowering = nullptr,
            int min_sdk = 0);
  ~DexOutput();
  void prepare(SortMode string_mode,
               const std::vector<SortMode>& code_mode,
               const ConfigFiles& conf,
               const std::string& dex_magic);
  void write();
  void metrics();
  static void check_method_instruction_size_limit(const ConfigFiles& conf,
                                                  int size,
                                                  const char* method_name);
};
