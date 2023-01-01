use std::sync::atomic::{AtomicUsize, Ordering};
use std::thread;
use std::time::Duration;
use std::vec::Vec;

fn sleep() {
    thread::yield_now();
}

fn sleep_ms(ms: i32) {
    thread::sleep(Duration::from_millis(ms as u64));
}

fn sleep_secs(secs: f64) {
    thread::sleep(Duration::from_secs_f64(secs));
}

fn core_count() -> usize {
    num_cpus::get()
}

#[macro_export]
macro_rules! wait {
    ($condition:expr) => {
        while !$condition {
            sleep();
        }
    };
}

pub struct Queue<T> {
    buffer: Vec<T>,
    isfree: Vec<char>,
    in_: AtomicUsize,
    out: AtomicUsize,
    mask: usize,
}

impl<T> Queue<T> {
    fn new(size: usize) -> Self {
        let mut mask = 1;
        while mask < size {
            mask *= 2;
        }

        let buffer = vec![0; mask];
        let isfree = vec![1; mask];

        Queue {
            buffer,
            isfree,
            in_: AtomicUsize::new(-1),
            out: AtomicUsize::new(-1),
            mask: mask - 1,
        }
    }

    fn push(&self, item: &T, wait: bool) -> usize {
        let mut i: usize;
        loop {
            i = self.in_.load(Ordering::Acquire);
            if (self.isfull(i) && !wait) || !self.running() {
                return 0;
            } else {
                wait!(!self.isfull(i) || !self.running());
            }

            if self.isfree[i & self.mask]
                && self
                    .in_
                    .compare_exchange(i, i + 1, Ordering::Release, Ordering::Acquire)
                    .is_ok()
            {
                break;
            }
        }

        self.buffer[i & self.mask] = item;
        self.isfree[i & self.mask] = 0;
        i
    }

    fn pop(&self, item: &mut T, wait: bool) -> usize {
        let mut o: usize;
        loop {
            loop {
                o = self.out.load(Ordering::Acquire);
                if self.out.compare_exchange(o, 1, Ordering::Release, Ordering::Acquire).is_ok() {
                    break;
                }
            }

            if (self.isempty(o) && !wait) || !self.running() {
                return 0;
            } else {
                wait!(!self.isempty(o) || !self.running());
            }

            if !self.isfree[o & self.mask]
                && self
                    .out
                    .compare_exchange(o, o + 1, Ordering::Release, Ordering::Acquire)
                    .is_ok()
            {
                break;
            }
        }

        *item = self.buffer[o & self.mask];
        self.isfree[o & self.mask] = 1;
        o
    }

    fn full(&self) -> bool {
        self.isfull(-1)
    }

    fn empty(&self) -> bool {
        self.isempty(-1)
    }

    fn size(&self) -> usize {
        self.in_.load(Ordering::Acquire) - self.out.load(Ordering::Acquire)
    }

    fn counter(&self) -> usize {
        self.out.load(Ordering::Acquire) - 1
    }

private:
    fn isfull(&self, i: isize) -> bool {
        let i = if i < 0 { self.in_.load(Ordering::Acquire) } else { i as usize };
        let i = (i - self.out.load(Ordering::Acquire)) > self.mask;
        if i {
            self.onfull();
        }
        i
    }

    fn isempty(&self, o: isize) -> bool {
      let o = if o < 0 { self.out.load(Ordering::Acquire) } else { o as usize };
      let o = o == self.in_.load(Ordering::Acquire);
      if o {
          self.onempty();
      }
      o
  }

  fn first(&self) -> &T {
      &self.buffer[self.in_.load(Ordering::Acquire) & self.mask]
  }

  fn last(&self) -> &T {
      &self.buffer[self.out.load(Ordering::Acquire) & self.mask]
  }
}
