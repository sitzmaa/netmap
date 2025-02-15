package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"time"
)

const (
	defaultServerAddress = "localhost:12345" // Default address if no flag is provided
)

// Establish connection to the C++ backend server
func connectToBackend(serverAddress string) (net.Conn, error) {
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		return nil, fmt.Errorf("error connecting to server: %v", err)
	}
	return conn, nil
}

// Send request to fetch network state
func sendRequest(conn net.Conn) (string, error) {
	_, err := conn.Write([]byte("GET_NETWORK_STATE\n"))
	if err != nil {
		return "", fmt.Errorf("error sending request: %v", err)
	}

	// Read the response from the server
	buf := make([]byte, 1024)
	n, err := conn.Read(buf)
	if err != nil {
		return "", fmt.Errorf("error reading response: %v", err)
	}

	return string(buf[:n]), nil
}

// Fetch network state from the C++ backend
func fetchNetworkState(serverAddress string) (string, error) {
	conn, err := connectToBackend(serverAddress)
	if err != nil {
		return "", err
	}
	defer conn.Close()

	return sendRequest(conn)
}


func main() {
	// Establish connection to the C++ backend
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Error connecting to server: %v", err)
	}
	defer conn.Close()

	// Send true state to alert the server that this is a GUI-type connection
	_, err = conn.Write([]byte("1\n")) // Sending "1" to indicate GUI type
	if err != nil {
		log.Fatalf("Error sending connection type: %v", err)
	}

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