package main

import (
	"context"
	"fmt"
	"log"
	"net"
	"time"

	"google.golang.org/grpc"
	"netmap/proto" // Adjust this import path as needed
)

const (
	grpcServerAddress = "localhost:50051" // Python gRPC server address
	backendServerAddr = "localhost:12345" // C++ backend server address
)

// Define the server struct implementing the proto interface for gRPC communication with Python
type server struct {
	proto.UnimplementedNetMapServer
}

// SendNetworkData is the method that sends data to the Python client and receives a response
func (s *server) SendNetworkData(ctx context.Context, req *proto.NetworkDataRequest) (*proto.NetworkDataResponse, error) {
	fmt.Println("Received data from Python frontend:", req.NetworkData)
	// Process the data if needed, then send a response back to the Python client
	return &proto.NetworkDataResponse{Message: "Data received and processed!"}, nil
}

func startGRPCServer() {
	// Set up the gRPC server to listen on port 50051 for the Python client
	listener, err := net.Listen("tcp", grpcServerAddress)
	if err != nil {
		log.Fatalf("Failed to listen on port 50051: %v", err)
	}
	defer listener.Close()

	// Create a new gRPC server
	grpcServer := grpc.NewServer()

	// Register the gRPC server with the NetMap implementation
	proto.RegisterNetMapServer(grpcServer, &server{})

	// Start serving requests for the Python frontend
	fmt.Println("Starting gRPC server for Python frontend on port 50051...")
	if err := grpcServer.Serve(listener); err != nil {
		log.Fatalf("Failed to serve gRPC server: %v", err)
	}
}

func startBackendClient() {
	// Set up the connection to the C++ backend server
	conn, err := net.Dial("tcp", backendServerAddr)
	if err != nil {
		log.Fatalf("Failed to connect to C++ backend server: %v", err)
	}
	defer conn.Close()

	// Send request to the C++ backend for network state
	_, err = conn.Write([]byte("1\n"))
	if err != nil {
		log.Fatalf("Error sending request to C++ backend: %v", err)
	}

	// Read the response from the C++ backend
	buf := make([]byte, 1024)
	n, err := conn.Read(buf)
	if err != nil {
		log.Fatalf("Error reading response from C++ backend: %v", err)
	}

	networkState := string(buf[:n])
	fmt.Printf("Received network state from C++ backend: %s\n", networkState)

	// Sending the network state to the Python client (if necessary)
	sendToPythonFrontend(networkState)
}

func sendToPythonFrontend(networkState string) {
	// Set up connection to Python gRPC server
	conn, err := grpc.Dial(grpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Fatalf("Failed to connect to Python gRPC server: %v", err)
	}
	defer conn.Close()

	client := proto.NewNetMapClient(conn)

	// Send network data to the Python frontend
	req := &proto.NetworkDataRequest{NetworkData: networkState}
	resp, err := client.SendNetworkData(context.Background(), req)
	if err != nil {
		log.Fatalf("Failed to send request to Python frontend: %v", err)
	}

	// Print the response from the Python frontend
	fmt.Println("Response from Python frontend:", resp.GetMessage())
}

func main() {
	// Start the gRPC server in a separate goroutine for the Python frontend
	go startGRPCServer()

	// Simulate a delay to ensure the gRPC server is ready
	time.Sleep(1 * time.Second)

	// Start the backend client to fetch network data and send it to Python frontend
	startBackendClient()

	// To keep the main function running while servers are active
	select {}
}