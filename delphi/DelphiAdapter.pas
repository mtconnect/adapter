unit DelphiAdapter;

interface
  type TAdapter = class
    procedure addDatum(aDatum: Pointer); virtual; stdcall; abstract;
    procedure startServer; virtual; stdcall; abstract;
    function startServerThread(): Boolean; virtual; stdcall; abstract;
    procedure sendChangedData; virtual; stdcall; abstract;
    procedure flush; virtual; stdcall; abstract;
    procedure timestamp; virtual; stdcall; abstract;
    procedure unavailable; virtual; stdcall; abstract;
    procedure stopServer; virtual; stdcall; abstract;
    procedure free; virtual; stdcall; abstract;
  end;

  function NewAdapter(aPort: integer): TAdapter; cdecl;

  type TEvent = class
    function getDatum() : Pointer; virtual; stdcall; abstract;
    procedure setValue(aValue: PChar); virtual; stdcall; abstract;
    function getValue() : PChar; virtual; stdcall; abstract;
    procedure free(); virtual; stdcall; abstract;
  end;

  function NewEvent(aName: PChar): TEvent; cdecl;

  type TSample = class
    function getDatum() : Pointer; virtual; stdcall; abstract;
    procedure setValue(aValue: Double); virtual; stdcall; abstract;
    function getValue() : Double; virtual; stdcall; abstract;
    procedure free(); virtual; stdcall; abstract;
  end;

  function NewSample(aName: PChar): TSample; cdecl;

 type
  TExecutionState = (
    ExecutionState_UNAVAILABLE,
    ExecutionState_READY,
    ExecutionState_INTERRUPTED,
    ExecutionState_STOPPED,
    ExecutionState_ACTIVE);

  TExecution = class
    function getDatum() : Pointer; virtual; stdcall; abstract;
    procedure setValue(aValue: TExecutionState); virtual; stdcall; abstract;
    function getValue() : TExecutionState; virtual; stdcall; abstract;
    procedure free(); virtual; stdcall; abstract;
  end;

  function NewExecution(aName: PChar): TExecution; cdecl;

 type
  TControllerModes = (
    ControllerMode_UNAVAILABLE,
    ControllerMode_AUTOMATIC,
    ControllerMode_MANUAL,
    ControllerMode_MANUAL_DATA_INPUT,
    ControllerMode_SEMI_AUTOMATIC);

  TControllerMode = class
    function getDatum() : Pointer; virtual; stdcall; abstract;
    procedure setValue(aValue: TControllerModes); virtual; stdcall; abstract;
    function getValue() : TControllerModes; virtual; stdcall; abstract;
    procedure free(); virtual; stdcall; abstract;
  end;

  function NewControllerMode(aName: PChar): TControllerMode; cdecl;

  type
   TRotationDirection = (
   Direction_UNAVAILABLE,
   Direction_CLOCKWISE,
   Direction_COUNTER_CLOCKWISE);

   TDirection = class
     function getDatum() : Pointer; virtual; stdcall; abstract;
     procedure setValue(aValue: TRotationDirection); virtual; stdcall; abstract;
     function getValue() : TRotationDirection; virtual; stdcall; abstract;
     procedure free(); virtual; stdcall; abstract;
   end;

   function NewDirection(aName: PChar): TDirection; cdecl;

 type
  TEStop = (
    EmergencyStop_UNAVAILABLE,
    EmergencyStop_TRIGGERED,
    EmergencyStop_ARMED);

  TEmergencyStop = class
    function getDatum() : Pointer; virtual; stdcall; abstract;
    procedure setValue(aValue: TEStop); virtual; stdcall; abstract;
    function getValue() : TEStop; virtual; stdcall; abstract;
    procedure free(); virtual; stdcall; abstract;
  end;

  function NewEmergencyStop(aName: PChar): TEmergencyStop; cdecl;

  type
   TCoupling = (
     AxisCoupling_UNAVAILABLE,
     AxisCoupling_TANDEM,
     AxisCoupling_SYNCHRONOUS,
     AxisCoupling_MASTER,
     AxisCoupling_SLAVE
     );

   TAxisCoupling = class
     function getDatum() : Pointer; virtual; stdcall; abstract;
     procedure setValue(aValue: TCoupling); virtual; stdcall; abstract;
     function getValue() : TCoupling; virtual; stdcall; abstract;
     procedure free(); virtual; stdcall; abstract;
   end;

   function NewAxisCoupling(aName: PChar): TAxisCoupling; cdecl;
      
  type
   TDoorStates = (
     DoorState_UNAVAILABLE,
     DoorState_OPEN,
     DoorState_CLOSED);

   TDoorState = class
     function getDatum() : Pointer; virtual; stdcall; abstract;
     procedure setValue(aValue: TDoorStates); virtual; stdcall; abstract;
     function getValue() : TDoorStates; virtual; stdcall; abstract;
     procedure free(); virtual; stdcall; abstract;
   end;

   function NewDoorState(aName: PChar): TDoorState; cdecl;

 type
  TPathModes = (
         PathMode_UNAVAILABLE,
         PathMode_INDEPENDENT,
         PathMode_SYNCHRONOUS,
         PathMode_MIRROR);
    
  TPathMode = class
    function getDatum() : Pointer; virtual; stdcall; abstract;
    procedure setValue(aValue: TPathModes); virtual; stdcall; abstract;
    function getValue() : TPathModes; virtual; stdcall; abstract;
    procedure free(); virtual; stdcall; abstract;
  end;

  function NewPathMode(aName: PChar): TPathMode; cdecl;

  type
   TRotaryModes = (
         RotaryMode_UNAVAILABLE,
         RotaryMode_SPINDLE,
         RotaryMode_INDEX,
         RotaryMode_CONTOUR);

   TRotaryMode = class
     function getDatum() : Pointer; virtual; stdcall; abstract;
     procedure setValue(aValue: TRotaryModes); virtual; stdcall; abstract;
     function getValue() : TRotaryModes; virtual; stdcall; abstract;
     procedure free(); virtual; stdcall; abstract;
   end;

   function NewRotaryMode(aName: PChar): TRotaryMode; cdecl;


  type
   TConditionLevels = (
     Condition_UNAVAILABLE,
     Condition_NORMAL,
     Condition_WARNING,
     Condition_FAULT);

   TCondition = class
     function getDatum() : Pointer; virtual; stdcall; abstract;
     procedure setValue(aLevel: TConditionLevels; aText: PChar; aCode: PChar;
   		                  aQualifier: PChar; aSeverity: PChar); virtual; stdcall; abstract;
     procedure free(); virtual; stdcall; abstract;
   end;

   function NewCondition(aName: PChar): TCondition; cdecl;

 type
  TMessage = class
    function getDatum() : Pointer; virtual; stdcall; abstract;
    procedure setValue(aText: PChar; aCode: PChar); virtual; stdcall; abstract;
    procedure free(); virtual; stdcall; abstract;
  end;

  function NewMessage(aName: PChar): TMessage; cdecl;

  type
   TAvailability = class
     function getDatum() : Pointer; virtual; stdcall; abstract;
     procedure available(); virtual; stdcall; abstract;
     procedure unavailable(); virtual; stdcall; abstract;
     procedure free(); virtual; stdcall; abstract;
   end;

   function NewAvailability(aName: PChar): TAvailability; cdecl;


implementation

  function NewAdapter(aPort: integer): TAdapter; external 'DelphiAdapter.dll';
  function NewEvent(aName: PChar): TEvent; external 'DelphiAdapter.dll';
  function NewSample(aName: PChar): TSample; external 'DelphiAdapter.dll';
  function NewExecution(aName: PChar): TExecution; external 'DelphiAdapter.dll';
  function NewControllerMode(aName: PChar): TControllerMode; external 'DelphiAdapter.dll';
  function NewDirection(aName: PChar): TDirection; external 'DelphiAdapter.dll';
  function NewEmergencyStop(aName: PChar): TEmergencyStop; external 'DelphiAdapter.dll';
  function NewAxisCoupling(aName: PChar): TAxisCoupling; external 'DelphiAdapter.dll';
  function NewDoorState(aName: PChar): TDoorState; external 'DelphiAdapter.dll';
  function NewPathMode(aName: PChar): TPathMode; external 'DelphiAdapter.dll';
  function NewRotaryMode(aName: PChar): TRotaryMode; external 'DelphiAdapter.dll';
  function NewCondition(aName: PChar): TCondition; external 'DelphiAdapter.dll';
  function NewMessage(aName: PChar): TMessage; external 'DelphiAdapter.dll';
  function NewAvailability(aName: PChar): TAvailability; external 'DelphiAdapter.dll';

end.



