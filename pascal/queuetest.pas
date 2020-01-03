unit queuetest; //{$IFDEF FPC} {$MODE Delphi}{$H+} {$ENDIF}
interface

uses
  Classes, SysUtils, Math, sets, fncs;

type
  TProducer = class(TThread)
    Total, Count: Integer;
    Priority: TProcessPriority;
    constructor Create(aCount: Integer; pp:TProcessPriority=ppNormal);
    procedure Execute; override;
  end;

  TConsumer = class(TThread)
    Total: Integer;
    Priority: TProcessPriority;
    constructor Create(pp:TProcessPriority=ppNormal);
    procedure Execute; override;
  end;

var
  Q: IQueue;

implementation

{ TProducer }

constructor TProducer.Create(aCount: Integer; pp:TProcessPriority);
begin
  Priority := pp;
  Count := aCount;
  inherited Create(False);
end;

procedure TProducer.Execute;
var
  i, o: Integer;
begin
  SetPriority(Priority);
  { Push [Count] integers into queue }
  for i := 0 to Count-1 do begin
    o := (i and $F)+1; //Random(High(Integer));
    Q.Push(TObject(o));
    Inc(Total, o);
  end;

  { Sinalize end of job end pushing a -1 into the queue }
  Q.Push( TObject(-1) );
end;

{ TConsumer }

constructor TConsumer.Create(pp:TProcessPriority);
begin
  Priority := pp;
  inherited Create(False);
end;

procedure TConsumer.Execute;
var
  o: Integer;
begin
  SetPriority(Priority);
  { Pop and sum numbers from the queue until found a -1 }
  Total := 0;
  repeat
    o := Integer(Q.Pop);
    Inc(Total, o);
  until o < 0;

  { Remove the -1 flag from Total }
  Inc(Total, 1);
end;

initialization
   //Q := Queue(16*1024);
   //Q := Queue(1024);
   //Q := Queue(256);
   //Q := Queue(128);
   Q := Queue(64);    // Melhor performance de queue0 (28M/s)
   //Q := Queue(32);
   //Q := Queue(16);
   //Q := Queue(8);
   //Q := Queue(1);

end.
