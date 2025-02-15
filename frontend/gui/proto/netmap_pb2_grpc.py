# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc
import warnings

import netmap_pb2 as netmap__pb2

GRPC_GENERATED_VERSION = '1.70.0'
GRPC_VERSION = grpc.__version__
_version_not_supported = False

try:
    from grpc._utilities import first_version_is_lower
    _version_not_supported = first_version_is_lower(GRPC_VERSION, GRPC_GENERATED_VERSION)
except ImportError:
    _version_not_supported = True

if _version_not_supported:
    raise RuntimeError(
        f'The grpc package installed is at version {GRPC_VERSION},'
        + f' but the generated code in netmap_pb2_grpc.py depends on'
        + f' grpcio>={GRPC_GENERATED_VERSION}.'
        + f' Please upgrade your grpc module to grpcio>={GRPC_GENERATED_VERSION}'
        + f' or downgrade your generated code using grpcio-tools<={GRPC_VERSION}.'
    )


class NetMapStub(object):
    """Define the service
    """

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.SendNetworkData = channel.unary_unary(
                '/netmap.NetMap/SendNetworkData',
                request_serializer=netmap__pb2.NetworkDataRequest.SerializeToString,
                response_deserializer=netmap__pb2.NetworkDataResponse.FromString,
                _registered_method=True)


class NetMapServicer(object):
    """Define the service
    """

    def SendNetworkData(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_NetMapServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'SendNetworkData': grpc.unary_unary_rpc_method_handler(
                    servicer.SendNetworkData,
                    request_deserializer=netmap__pb2.NetworkDataRequest.FromString,
                    response_serializer=netmap__pb2.NetworkDataResponse.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'netmap.NetMap', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))
    server.add_registered_method_handlers('netmap.NetMap', rpc_method_handlers)


 # This class is part of an EXPERIMENTAL API.
class NetMap(object):
    """Define the service
    """

    @staticmethod
    def SendNetworkData(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/netmap.NetMap/SendNetworkData',
            netmap__pb2.NetworkDataRequest.SerializeToString,
            netmap__pb2.NetworkDataResponse.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)
