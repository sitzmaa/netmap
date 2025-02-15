package main

import (
    "context"
    "fmt"
    "log"
    "net"
    "encoding/json"

    pb "path/to/generated/proto"
    "google.golang.org/grpc"
)

// NetMapServer struct
type NetMapServer struct {
    pb.UnimplementedNetMapServer
}

// GetState: Fetch current netmap state
func (s *NetMapServer) GetState(ctx context.Context, req *pb.Request) (*pb.Response, error) {
    // Fetch network state from C++ server
    data := queryCPlusPlusBackend()

    // Convert to JSON for Python GUI
    jsonData, _ := json.Marshal(data)

    return &pb.Response{JsonData: string(jsonData)}, nil
}

// Function to query C++ backend over socket
func queryCPlusPlusBackend() map[string]interface{} {
    // Implement socket connection to C++ here
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
    lis, err := net.Listen("tcp", ":50051")
    if err != nil {
        log.Fatalf("Failed to listen: %v", err)
    }

    grpcServer := grpc.NewServer()
    pb.RegisterNetMapServer(grpcServer, &NetMapServer{})

    fmt.Println("gRPC Server running on port 50051")
    if err := grpcServer.Serve(lis); err != nil {
        log.Fatalf("Failed to serve: %v", err)
    }
}