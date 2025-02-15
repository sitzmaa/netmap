package main

import (
    "context"
    "encoding/json"
    "fmt"
    "log"
    "net"

    pb "netmap/proto" // Ensure this is the correct relative path to your generated proto file
    "google.golang.org/grpc"
)

// NetMapServer struct implements the gRPC server interface
type NetMapServer struct {
    pb.UnimplementedNetMapServer
}

// GetState: Fetch current netmap state and send it to the Python client
func (s *NetMapServer) GetState(ctx context.Context, req *pb.NetworkDataRequest) (*pb.NetworkDataResponse, error) {
    // Fetch network state from the C++ backend
    data := queryCPlusPlusBackend()

    // Convert to JSON for Python GUI
    jsonData, err := json.Marshal(data)
    if err != nil {
        return nil, fmt.Errorf("failed to marshal data: %v", err)
    }

    // Return the response with the JSON data
    return &pb.NetworkDataResponse{Message: string(jsonData)}, nil
}

func (s *NetMapServer) SendNetworkData(ctx context.Context, req *pb.NetworkDataRequest) (*pb.NetworkDataResponse, error) {
	// Handle the request and return a response
	fmt.Println("Received network data:", req.GetNetworkData())
	return &pb.NetworkDataResponse{
		Message: "Network data received successfully",
	}, nil
}

// Function to query C++ backend (over a socket or another method)
func queryCPlusPlusBackend() map[string]interface{} {
    // Placeholder: Implement the actual socket or inter-process communication with C++ backend
    // Example mock data returned from the C++ backend for demonstration
    return map[string]interface{}{
        "nodes": []map[string]string{
            {"ip": "192.168.1.1", "mac": "AA:BB:CC:DD:EE:FF"},
        },
        "links": []map[string]string{
            {"source": "192.168.1.1", "target": "192.168.1.2"},
        },
    }
}

func main() {
    // Listen for incoming connections on TCP port 50051
    lis, err := net.Listen("tcp", ":50051")
    if err != nil {
        log.Fatalf("Failed to listen on port 50051: %v", err)
    }

    // Create a new gRPC server
    grpcServer := grpc.NewServer()

    // Register the NetMapServer with the gRPC server
    pb.RegisterNetMapServer(grpcServer, &NetMapServer{})

    fmt.Println("gRPC Server is running on port 50051")

    // Start serving requests
    if err := grpcServer.Serve(lis); err != nil {
        log.Fatalf("Failed to serve gRPC server: %v", err)
    }
}