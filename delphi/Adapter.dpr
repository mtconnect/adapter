program DepphiAdapter;

{$APPTYPE CONSOLE}

uses
  MTConnect, Windows, SysUtils;
var
  Connector: TMTConnector;

begin
  Connector := TMTConnector.Create;
  Windows.Sleep(10000);
end.
