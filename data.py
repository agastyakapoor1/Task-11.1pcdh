import socket

# Set up the IP address and port
HOST = '0.0.0.0'  # Listen on all available network interfaces
PORT = 12345      # Port to listen on

# Create the socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print("Server started. Waiting for connection...")

    # Accept an incoming connection
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            data = conn.recv(1024)
            if not data:
                break
            # Print the data received from Arduino
            print("Received from Arduino:", data.decode())
