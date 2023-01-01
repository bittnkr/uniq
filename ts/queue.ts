// Queue • A lock free multi-reader multi-writer circular buffered queue.
import { sleep, coreCount } from './utils';

export class Queue<T> {
  private buffer: Array<T>;
  private isfree: Array<i32>;
  private in: AtomicI32;
  private out: AtomicI32;
  private mask: i32;

  constructor(size: i32 = 1) {
    this.mask = 1;
    while (this.mask < size) this.mask *= 2;
    this.buffer = new Array<T>(this.mask).fill(0);
    this.isfree = new Array<i32>(this.mask).fill(1);
    this.out = new AtomicI32(-1); // start in overflow
    this.in = new AtomicI32(-1); // start in overflow
    this.mask--; // 01000000 => 00111111
  }

  public push(item: T, wait: bool = true): i32 {
    let i: i32;
    do {
      i = this.in.load();
      if ((this.isfull(i) && !wait) || !this.running()) return 0;
    } while (!this.isfree[i & this.mask] || !this.in.compareExchangeWeak(i, i + 1) || !i);

    this.buffer[i & this.mask] = item;
    this.isfree[i & this.mask] = 0;
    return i;
  }

  public pop(item: T, wait: bool = true): i32 {
    let o: i32;
    do {
      do {
        o = this.out.load();
      } while (!o && !this.out.compareExchangeWeak(o, 1)); // skip zero

      if ((this.isempty(o) && !wait) || !this.running()) return 0;
    } while (this.isfree[o & this.mask] || !this.out.compareExchangeWeak(o, o + 1));

    item = this.buffer[o & this.mask];
    this.isfree[o & this.mask] = 1;
    return o;
  }

  public full(): bool {
    return this.isfull(-1);
  }

  public empty(): bool {
    return this.isempty(-1);
  }

  public size(): i32 {
    return this.in.load() - this.out.load();
  }

  public counter(): i32 {
    return this.out.load() - 1;
  }

  private isfull(i: i32 = -1): bool {
    if (i < 0) i = this.in.load();
    i = (i - this.out.load()) > this.mask;
    if (i) this.onfull();
    return i;
  }

  private isempty(o: i32 = -1): bool {
    if (o < 0) o = this.out.load();
    o = o == this.in.load();
    if (o) this.onempty();
    return o;
  }

  private first(): T {
    return this.buffer[this.in.load() & this.mask];
  }

  private last(): T {
    return this.buffer[this.out.load() & this.mask];
  }

  private running(): bool {
    return true; // This function was not implemented in the original code, so I assumed it always returns true.
  }

  private onfull(): void {
    // This function was not implemented in the original code, so I left it empty.
  }

  private onempty(): void {
    // This function was not implemented in the original code, so I left it empty.
  }
}
