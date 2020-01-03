unit thrds;

interface

uses Windows, SysUtils, Classes, SyncObjs, Variants;//, sets;

type

  IHandle = interface
    ['{A0159BF7-470A-4ABF-9C24-CC5B34EAED81}']
    function Handle: THandle;
  end;

  IMutex = interface
    ['{A8B06463-AC84-4931-AFC2-362486E7BE65}']
    procedure Lock;
    procedure Unlock;
  end;

  ISemaphore = interface
    ['{C5E53D8F-A9EF-4CC9-A05C-10872D1B6376}']
    function Wait: Boolean;
    function Signal: Boolean;
    function Pass: Boolean;
  end;

  IEvent = interface
    ['{8BFAC638-1706-4B63-B695-144555470A73}']
    function Wait: Boolean;
    procedure Signal;
    procedure Reset;
    // function Pulse: Boolean;
  end;

  ILightSwitch = interface
    ['{F306AAA6-4306-4EE3-95F5-8C3003CEEC32}']
    procedure Enter;
    procedure Leave;
  end;

  function Mutex(GetLock:Boolean=False; Name:string=''): IMutex;
  function CriticalSection(GetLock:Boolean=False): IMutex;
  function Semaphore(MaxCount:Integer=1; CurrentCount: Integer=1; Name:string=''): ISemaphore;
  function Event(Signaled:Boolean=False; ManualReset:Boolean=True; Name:string=''): IEvent;
  function LightSwitch(GetLock:Boolean=False; Name:string=''): ILightSwitch;

const
  TIMEOUT:Cardinal = INFINITE;

  SEMAPHORE_ALL_ACCESS = EVENT_ALL_ACCESS;
  SEMAPHORE_MODIFY_STATE = $0002;

var
  SA: TSecurityAttributes;
  SD: TSecurityDescriptor;

implementation

uses utils;

type { TCriticalSection }

  TCriticalSection = class(TInterfacedObject, IMutex)
  private
     CriticalSection : TRTLCriticalSection;
  public
     procedure Lock;
     procedure Unlock;
     //function IMutex.Enter = Lock;
     //function IMutex.Leave = Unlock;
     constructor Create(GetLock:Boolean);
     destructor Destroy;override;
  end;

  function CriticalSection(GetLock:Boolean=False): IMutex;
  begin
    Result := TCriticalSection.Create(GetLock);
  end;

constructor TCriticalSection.Create(GetLock:Boolean);
begin
  inherited Create;
  InitCriticalSection(CriticalSection);
  if GetLock then Lock;
end;

destructor TCriticalSection.Destroy;
begin
  DoneCriticalSection(CriticalSection);
end;

procedure TCriticalSection.Lock;
begin
  EnterCriticalSection(CriticalSection);
end;

procedure TCriticalSection.Unlock;
begin
  LeaveCriticalSection(CriticalSection);
end;

type { THandleObject }
  THandleObject = class(TInterfacedObject, IHandle)
  protected
    fHandle: THandle;
  public
    destructor Destroy; override;
    function Handle: THandle;
  end;

destructor THandleObject.Destroy;
begin
  if FHandle <> 0 then begin
    Check( CloseHandle(FHandle) );
    FHandle := 0;
  end;
end;

function THandleObject.Handle: THandle;
begin
  Result := FHandle;
end;

type { TEvent }
  TEvent = class(TInterfacedObject, IEvent)
  private
    Handle:Pointer;
  public
    function Wait: Boolean;
    procedure Signal;
    procedure Reset;
    // function Pulse: Boolean;
    constructor Create(GetLock: Boolean=False; ManualReset:Boolean=True; const Name: string='');
    destructor Destroy; override;
  end;

constructor TEvent.Create(GetLock: Boolean=False; ManualReset:Boolean=True; const Name: string='');
begin
  Handle := BasicEventCreate(@SA, ManualReset, GetLock, Name);
end;

destructor TEvent.destroy;
begin
  BasicEventDestroy(Handle);
  inherited;
end;

procedure TEvent.Signal;
begin
  BasicEventSetEvent(Handle);
end;

procedure TEvent.Reset;
begin
  BasicEventResetEvent(Handle);
end;

function TEvent.Wait: Boolean;
begin
  Result := TWaitResult(basiceventWaitFor(Timeout, Handle)) = wrSignaled;
  // if Result = wrError then FLastError := GetLastOSError;
end;

function Event(Signaled:Boolean=False; ManualReset:Boolean=True; Name:string=''): IEvent;
begin
  Result := TEvent.Create(Signaled, ManualReset, Name);
end;

type  { TSemaphore }

  TSemaphore = class(THandleObject, ISemaphore, IHandle)
  public
    constructor Create(MaxCount:Integer=1; CurrentCount: Integer=1; Name:string='');
    function Wait: Boolean;
    function Signal: Boolean;
    function Pass: Boolean;
  end;

  function Semaphore(MaxCount:Integer=1; CurrentCount: Integer=1; Name:string=''): ISemaphore;
  begin
    Result := TSemaphore.Create(MaxCount, CurrentCount, Name);
  end;

constructor TSemaphore.Create(MaxCount, CurrentCount: Integer; Name: string);
begin
  FHandle := CreateSemaphore(@SA, CurrentCount, MaxCount, PChar(Name) );
  if (FHandle = 0) and (GetLastError = ERROR_ALREADY_EXISTS) then
    FHandle := OpenSemaphore(SEMAPHORE_ALL_ACCESS or SEMAPHORE_MODIFY_STATE, True, PChar(Name) );
  if (FHandle = 0) then
    CheckLastError;
end;

function TSemaphore.Signal: Boolean;
begin
  Result := ReleaseSemaphore(FHandle, 1, nil);
end;

function TSemaphore.Wait: Boolean;
begin
  Result := WaitForSingleObject(FHandle, Timeout) = WAIT_OBJECT_0;
end;

function TSemaphore.Pass: Boolean;
begin
  Wait;
  Result := Signal;
end;

type { TMutex }

  TMutex = class(TSemaphore, IMutex, IHandle)
  public
    constructor Create(GetLock:Boolean=False; Name:string='');
    { ISemaphore }
    procedure Lock;
    procedure Unlock;
    function Pass: Boolean;
    //function IMutex.Lock = Wait;
    //function IMutex.Unlock = Signal;
    //function IMutex.Enter = Wait;
    //function IMutex.Leave = Signal;
    //function IMutex.Acquire = Wait;
    //function IMutex.Release = Signal;
  end;

function Mutex(GetLock:Boolean=False; Name:string=''): IMutex;
begin
  Result := TMutex.Create(GetLock, Name);
end;

constructor TMutex.Create(GetLock: Boolean=False; Name:string='');
begin
  FHandle := CreateMutex(nil, GetLock, PChar(Name));
  Check(FHandle <> 0);
end;

procedure TMutex.Lock;
begin
  WaitForSingleObject(FHandle, Timeout);
  CheckLastError;
end;

function TMutex.Pass: Boolean;
begin
  Result := WaitForSingleObject(Handle, TimeOut) = WAIT_OBJECT_0;
  if Result then
    Check( ReleaseMutex(Handle) )
  else
    CheckLastError;
end;

procedure TMutex.Unlock;
begin
  Check( ReleaseMutex(Handle) );
end;

type { TLightSwitch }

  TLightSwitch = class(TSemaphore, ILightSwitch)
  private
  public
    constructor Create(GetLock: Boolean=False; Name:string='');
    procedure Enter;
    procedure Leave;
    // function ILightSwitch.Lock = Wait;
    // function ILightSwitch.Unlock = Signal;
    {Threads que usam Enter podem entrar com outras iguais,
    mas usando Lock, só entra sozinha }
  end;

function LightSwitch(GetLock:Boolean=False; Name:string=''): ILightSwitch;
begin
  Result := TLightSwitch.Create(GetLock, Name);
end;

constructor TLightSwitch.Create(GetLock: Boolean=False; Name:string='');
var
  i: Integer;
begin
  if GetLock then i := 0 else i := 1;
  inherited Create(1, i, Name);
end;

var
  Count: Integer = 0;

procedure TLightSwitch.Enter;
begin
  if InterlockedIncrement(Count) = 1 then
    inherited Wait; { Se é a primeira da entrar na sala usando Enter,
                                         trava a porta para estranhos. }
end;

procedure TLightSwitch.Leave;
begin
  if InterlockedDecrement(Count) = 0 then
    inherited Signal; {Ultima Thread deixando a sala, sinaliza que está vazia}
end;

procedure InitSecurityAttributes; {from WinSDK: Sharing Objects with a Service}
begin
  Check( InitializeSecurityDescriptor(@SD, SECURITY_DESCRIPTOR_REVISION) );
  Check( SetSecurityDescriptorDacl(@SD, true, nil, False) );
  SA.nLength              := SizeOf(SA);
  SA.bInheritHandle       := True;
  SA.lpSecurityDescriptor := @SD;
end;

initialization
  InitSecurityAttributes;

finalization

end.
