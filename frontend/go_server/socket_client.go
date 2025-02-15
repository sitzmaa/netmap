package main

import (
	"fmt"
	"log"
	"net"
	"os"
)

const (
	serverAddress = "localhost:12345" // Address of your C++ backend
)

func main() {
	// Establish connection to the C++ backend server
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Error connecting to server: %v", err)
	}
	defer conn.Close()

	// Send request to fetch network state
	_, err = conn.Write([]byte("GET_NETWORK_STATE\n"))
	if err != nil {
		log.Fatalf("Error sending request: %v", err)
	}

	// Read the response from the server
	buf := make([]byte, 1024)
	n, err := conn.Read(buf)
	if err != nil {
		log.Fatalf("Error reading response: %v", err)
	}

	fmt.Printf("Received network state: %s\n", string(buf[:n]))
}