# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: netmap.proto
# Protobuf Python Version: 5.29.0
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(
    _runtime_version.Domain.PUBLIC,
    5,
    29,
    0,
    '',
    'netmap.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cnetmap.proto\x12\x06netmap\"\x1a\n\x07Request\x12\x0f\n\x07\x63ommand\x18\x01 \x01(\t\"\x1d\n\x08Response\x12\x11\n\tjson_data\x18\x01 \x01(\t2r\n\x06NetMap\x12-\n\x08GetState\x12\x0f.netmap.Request\x1a\x10.netmap.Response\x12\x39\n\x10SubscribeUpdates\x12\x0f.netmap.Request\x1a\x10.netmap.Response(\x01\x30\x01\x42\"Z github.com/sitzmaa/netmap;netmapb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'netmap_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  _globals['DESCRIPTOR']._loaded_options = None
  _globals['DESCRIPTOR']._serialized_options = b'Z github.com/sitzmaa/netmap;netmap'
  _globals['_REQUEST']._serialized_start=24
  _globals['_REQUEST']._serialized_end=50
  _globals['_RESPONSE']._serialized_start=52
  _globals['_RESPONSE']._serialized_end=81
  _globals['_NETMAP']._serialized_start=83
  _globals['_NETMAP']._serialized_end=197
# @@protoc_insertion_point(module_scope)
