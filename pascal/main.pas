unit main; {$IFDEF FPC} {$MODE Delphi}{$H+} {$ENDIF}
interface

uses
  Classes, SysUtils, sets, thrds;

  function TestQueue(queueSize, producersCount, consumersCount, itemsCount:Integer): Integer;

implementation

uses utils, variants;

procedure ProducerProc(const input:Variant; var output:Variant);
var
  i, o, count, total: Integer;
  queue: IQueue;
begin
  queue := input[0];
  count := input[1]-1;

  { Push [Count] integers into queue }
  total := 0;
  for i := 0 to Count do begin
    o := i and $F;
    Inc(total, o);

    queue.Push( TObject(o) );
  end;

  Result := total;
end;

procedure ConsumerProc(const input:Variant; var output:Variant);
var
  o, count, total: Integer;
  queue: IQueue;
begin
  queue := input[0];
  count := input[1]-1;

  { Pop and sum numbers from the queue until thread termination }
  total := 0;
  repeat
    o := Integer(queue.Pop);
    Inc(Total, o);
  until CurrentThread.Terminated;

  Result := Total;
end;

function TestQueue(queueSize, producersCount, consumersCount, itemsCount:Integer): Integer;
var
  q: IQueue;
  t: IThread;

  producers,
  consumers:IPool;
begin
  q := Queue(queueSize);

  producers := ThreadPool(ProducerProc, VarArrayOf([q, itemsCount]), producersCount );
  consumers := ThreadPool(ConsumerProc, VarArrayOf([q, itemsCount]), consumersCount );

  StartTimer;

  { Wait for ProducerProc threads termination }
  Wait(producers);

  { Wait for ConsumerProc threads termination (wich will happen when queue is empty) }
  Wait(consumers, q.Empty );

  { Show the sum of elements pushed by producers threads }
  for t in producers do begin
    Writeln('p(', t.Id, '): ', FormatFloat(',0', t.Result) );
    Inc(Result, Integer(t.result) );
  end;

  { Show sum of elements Popped by consumers threads (should be equal producers sum) }
  for t in consumers do begin
    Writeln('p(', t.Id, '): ', FormatFloat(',0', t.result) );
    Dec(Result, Integer(t.result) );
  end;

  WriteLn('---------------');
  WriteLn('           : ',  Result); { Should be allways zero }

  WriteLn('Push+Pop/sec: ', FormatFloat(',0',2*1000*(itemsCount/Timer)));
end;

end.
