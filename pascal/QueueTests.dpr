{
  This file is part of futureCall Multiprocessing library.
  Copyright (c) 2012, Anderson R. Carli.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License 3
  (GPLv3) published by the Free Software Foundation as stated
  at: http://www.gnu.org/licenses/gpl-3.0.txt
}

program QueueTests;

uses
  Classes, SysUtils, queuetest, fncs, utils;//, thrds, csqueue, lfqueue7, lfqueue0;

const
  threads = 1;
  Mega = 1000*1000;
  MaxCount = 10*Mega;

var
  i,total: Integer;
  p: array [0..threads-1] of TProducer;
  c: array [0..threads-1] of TConsumer;

begin
  // SetPriority(ppCritical);
  // TestQueue(2, 2, 10*Mega, 1024);
  // ReadLn;

  StartTimer;

  for i := 0 to threads-1 do begin
    p[i] := TProducer.Create(MaxCount);
    c[i] := TConsumer.Create();
  end;

  total := 0;
  for i := 0 to threads-1 do begin
    p[i].WaitFor;
    WriteLn('p', i, ': +', FormatFloat(',0',p[i].Total));
    total += p[i].total;
    p[i].free;
  end;

  for i := 0 to threads-1 do begin
    c[i].WaitFor;
    WriteLn('p', i, ': -', FormatFloat(',0',c[i].Total));
    total -= c[i].total;
    c[i].free;
  end;
  WriteLn('---------------');
  WriteLn('   : ', total);

  WriteLn('Rounds/sec: ', FormatFloat(',0',threads*1000*(MaxCount/Timer)));
  ReadLn;
end.

