// Utility functions

import { clock_time } from './timer';

export function CpuTime(): f64 {
  return clock_time() - START_TICKS;
}

export function CpuTime(prev: f64): f64 {
  return CpuTime() - prev;
}

export function sleep(): void {
  // sched_yield is not available in AssemblyScript, so we can use an alternate method to yield execution.
  // One option is to use the atomic_fence instruction, which forces a full memory fence and can be used as a
  // synchronization point.
  atomic_fence();
}

export function sleep(ms: i32): void {
  // usleep is not available in AssemblyScript, so we can use the sleep_ms function from the timer module instead.
  timer.sleep_ms(ms);
}

export function sleep(secs: f64): void {
  // usleep is not available in AssemblyScript, so we can use the sleep_ms function from the timer module instead.
  // The sleep_ms function takes an integer number of milliseconds, so we need to convert the number of seconds
  // to milliseconds and round to the nearest integer.
  timer.sleep_ms(<i32>(secs * 1000.0 + 0.5));
}

export function coreCount(): i32 {
  // thread::hardware_concurrency is not available in AssemblyScript, so we can use the get_core_count function
  // from the system module instead. This returns the number of logical CPU cores available on the system.
  return system.get_core_count();
}