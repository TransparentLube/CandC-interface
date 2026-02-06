use std::io::{Read, Write};
use std::net::TcpStream;

fn main() {
    let mut stream = TcpStream::connect("127.0.0.1:8080")
        .expect("failad connection");

    let message = "tjenerix";
    stream
        .write_all(message.as_bytes())
        .expect("kunde inte skriva till servern");

    let mut buffer = [0; 4096]; // igen samma sak för storleken på denna buffer @cris
    match stream.read(&mut buffer) {
        Ok(n) => {
            let response = String::from_utf8_lossy(&buffer[..n]);
            println!("svar från servern: {}", response);
        }
        Err(e) => eprintln!("faila att läsa från servern: {}", e),
    }
}