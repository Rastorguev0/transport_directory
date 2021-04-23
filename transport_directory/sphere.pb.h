// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: sphere.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_sphere_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_sphere_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3014000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3014000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_sphere_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_sphere_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_sphere_2eproto;
namespace SphereProto {
class Coords;
class CoordsDefaultTypeInternal;
extern CoordsDefaultTypeInternal _Coords_default_instance_;
}  // namespace SphereProto
PROTOBUF_NAMESPACE_OPEN
template<> ::SphereProto::Coords* Arena::CreateMaybeMessage<::SphereProto::Coords>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace SphereProto {

// ===================================================================

class Coords PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:SphereProto.Coords) */ {
 public:
  inline Coords() : Coords(nullptr) {}
  virtual ~Coords();

  Coords(const Coords& from);
  Coords(Coords&& from) noexcept
    : Coords() {
    *this = ::std::move(from);
  }

  inline Coords& operator=(const Coords& from) {
    CopyFrom(from);
    return *this;
  }
  inline Coords& operator=(Coords&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Coords& default_instance();

  static inline const Coords* internal_default_instance() {
    return reinterpret_cast<const Coords*>(
               &_Coords_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Coords& a, Coords& b) {
    a.Swap(&b);
  }
  inline void Swap(Coords* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Coords* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Coords* New() const final {
    return CreateMaybeMessage<Coords>(nullptr);
  }

  Coords* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Coords>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Coords& from);
  void MergeFrom(const Coords& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Coords* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "SphereProto.Coords";
  }
  protected:
  explicit Coords(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_sphere_2eproto);
    return ::descriptor_table_sphere_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kLatFieldNumber = 1,
    kLonFieldNumber = 2,
  };
  // double lat = 1;
  void clear_lat();
  double lat() const;
  void set_lat(double value);
  private:
  double _internal_lat() const;
  void _internal_set_lat(double value);
  public:

  // double lon = 2;
  void clear_lon();
  double lon() const;
  void set_lon(double value);
  private:
  double _internal_lon() const;
  void _internal_set_lon(double value);
  public:

  // @@protoc_insertion_point(class_scope:SphereProto.Coords)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  double lat_;
  double lon_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_sphere_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Coords

// double lat = 1;
inline void Coords::clear_lat() {
  lat_ = 0;
}
inline double Coords::_internal_lat() const {
  return lat_;
}
inline double Coords::lat() const {
  // @@protoc_insertion_point(field_get:SphereProto.Coords.lat)
  return _internal_lat();
}
inline void Coords::_internal_set_lat(double value) {
  
  lat_ = value;
}
inline void Coords::set_lat(double value) {
  _internal_set_lat(value);
  // @@protoc_insertion_point(field_set:SphereProto.Coords.lat)
}

// double lon = 2;
inline void Coords::clear_lon() {
  lon_ = 0;
}
inline double Coords::_internal_lon() const {
  return lon_;
}
inline double Coords::lon() const {
  // @@protoc_insertion_point(field_get:SphereProto.Coords.lon)
  return _internal_lon();
}
inline void Coords::_internal_set_lon(double value) {
  
  lon_ = value;
}
inline void Coords::set_lon(double value) {
  _internal_set_lon(value);
  // @@protoc_insertion_point(field_set:SphereProto.Coords.lon)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace SphereProto

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_sphere_2eproto
