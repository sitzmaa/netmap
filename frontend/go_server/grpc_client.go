package main

import (
	"context"
	"fmt"
	"log"
	"net"

	"google.golang.org/grpc"
	"your_project_path/proto" // replace with the correct path to your generated proto files
)

const (
	serverAddress = "localhost:50051" // Python gRPC server address
)

type server struct {
	proto.UnimplementedNetworkServiceServer
}

// Example: A method to send data to the Python UI and get a response
func (s *server) SendNetworkData(ctx context.Context, req *proto.NetworkDataRequest) (*proto.NetworkDataResponse, error) {
	fmt.Println("Received data from client:", req)
	// Process the data if needed, then send the response back
	return &proto.NetworkDataResponse{Message: "Data received and processed!"}, nil
}

func main() {
	// Set up connection to Python gRPC server
	conn, err := grpc.Dial(serverAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Fatalf("Failed to connect to server: %v", err)
	}
	defer conn.Close()

	client := proto.NewNetworkServiceClient(conn)

	// Sending data to Python UI
	req := &proto.NetworkDataRequest{NetworkData: "Current network state"}
	resp, err := client.SendNetworkData(context.Background(), req)
	if err != nil {
		log.Fatalf("Failed to send request: %v", err)
	}

	fmt.Println("Response from Python UI:", resp.GetMessage())
}