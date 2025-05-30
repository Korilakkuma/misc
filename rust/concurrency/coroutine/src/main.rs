use futures::future::{BoxFuture, FutureExt};
use futures::task::{waker_ref, ArcWake};
use std::future::Future;
use std::pin::Pin;
use std::sync::{Arc, Mutex};
use std::task::{Context, Poll};

enum MessageState {
    START,
    SEND,
    END,
}

struct Message {
    state: MessageState,
}

impl Message {
    fn new() -> Self {
        Message {
            state: MessageState::START,
        }
    }
}

impl Future for Message {
    type Output = ();

    fn poll(mut self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<()> {
        match (*self).state {
            MessageState::START => {
                println!("Start message ...");

                (*self).state = MessageState::SEND;

                Poll::Pending
            }
            MessageState::SEND => {
                println!("Send 'Message !'");

                (*self).state = MessageState::END;

                Poll::Pending
            }
            MessageState::END => Poll::Ready(()),
        }
    }
}

struct Task {
    message: Mutex<BoxFuture<'static, ()>>,
}

impl Task {
    fn new() -> Self {
        let message = Message::new();

        Task {
            message: Mutex::new(message.boxed()),
        }
    }
}

impl ArcWake for Task {
    fn wake_by_ref(_arc_self: &Arc<Self>) {}
}

fn main() {
    let task = Arc::new(Task::new());
    let waker = waker_ref(&task);

    let mut ctx = Context::from_waker(&waker);
    let mut message = task.message.lock().unwrap();

    let _ = message.as_mut().poll(&mut ctx);
    let _ = message.as_mut().poll(&mut ctx);
    let _ = message.as_mut().poll(&mut ctx);
}
