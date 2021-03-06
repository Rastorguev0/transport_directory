// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: svg.proto

#include "svg.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_svg_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Rgba_svg_2eproto;
namespace SvgProto {
class RgbaDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Rgba> _instance;
} _Rgba_default_instance_;
class ColorDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Color> _instance;
} _Color_default_instance_;
class PointDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Point> _instance;
} _Point_default_instance_;
}  // namespace SvgProto
static void InitDefaultsscc_info_Color_svg_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::SvgProto::_Color_default_instance_;
    new (ptr) ::SvgProto::Color();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_Color_svg_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_Color_svg_2eproto}, {
      &scc_info_Rgba_svg_2eproto.base,}};

static void InitDefaultsscc_info_Point_svg_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::SvgProto::_Point_default_instance_;
    new (ptr) ::SvgProto::Point();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Point_svg_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Point_svg_2eproto}, {}};

static void InitDefaultsscc_info_Rgba_svg_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::SvgProto::_Rgba_default_instance_;
    new (ptr) ::SvgProto::Rgba();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Rgba_svg_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Rgba_svg_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_svg_2eproto[3];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_svg_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_svg_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_svg_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::SvgProto::Rgba, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::SvgProto::Rgba, red_),
  PROTOBUF_FIELD_OFFSET(::SvgProto::Rgba, green_),
  PROTOBUF_FIELD_OFFSET(::SvgProto::Rgba, blue_),
  PROTOBUF_FIELD_OFFSET(::SvgProto::Rgba, has_opacity_),
  PROTOBUF_FIELD_OFFSET(::SvgProto::Rgba, opacity_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::SvgProto::Color, _internal_metadata_),
  ~0u,  // no _extensions_
  PROTOBUF_FIELD_OFFSET(::SvgProto::Color, _oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  ::PROTOBUF_NAMESPACE_ID::internal::kInvalidFieldOffsetTag,
  PROTOBUF_FIELD_OFFSET(::SvgProto::Color, data_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::SvgProto::Point, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::SvgProto::Point, x_),
  PROTOBUF_FIELD_OFFSET(::SvgProto::Point, y_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::SvgProto::Rgba)},
  { 10, -1, sizeof(::SvgProto::Color)},
  { 19, -1, sizeof(::SvgProto::Point)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::SvgProto::_Rgba_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::SvgProto::_Color_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::SvgProto::_Point_default_instance_),
};

const char descriptor_table_protodef_svg_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\tsvg.proto\022\010SvgProto\"V\n\004Rgba\022\013\n\003red\030\001 \001"
  "(\r\022\r\n\005green\030\002 \001(\r\022\014\n\004blue\030\003 \001(\r\022\023\n\013has_o"
  "pacity\030\004 \001(\010\022\017\n\007opacity\030\005 \001(\001\"R\n\005Color\022\021"
  "\n\007is_none\030\001 \001(\010H\000\022\016\n\004name\030\002 \001(\tH\000\022\036\n\004rgb"
  "a\030\003 \001(\0132\016.SvgProto.RgbaH\000B\006\n\004data\"\035\n\005Poi"
  "nt\022\t\n\001x\030\001 \001(\001\022\t\n\001y\030\002 \001(\001b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_svg_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_svg_2eproto_sccs[3] = {
  &scc_info_Color_svg_2eproto.base,
  &scc_info_Point_svg_2eproto.base,
  &scc_info_Rgba_svg_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_svg_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_svg_2eproto = {
  false, false, descriptor_table_protodef_svg_2eproto, "svg.proto", 232,
  &descriptor_table_svg_2eproto_once, descriptor_table_svg_2eproto_sccs, descriptor_table_svg_2eproto_deps, 3, 0,
  schemas, file_default_instances, TableStruct_svg_2eproto::offsets,
  file_level_metadata_svg_2eproto, 3, file_level_enum_descriptors_svg_2eproto, file_level_service_descriptors_svg_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_svg_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_svg_2eproto)), true);
namespace SvgProto {

// ===================================================================

class Rgba::_Internal {
 public:
};

Rgba::Rgba(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:SvgProto.Rgba)
}
Rgba::Rgba(const Rgba& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&red_, &from.red_,
    static_cast<size_t>(reinterpret_cast<char*>(&opacity_) -
    reinterpret_cast<char*>(&red_)) + sizeof(opacity_));
  // @@protoc_insertion_point(copy_constructor:SvgProto.Rgba)
}

void Rgba::SharedCtor() {
  ::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
      reinterpret_cast<char*>(&red_) - reinterpret_cast<char*>(this)),
      0, static_cast<size_t>(reinterpret_cast<char*>(&opacity_) -
      reinterpret_cast<char*>(&red_)) + sizeof(opacity_));
}

Rgba::~Rgba() {
  // @@protoc_insertion_point(destructor:SvgProto.Rgba)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Rgba::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void Rgba::ArenaDtor(void* object) {
  Rgba* _this = reinterpret_cast< Rgba* >(object);
  (void)_this;
}
void Rgba::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Rgba::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Rgba& Rgba::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Rgba_svg_2eproto.base);
  return *internal_default_instance();
}


void Rgba::Clear() {
// @@protoc_insertion_point(message_clear_start:SvgProto.Rgba)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&red_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&opacity_) -
      reinterpret_cast<char*>(&red_)) + sizeof(opacity_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Rgba::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // uint32 red = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          red_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 green = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          green_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 blue = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          blue_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bool has_opacity = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          has_opacity_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // double opacity = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 41)) {
          opacity_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Rgba::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:SvgProto.Rgba)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 red = 1;
  if (this->red() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(1, this->_internal_red(), target);
  }

  // uint32 green = 2;
  if (this->green() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(2, this->_internal_green(), target);
  }

  // uint32 blue = 3;
  if (this->blue() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(3, this->_internal_blue(), target);
  }

  // bool has_opacity = 4;
  if (this->has_opacity() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(4, this->_internal_has_opacity(), target);
  }

  // double opacity = 5;
  if (!(this->opacity() <= 0 && this->opacity() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(5, this->_internal_opacity(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SvgProto.Rgba)
  return target;
}

size_t Rgba::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:SvgProto.Rgba)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // uint32 red = 1;
  if (this->red() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_red());
  }

  // uint32 green = 2;
  if (this->green() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_green());
  }

  // uint32 blue = 3;
  if (this->blue() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_blue());
  }

  // bool has_opacity = 4;
  if (this->has_opacity() != 0) {
    total_size += 1 + 1;
  }

  // double opacity = 5;
  if (!(this->opacity() <= 0 && this->opacity() >= 0)) {
    total_size += 1 + 8;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Rgba::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:SvgProto.Rgba)
  GOOGLE_DCHECK_NE(&from, this);
  const Rgba* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Rgba>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:SvgProto.Rgba)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:SvgProto.Rgba)
    MergeFrom(*source);
  }
}

void Rgba::MergeFrom(const Rgba& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:SvgProto.Rgba)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.red() != 0) {
    _internal_set_red(from._internal_red());
  }
  if (from.green() != 0) {
    _internal_set_green(from._internal_green());
  }
  if (from.blue() != 0) {
    _internal_set_blue(from._internal_blue());
  }
  if (from.has_opacity() != 0) {
    _internal_set_has_opacity(from._internal_has_opacity());
  }
  if (!(from.opacity() <= 0 && from.opacity() >= 0)) {
    _internal_set_opacity(from._internal_opacity());
  }
}

void Rgba::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:SvgProto.Rgba)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Rgba::CopyFrom(const Rgba& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:SvgProto.Rgba)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Rgba::IsInitialized() const {
  return true;
}

void Rgba::InternalSwap(Rgba* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Rgba, opacity_)
      + sizeof(Rgba::opacity_)
      - PROTOBUF_FIELD_OFFSET(Rgba, red_)>(
          reinterpret_cast<char*>(&red_),
          reinterpret_cast<char*>(&other->red_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Rgba::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

class Color::_Internal {
 public:
  static const ::SvgProto::Rgba& rgba(const Color* msg);
};

const ::SvgProto::Rgba&
Color::_Internal::rgba(const Color* msg) {
  return *msg->data_.rgba_;
}
void Color::set_allocated_rgba(::SvgProto::Rgba* rgba) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  clear_data();
  if (rgba) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(rgba);
    if (message_arena != submessage_arena) {
      rgba = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, rgba, submessage_arena);
    }
    set_has_rgba();
    data_.rgba_ = rgba;
  }
  // @@protoc_insertion_point(field_set_allocated:SvgProto.Color.rgba)
}
Color::Color(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:SvgProto.Color)
}
Color::Color(const Color& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  clear_has_data();
  switch (from.data_case()) {
    case kIsNone: {
      _internal_set_is_none(from._internal_is_none());
      break;
    }
    case kName: {
      _internal_set_name(from._internal_name());
      break;
    }
    case kRgba: {
      _internal_mutable_rgba()->::SvgProto::Rgba::MergeFrom(from._internal_rgba());
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  // @@protoc_insertion_point(copy_constructor:SvgProto.Color)
}

void Color::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_Color_svg_2eproto.base);
  clear_has_data();
}

Color::~Color() {
  // @@protoc_insertion_point(destructor:SvgProto.Color)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Color::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  if (has_data()) {
    clear_data();
  }
}

void Color::ArenaDtor(void* object) {
  Color* _this = reinterpret_cast< Color* >(object);
  (void)_this;
}
void Color::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Color::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Color& Color::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Color_svg_2eproto.base);
  return *internal_default_instance();
}


void Color::clear_data() {
// @@protoc_insertion_point(one_of_clear_start:SvgProto.Color)
  switch (data_case()) {
    case kIsNone: {
      // No need to clear
      break;
    }
    case kName: {
      data_.name_.Destroy(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
      break;
    }
    case kRgba: {
      if (GetArena() == nullptr) {
        delete data_.rgba_;
      }
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  _oneof_case_[0] = DATA_NOT_SET;
}


void Color::Clear() {
// @@protoc_insertion_point(message_clear_start:SvgProto.Color)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  clear_data();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Color::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // bool is_none = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _internal_set_is_none(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string name = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_name();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "SvgProto.Color.name"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .SvgProto.Rgba rgba = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_rgba(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Color::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:SvgProto.Color)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // bool is_none = 1;
  if (_internal_has_is_none()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(1, this->_internal_is_none(), target);
  }

  // string name = 2;
  if (_internal_has_name()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_name().data(), static_cast<int>(this->_internal_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "SvgProto.Color.name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_name(), target);
  }

  // .SvgProto.Rgba rgba = 3;
  if (_internal_has_rgba()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        3, _Internal::rgba(this), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SvgProto.Color)
  return target;
}

size_t Color::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:SvgProto.Color)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  switch (data_case()) {
    // bool is_none = 1;
    case kIsNone: {
      total_size += 1 + 1;
      break;
    }
    // string name = 2;
    case kName: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_name());
      break;
    }
    // .SvgProto.Rgba rgba = 3;
    case kRgba: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
          *data_.rgba_);
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Color::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:SvgProto.Color)
  GOOGLE_DCHECK_NE(&from, this);
  const Color* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Color>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:SvgProto.Color)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:SvgProto.Color)
    MergeFrom(*source);
  }
}

void Color::MergeFrom(const Color& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:SvgProto.Color)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  switch (from.data_case()) {
    case kIsNone: {
      _internal_set_is_none(from._internal_is_none());
      break;
    }
    case kName: {
      _internal_set_name(from._internal_name());
      break;
    }
    case kRgba: {
      _internal_mutable_rgba()->::SvgProto::Rgba::MergeFrom(from._internal_rgba());
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
}

void Color::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:SvgProto.Color)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Color::CopyFrom(const Color& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:SvgProto.Color)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Color::IsInitialized() const {
  return true;
}

void Color::InternalSwap(Color* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(data_, other->data_);
  swap(_oneof_case_[0], other->_oneof_case_[0]);
}

::PROTOBUF_NAMESPACE_ID::Metadata Color::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

class Point::_Internal {
 public:
};

Point::Point(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:SvgProto.Point)
}
Point::Point(const Point& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&x_, &from.x_,
    static_cast<size_t>(reinterpret_cast<char*>(&y_) -
    reinterpret_cast<char*>(&x_)) + sizeof(y_));
  // @@protoc_insertion_point(copy_constructor:SvgProto.Point)
}

void Point::SharedCtor() {
  ::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
      reinterpret_cast<char*>(&x_) - reinterpret_cast<char*>(this)),
      0, static_cast<size_t>(reinterpret_cast<char*>(&y_) -
      reinterpret_cast<char*>(&x_)) + sizeof(y_));
}

Point::~Point() {
  // @@protoc_insertion_point(destructor:SvgProto.Point)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Point::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void Point::ArenaDtor(void* object) {
  Point* _this = reinterpret_cast< Point* >(object);
  (void)_this;
}
void Point::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Point::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Point& Point::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Point_svg_2eproto.base);
  return *internal_default_instance();
}


void Point::Clear() {
// @@protoc_insertion_point(message_clear_start:SvgProto.Point)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&x_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&y_) -
      reinterpret_cast<char*>(&x_)) + sizeof(y_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Point::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // double x = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 9)) {
          x_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // double y = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 17)) {
          y_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Point::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:SvgProto.Point)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // double x = 1;
  if (!(this->x() <= 0 && this->x() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(1, this->_internal_x(), target);
  }

  // double y = 2;
  if (!(this->y() <= 0 && this->y() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(2, this->_internal_y(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SvgProto.Point)
  return target;
}

size_t Point::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:SvgProto.Point)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // double x = 1;
  if (!(this->x() <= 0 && this->x() >= 0)) {
    total_size += 1 + 8;
  }

  // double y = 2;
  if (!(this->y() <= 0 && this->y() >= 0)) {
    total_size += 1 + 8;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Point::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:SvgProto.Point)
  GOOGLE_DCHECK_NE(&from, this);
  const Point* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Point>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:SvgProto.Point)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:SvgProto.Point)
    MergeFrom(*source);
  }
}

void Point::MergeFrom(const Point& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:SvgProto.Point)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!(from.x() <= 0 && from.x() >= 0)) {
    _internal_set_x(from._internal_x());
  }
  if (!(from.y() <= 0 && from.y() >= 0)) {
    _internal_set_y(from._internal_y());
  }
}

void Point::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:SvgProto.Point)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Point::CopyFrom(const Point& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:SvgProto.Point)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Point::IsInitialized() const {
  return true;
}

void Point::InternalSwap(Point* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Point, y_)
      + sizeof(Point::y_)
      - PROTOBUF_FIELD_OFFSET(Point, x_)>(
          reinterpret_cast<char*>(&x_),
          reinterpret_cast<char*>(&other->x_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Point::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace SvgProto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::SvgProto::Rgba* Arena::CreateMaybeMessage< ::SvgProto::Rgba >(Arena* arena) {
  return Arena::CreateMessageInternal< ::SvgProto::Rgba >(arena);
}
template<> PROTOBUF_NOINLINE ::SvgProto::Color* Arena::CreateMaybeMessage< ::SvgProto::Color >(Arena* arena) {
  return Arena::CreateMessageInternal< ::SvgProto::Color >(arena);
}
template<> PROTOBUF_NOINLINE ::SvgProto::Point* Arena::CreateMaybeMessage< ::SvgProto::Point >(Arena* arena) {
  return Arena::CreateMessageInternal< ::SvgProto::Point >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
