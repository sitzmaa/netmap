import grpc
import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), 'proto'))
import proto.netmap_pb2
import proto.netmap_pb2_grpc

class NetworkClient:
    def __init__(self, channel):
        self.client = proto.netmap_pb2_grpc.NetMapStub(channel)

    def send_network_data(self, network_data):
        request = proto.netmap_pb2.NetworkDataRequest(network_data=network_data)
        response = self.client.SendNetworkData(request)
        print("Response from server:", response.message)
        return response.message

def run():
    # Set up the connection to the Go server
    channel = grpc.insecure_channel('localhost:50051')
    client = NetworkClient(channel)

    # Example network data to send to the server
    network_data = "Requesting current network state"
    client.send_network_data(network_data)

if __name__ == "__main__":
    run()