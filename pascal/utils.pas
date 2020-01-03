unit utils; {J+} {$IFDEF FPC} {$MODE Delphi} {$H+} {$asmmode Intel} {$ENDIF}
interface

uses
  Classes, SysUtils, Windows;

procedure Debug(const s: string); overload;
procedure Debug(const s: string; const Args: array of const); overload;
procedure Error(const Msg: string);
procedure ErrorFmt(const Msg: string; const Args: array of const );
procedure Check(ResultOK: Boolean);
function LastErrorMsg: string;
procedure CheckLastError;
procedure StartTimer;
function Timer(Reset:Boolean=True): Integer;
procedure WriteTimer(Reset:Boolean=True);

procedure IncMax(var i:Integer; const Max:Integer); // inline;

function RDTSC: Int64; assembler;
function CPUCores: Integer;
function CPUSpeed: Integer;

function AtomicInc(var v:Integer; i:Integer=1): Integer; overload;
function AtomicInc(var v:Variant; i:Int64=1): Int64; overload;

function NextPowerOf2(i:Integer): Integer;

implementation

function NextPowerOf2(i:Integer): Integer;
begin
  Dec(i);
  i := i or (i shr  1);
  i := i or (i shr  2);
  i := i or (i shr  4);
  i := i or (i shr  8);
  i := i or (i shr 16);
  Result := i + 1;
end;

function RDTSC: Int64; assembler;
{Read Pentium CPU Time-Stamp Counter - http://www.geocities.com/izenkov/howto-rdtsc.htm }
asm
    RDTSC  // result Int64 in EAX and EDX
end;

function CPUSpeed: Integer;
{http://www.programmingforums.org/post223669.html}
const
  PrevResult:LongWord = 0;
  DelayTime:LongWord = 100; //500;
var
  TimerHi, TimerLo: DWORD;
  PriorityClass, Priority: Integer;
begin

  if PrevResult <> 0 then begin
    Result := PrevResult;
    Exit;
  end;

  PriorityClass := GetPriorityClass(GetCurrentProcess);
  Priority      := GetThreadPriority(GetCurrentThread);
  SetPriorityClass(GetCurrentProcess, REALTIME_PRIORITY_CLASS);
  SetThreadPriority(GetCurrentThread, THREAD_PRIORITY_TIME_CRITICAL);
  Sleep(10);
  asm
    dw 310Fh
    mov TimerLo, eax
    mov TimerHi, edx
  end;
  Sleep(DelayTime);
  asm
    dw 310Fh
    sub eax, TimerLo
    sbb edx, TimerHi
    mov TimerLo, eax
    mov TimerHi, edx
  end;
  SetThreadPriority(GetCurrentThread, Priority);
  SetPriorityClass(GetCurrentProcess, PriorityClass);

  Result := TimerLo div (1000 * DelayTime);
  PrevResult := Result;
end;

function AtomicInc(var v:Integer; i:Integer): Integer;
const
  Inside: Boolean = False;
begin
  while Inside do ;
  Inside := True;
    Result := v;
    v := v + i;
  Inside := False;
end;

function AtomicInc(var v:Variant; i:Int64): Int64;
const
  Inside: Boolean = False;
begin
  while Inside do ;
  Inside := True;
    case TVarData(v).vtype of
      //varsmallint : (vsmallint : smallint);
      //varinteger : (vinteger : longint);
      //varshortint : (vshortint : shortint);
      //varbyte : (vbyte : byte);
      //varword : (vword : word);
      //varlongword : (vlongword : dword);

      varint64 : begin
        Result := TVarData(v).vInt64;
        Inc(TVarData(v).vInt64, i);
      end;

    end;
  Inside := False;
end;


var
  Ticks: Integer;

procedure StartTimer;
begin
  ticks := GetTickCount;
end;

function Timer(Reset:Boolean=True): Integer;
begin
  Result := GetTickCount - ticks;
  if Reset then
    StartTimer;
end;

procedure WriteTimer(Reset:Boolean=True);
begin
  Write(Timer(Reset));
end;

{ Utility functions }

function LastErrorMsg: string;
var
  Buffer: PChar;
  ErrCode: Integer;
begin
  ErrCode := GetLastError;
  if ErrCode <> 0 then begin
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER or FORMAT_MESSAGE_FROM_SYSTEM,
      nil, GetLastError(), (SUBLANG_DEFAULT shl 10) or LANG_NEUTRAL,
      @Buffer, 0, nil);
    Result := Buffer;
    Result := Format('Error %d: %s',[ErrCode, Result]);
    LocalFree(HLOCAL(Buffer));
  end;
end;

procedure Error(const Msg: string);
begin
  raise Exception.Create(Msg);
end;

procedure ErrorFmt(const Msg: string; const Args: array of const );
begin
  raise Exception.CreateFmt(Msg,Args);
end;

procedure CheckLastError;
begin
  if GetLastError <> 0 then
    Error(LastErrorMsg);
end;

procedure Check(ResultOK: Boolean);
begin
  if not ResultOK then
    CheckLastError;
end;

procedure Debug(const s: string);
begin
  OutputDebugString(PChar(Format('Thrd(%u).',[GetCurrentThreadId])+ s));
end;

procedure Debug(const s: string; const Args: array of const); overload;
begin
  Debug(Format(s,args));
end;

procedure IncMax(var i:Integer; const Max:Integer);
begin
  i := i + 1;
  if i > Max then i := 0;
end;

var
  SysInfo: TSystemInfo;

function CPUCores: Integer;
begin
  result := SysInfo.dwNumberOfProcessors;
end;

initialization
  GetSystemInfo(@SysInfo);
end.

