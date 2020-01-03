unit lfqueue7; {$IFDEF FPC} {$MODE Delphi} {$H+} {$ENDIF}
interface

uses
  Classes, SysUtils;

type { TQueue }

  TQueue = class(TInterfacedObject, IQueue)
  private
    Buffer: array of TObject;
    FSize: Integer;
    Mask: Integer;
    Head: Integer;
    Tail: Integer;
    function GetCount: Integer;
    function GetSize: Integer;
  public
    constructor Create(size: Integer);
    procedure Push(const o: TObject);
    function Pop: TObject;
    property Size:Integer read GetSize;
    property Count:Integer read GetCount;
  end;

implementation

uses utils;

constructor TQueue.Create(size: Integer = 64);
begin
  FSize := size; //NextPowerOf2(size);
  SetLength(Buffer, FSize);
  FillDWord(Buffer[0], FSize, 0);
  Mask := FSize-1;
end;

function CompareAndSet(var Dest:Integer; const Compare, NewValue:Integer):Boolean; inline;
begin
  Result := InterlockedCompareExchange(Dest, NewValue, Compare) = Compare;
end;

procedure TQueue.Push(const o: TObject);
var
  t: Integer;
begin
  Assert(o<>nil, 'TQueue.Push(): Object cannot be nil.');
  repeat
    t := Tail;
    while (t-Head) = FSize do Sleep(0); // full
  until (Buffer[t and Mask]=nil) and CompareAndSet(Tail, t, t+1);

  Buffer[t and Mask] := o;
end;

function TQueue.Pop: TObject;
var
  h: Integer;
begin
  repeat
    h := Head;
    while h = Tail do Sleep(0); // empty
  until (Buffer[h and Mask]<>nil) and CompareAndSet(Head, h, h+1);

  h := h and Mask;
  Result := Buffer[h];
  Buffer[h] := nil;
end;

function TQueue.GetCount: Integer;
begin
  Result := Tail-Head;
end;

function TQueue.GetSize: Integer;
begin
  Result := FSize;
end;

end.
