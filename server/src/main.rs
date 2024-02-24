use std::{
    collections::HashMap,
    net::SocketAddr,
    sync::{Arc, Mutex},
    io::Error as IoError
};

use futures_channel::mpsc::{unbounded, UnboundedSender};
use futures_util::{future, pin_mut, stream::TryStreamExt, StreamExt};

use tokio::net::{TcpListener, TcpStream};
use tokio_tungstenite::tungstenite::{protocol::Message, handshake::server::Request, http::HeaderValue};

type Tx = UnboundedSender<Message>;
type PeerMap = Arc<Mutex<HashMap<String, Tx>>>;

async fn handle_connection(peer_map: PeerMap, raw_stream: TcpStream, addr: SocketAddr) {
    println!("Incoming TCP connection from: {}", addr);

    let mut client_header: Option<HeaderValue> = Default::default();
    let ws_stream = tokio_tungstenite::accept_hdr_async(raw_stream, |request: &Request, response| {
        client_header = request.headers().get("sec-websocket-protocol").cloned();

        Ok(response)
    })
        .await
        .expect("Error during the websocket handshake occurred");

    if let Some(client_header) = client_header {
        let client_id = client_header.to_str().unwrap().to_string();

        println!("WebSocket connection established: {}", addr);

        let (tx, rx) = unbounded();
        peer_map.lock().unwrap().insert(client_id.clone(), tx);

        let (outgoing, incoming) = ws_stream.split();

        let broadcast_incoming = incoming.try_for_each(|msg| {
            println!("msg received websocket: {}", msg.to_string());

            future::ok(())
        });

        let receive_from_others = rx.map(Ok).forward(outgoing);

        pin_mut!(broadcast_incoming, receive_from_others);
        future::select(broadcast_incoming, receive_from_others).await;

        println!("{} disconnected", addr);
        peer_map.lock().unwrap().remove(&client_id);
    }
}

async fn init_tcp_listener() {
    let addr = "0.0.0.0:8081";
    let state = PeerMap::new(Mutex::new(HashMap::new()));

    let try_socket = TcpListener::bind(addr).await;
    let listener = try_socket.expect("Failed to bind");
    println!("Listening on: {}", addr);

    while let Ok((stream, addr)) = listener.accept().await {
        tokio::spawn(handle_connection(state.clone(), stream, addr));
    }
}

#[tokio::main]
async fn main() -> Result<(), IoError> {
    init_tcp_listener().await;

    Ok(())
}
