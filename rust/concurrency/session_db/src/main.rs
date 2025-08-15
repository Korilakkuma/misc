#[macro_use]
extern crate session_types;

mod channel;
mod db_client;
mod db_server;
mod db_server_macro;

use crate::channel::S;
use crate::db_client::db_client;
use crate::db_server::db_server;
use crate::db_server_macro::db_server_macro;
use std::thread;

fn main() {
    let (server_chan, client_chan) = S::session_channel();

    let srv_t = thread::spawn(move || db_server(server_chan));
    let cli_t = thread::spawn(move || db_client(client_chan));

    srv_t.join().unwrap();
    cli_t.join().unwrap();

    println!("Use macro ----------");

    // Use macro
    let (server_chan, client_chan) = S::session_channel();

    let srv_t = thread::spawn(move || db_server_macro(server_chan));
    let cli_t = thread::spawn(move || db_client(client_chan));

    srv_t.join().unwrap();
    cli_t.join().unwrap();
}
