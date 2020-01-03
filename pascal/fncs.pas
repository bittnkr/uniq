unit fncs; {$IFDEF FPC} {$MODE Delphi} {$H+} {$asmmode Intel} {$ENDIF}

interface

type
  TProcessPriority = (ppIdle, ppLow, ppNormal, ppHigh, ppCritical);

function SetPriority(p: TProcessPriority): TProcessPriority;

function CPUTime: Int64; assembler;
function CPUCores: Integer;
function CPUFrequency(ForceCalc: Boolean=False): Int64;
procedure StartTimer;
function Timer(Reset:Boolean=False): Double;

implementation

uses SysUtils, Windows;

var
  CPUCounter: Int64;

function CPUCores: Integer;
var
  SysInfo: TSystemInfo;
begin
  GetSystemInfo(@SysInfo);
  Result := SysInfo.dwNumberOfProcessors;
end;

function CPUTime: Int64; assembler;
{Read Pentium CPU Time-Stamp Counter }
asm
    RDTSC  // result Int64 in EAX and EDX
end;

function GetPriority: TProcessPriority;
begin
  case GetThreadPriority(GetCurrentThread) of
    THREAD_PRIORITY_IDLE:          Result := ppIdle;
    THREAD_PRIORITY_HIGHEST:       Result := ppHigh;
    THREAD_PRIORITY_LOWEST:        Result := ppLow;
    THREAD_PRIORITY_NORMAL:        Result := ppNormal;
    THREAD_PRIORITY_TIME_CRITICAL: Result := ppCritical;
  end;
end;

function SetPriority(p: TProcessPriority): TProcessPriority;
const
  TP: array [TProcessPriority] of Integer = (THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL);
  //PC: array [TProcessPriority] of Integer = (IDLE_PRIORITY_CLASS,  NORMAL_PRIORITY_CLASS,  NORMAL_PRIORITY_CLASS,  HIGH_PRIORITY_CLASS,     REALTIME_PRIORITY_CLASS);
begin
  Result := GetPriority;
  if p <> Result then begin
    // SetPriorityClass(GetCurrentProcess, PC[p]);
    SetThreadPriority(GetCurrentThread, TP[p]);
    Sleep(1);
  end;
end;

function CPUFrequency(ForceCalc: Boolean=False): Int64;
const
  PrevResult: Int64 = 0;
var
  pp: TProcessPriority;
  i: Integer;
begin
  if (PrevResult = 0) or ForceCalc then begin
    pp := SetPriority(ppCritical);
    for i := 1 to 20 do begin
      { take 20 measures of 10ms each, and keeps the higher }
      Result := CPUTime;
      Sleep(10);
      Result := 100 * (CPUTime - Result);
      if Result > PrevResult then
        PrevResult := Result;
    end;
    SetPriority(pp);
  end;
  Result := PrevResult;
end;

procedure StartTimer;
begin
  CPUCounter := CPUTime;
end;

function Timer(Reset:Boolean): Double;
begin
  Result := (CPUTime - CPUCounter) / CPUFrequency;
  if Reset then StartTimer;
end;

end.
