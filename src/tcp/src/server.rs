use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};
use std::thread;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:8080") //bara localhost för nu
        .expect("Failed to bind to port 8080");
    
    println!("Server listening on 127.0.0.1:8080"); //meddela att servern är igång
    
    for stream in listener.incoming() { //acceptera inkommande anslutningar
        match stream {
            Ok(stream) => {
                thread::spawn(|| {
                    handle_client(stream);
                });
            }
            Err(e) => {
                eprintln!("Connection error: {}", e);
            }
        }
    }
}

fn handle_client(mut stream: TcpStream) {
    let mut buffer = [0; 4096]; //mer buffer desto bättre? FRÅGA T CRIS: är det så för vi vill ju ha en massa workers på vår server, idfk
    
    match stream.read(&mut buffer) {
        Ok(n) => {
            let request = String::from_utf8_lossy(&buffer[..n]);
            println!("Received: {}", request);
            
            let response = "HTTP/1.1 200 OK\r\n\r\ndu har konnektat bror!";
            let _ = stream.write_all(response.as_bytes());
        }
        Err(e) => eprintln!("Read error: {}", e),
    }
}