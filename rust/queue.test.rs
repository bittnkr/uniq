#[test]
fn test_queue() {
    let q = Queue::new(64);
    let mut threads = Vec::new();

    let produced = std::sync::atomic::AtomicUsize::new(0);
    let producer = move |n: usize| {
        let mut i = 0;
        while i <= n && q.push(&(i as i32)) {
            produced.fetch_add(i, Ordering::SeqCst);
            i += 1;
        }
        q.push(&(-1));
    };

    let consumed = std::sync::atomic::AtomicUsize::new(0);
    let consumer = move || {
        let mut v: i32 = 0;
        while q.pop(&mut v) && v != -1 {
            consumed.fetch_add(v as usize, Ordering::SeqCst);
        }
    };

    let t = std::time::Instant::now();

    for _ in 0..num_cpus::get() / 2 {
      threads.push(thread::spawn(move || consumer()));
      threads.push(thread::spawn(move || producer(100_000)));
  }

  for t in threads {
      t.join().unwrap();
  }

  assert!(produced.load(Ordering::SeqCst) != 0);
  assert_eq!(produced.load(Ordering::SeqCst), consumed.load(Ordering::SeqCst));
  // log("Queue:", double(CpuTime(t)));
}

