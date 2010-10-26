program DepphiAdapter;

{$APPTYPE CONSOLE}

uses
  DelphiAdapter, Windows, SysUtils;
var
  Adapter: TAdapter;
  XPosition: TSample;
  ProgramName: TEvent;
  Exec: TExecution;
  Mode: TControllerMode;
  Avail: TAvailability;
  OverTemp: TCondition;

  i: Integer;

begin
  Adapter := NewAdapter(7878);
  XPosition := NewSample('Xpos');
  Adapter.addDatum(XPosition);
  Exec := NewExecution('exec');
  Adapter.addDatum(Exec);
  Avail := NewAvailability('avail');
  Adapter.addDatum(Avail);
  ProgramName := NewEvent('program');
  Adapter.addDatum(ProgramName);
  OverTemp := NewCondition('over_temp');
  Adapter.addDatum(OverTemp);
  Mode := NewControllerMode('mode');
  Adapter.addDatum(Mode);
  Adapter.startServerThread;

  ProgramName.setValue('prog.iso');
  Mode.setValue(ControllerMode_MANUAL);

  for i := 1 to 120 Do
  Begin
    Writeln(i);
    Adapter.timestamp;
    XPosition.setValue(i);

    if (i = 5)
    Then Mode.setValue(ControllerMode_AUTOMATIC);

    if ((i mod 4) = 0)
    Then Exec.setValue(ExecutionState_READY)
    else Exec.setValue(ExecutionState_ACTIVE);

    if (i = 10)
    Then OverTemp.setValue(Condition_FAULT, 'Overtemp Fault', '1123', 'HIGH', '');

    if (i = 12)
    Then OverTemp.setValue(Condition_NORMAL, '', '', '', '');

    Adapter.sendChangedData;
    Windows.Sleep(1000);
  end;

  Adapter.unavailable;
  Adapter.stopServer;
end.
