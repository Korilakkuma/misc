use futures::future::{BoxFuture, FutureExt};
use futures::task::{waker_ref, ArcWake};
use std::future::Future;
use std::pin::Pin;
use std::sync::mpsc::{sync_channel, Receiver, SyncSender};
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

    fn poll(mut self: Pin<&mut Self>, cx: &mut Context<'_>) -> Poll<()> {
        match (*self).state {
            MessageState::START => {
                println!("Start message ...");

                (*self).state = MessageState::SEND;

                cx.waker().wake_by_ref();

                Poll::Pending
            }
            MessageState::SEND => {
                println!("Send 'Message !'");

                (*self).state = MessageState::END;

                cx.waker().wake_by_ref();

                Poll::Pending
            }
            MessageState::END => Poll::Ready(()),
        }
    }
}

struct Task {
    future: Mutex<BoxFuture<'static, ()>>,
    sender: SyncSender<Arc<Task>>,
}

impl ArcWake for Task {
    fn wake_by_ref(arc_self: &Arc<Self>) {
        let self_0 = arc_self.clone();

        arc_self.sender.send(self_0).unwrap();
    }
}

struct Executor {
    sender: SyncSender<Arc<Task>>,
    receiver: Receiver<Arc<Task>>,
}

struct Spawner {
    sender: SyncSender<Arc<Task>>,
}

impl Spawner {
    fn spawn(&self, future: impl Future<Output = ()> + 'static + Send) {
        let future = future.boxed();

        let task = Arc::new(Task {
            future: Mutex::new(future),
            sender: self.sender.clone(),
        });

        self.sender.send(task).unwrap();
    }
}

impl Executor {
    fn new() -> Self {
        let (sender, receiver) = sync_channel(1024);

        Executor {
            sender: sender.clone(),
            receiver,
        }
    }

    fn get_spawner(&self) -> Spawner {
        Spawner {
            sender: self.sender.clone(),
        }
    }

    fn run(&self) {
        while let Ok(task) = self.receiver.recv() {
            let mut future = task.future.lock().unwrap();
            let waker = waker_ref(&task);
            let mut ctx = Context::from_waker(&waker);

            let _ = future.as_mut().poll(&mut ctx);
        }
    }
}

fn main() {
    let executor = Executor::new();

    executor.get_spawner().spawn(Message::new());

    executor.run();
}
