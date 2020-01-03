unit sets; {$IFDEF FPC} {$MODE Delphi} {$H+} {$ENDIF}
interface

type
  IQueue = interface
    procedure Push(const o: TObject);
    function Pop: TObject;
    function GetCount: Integer;
    function GetSize: Integer;
    // function Empty: Boolean
    property Size:Integer read GetSize;
    property Count:Integer read GetCount;
  end;

  IPool = interface
    procedure Put(const o: TObject);
    function Get: TObject; overload;
    procedure Get(var o); overload;
  end;

  function Queue(Size: Integer): IQueue;
  function Pool(Count: Integer; ObjectClass: TClass): IPool;

implementation

uses csQueue, semqueue, lfqueue0, lfqueue1, lfqueue2, lfqueue3, lfqueue4, lfqueue5, lfqueue6, lfQueue7, setpool, thrds, utils;

function Queue(Size: Integer): IQueue;
begin
  { Tested OK }
  //Result := csqueue.TQueue.Create(size);
  //Result := semqueue.TQueue.Create(size);
  //Result := lfqueue0.TQueue.Create(size);
  Result := lfqueue7.TQueue.Create(size);

  { Fastest but with ABA Problem}
  //Result := lfqueue1.TQueue.Create(size);
  //Result := lfqueue2.TQueue.Create(size);
  //Result := lfqueue3.TQueue.Create(size);
  //Result := lfqueue6.TQueue.Create(size);

  { Trash }
  //Result := lfqueue4.TQueue.Create(size);
  //Result := lfqueue5.TQueue.Create(size);
end;

function Pool(Count: Integer; ObjectClass: TClass): IPool;
var
  i: Integer;
begin
  Result := setpool.TPool.Create(Count);
  for i := 1 to Count do
    Result.Put( ObjectClass.Create );
end;

end.

