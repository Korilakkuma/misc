mod channel;
mod client;
mod server;

use crate::channel::{Op, S};
use crate::client::client;
use crate::server::server;
use std::thread;

fn main() {
    let (server_chan, client_chan) = S::session_channel();
    let srv_t = thread::spawn(move || server(server_chan));
    let cli_t = thread::spawn(move || client(client_chan, 11, Op::Even));

    srv_t.join().unwrap();
    cli_t.join().unwrap();

    let (server_chan, client_chan) = S::session_channel();
    let srv_t = thread::spawn(move || server(server_chan));
    let cli_t = thread::spawn(move || client(client_chan, 11, Op::Square));

    srv_t.join().unwrap();
    cli_t.join().unwrap();
}
