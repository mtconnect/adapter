' Source: http://www.pcdmisforum.com/showthread.php?3840-PC-DMIS-to-Excel

Sub main()

  'Dim app As PCDLRN.Application
  'Dim pp As PCDLRN.PartPrograms
  'Dim cmds As PCDLRN.Commands
  'Dim cmd As PCDLRN.Command
  'Dim part As PCDLRN.PartProgram
  
  Dim app As Object 'PCDLRN.Application
  Dim pp As Object ' PCDLRN.PartPrograms
  Dim cmds As Object 'PCDLRN.Commands
  Dim cmd As Object 'PCDLRN.Command
  Dim part As Object 'PCDLRN.PartProgram

 Dim DIMENSION_TRUE_START_POSITION: DIMENSION_TRUE_START_POSITION = 1200
  Dim DIMENSION_START_LOCATION: DIMENSION_START_LOCATION = 1000
Dim DIMENSION_X_LOCATION: DIMENSION_X_LOCATION = 1002
           Dim DIMENSION_R_LOCATION: DIMENSION_R_LOCATION = 1006
             Dim DIMENSION_Y_LOCATION: DIMENSION_Y_LOCATION = 1003
             Dim DIMENSION_Z_LOCATION: DIMENSION_Z_LOCATION = 1004
             Dim DIMENSION_D_LOCATION: DIMENSION_D_LOCATION = 1005
             Dim DIMENSION_T_LOCATION: DIMENSION_T_LOCATION = 1008
             Dim DIMENSION_L_LOCATION: DIMENSION_L_LOCATION = 1012
             Dim DIMENSION_TRUE_DIAM_LOCATION: DIMENSION_TRUE_DIAM_LOCATION = 1209
             Dim DIMENSION_TRUE_D1_LOCATION: DIMENSION_TRUE_D1_LOCATION = 1214
             Dim DIMENSION_TRUE_D2_LOCATION: DIMENSION_TRUE_D2_LOCATION = 1215
             Dim DIMENSION_TRUE_D3_LOCATION: DIMENSION_TRUE_D3_LOCATION = 1216
             Dim DIMENSION_TRUE_DD_LOCATION: DIMENSION_TRUE_DD_LOCATION = 1205
             Dim DIMENSION_TRUE_DF_LOCATION: DIMENSION_TRUE_DF_LOCATION = 1206
             Dim DIMENSION_TRUE_END_POSITION: DIMENSION_TRUE_END_POSITION = 1201
             Dim DIMENSION_TRUE_LD_LOCATION: DIMENSION_TRUE_LD_LOCATION = 1210
             Dim DIMENSION_TRUE_LF_LOCATION: DIMENSION_TRUE_LF_LOCATION = 1212
             Dim DIMENSION_TRUE_PA_LOCATION: DIMENSION_TRUE_PA_LOCATION = 1208
             Dim DIMENSION_TRUE_PR_LOCATION: DIMENSION_TRUE_PR_LOCATION = 1207
             Dim DIMENSION_TRUE_WD_LOCATION: DIMENSION_TRUE_WD_LOCATION = 1211
             Dim DIMENSION_TRUE_WF_LOCATION: DIMENSION_TRUE_WF_LOCATION = 1213
             Dim DIMENSION_TRUE_X_LOCATION: DIMENSION_TRUE_X_LOCATION = 1202
             Dim DIMENSION_TRUE_Y_LOCATION: DIMENSION_TRUE_Y_LOCATION = 1203
             Dim DIMENSION_TRUE_Z_LOCATION: DIMENSION_TRUE_Z_LOCATION = 1204
             Dim DIMENSION_V_LOCATION: DIMENSION_V_LOCATION = 1011
             Dim DIMENSION_FLATNESS: DIMENSION_FLATNESS = 1018
             Dim DIMENSION_KEYIN: DIMENSION_KEYIN = 1113
             Dim DIMENSION_PA_LOCATION: DIMENSION_PA_LOCATION = 1010
             Dim DIMENSION_PR_LOCATION: DIMENSION_PR_LOCATION = 1009
            Dim DIMENSION_2D_DISTANCE: DIMENSION_2D_DISTANCE = 1107
             Dim DIMENSION_3D_ANGLE: DIMENSION_3D_ANGLE = 1108
             Dim DIMENSION_3D_DISTANCE: DIMENSION_3D_DISTANCE = 1106
            Dim DIMENSION_A_LOCATION: DIMENSION_A_LOCATION = 1007
             Dim DIMENSION_ANGULARITY: DIMENSION_ANGULARITY = 1112
             Dim DIMENSION_COAXIALITY: DIMENSION_COAXIALITY = 1114
             Dim DIMENSION_CONCENTRICITY: DIMENSION_CONCENTRICITY = 1111
             Dim DIMENSION_PARALLELISM: DIMENSION_PARALLELISM = 1104
             Dim DIMENSION_PERPENDICULARITY: DIMENSION_PERPENDICULARITY = 1103
             Dim DIMENSION_PROFILE: DIMENSION_PROFILE = 1105
             Dim DIMENSION_ROUNDNESS: DIMENSION_ROUNDNESS = 1019
            Dim DIMENSION_RUNOUT: DIMENSION_RUNOUT = 1110
             Dim DIMENSION_STRAIGHTNESS: DIMENSION_STRAIGHTNESS = 1100
  '           Dim DIMENSION_TRUE_END_POSITION: DIMENSION_TRUE_END_POSITION = 1201
             Dim DIMENSION_END_LOCATION: DIMENSION_END_LOCATION = 1001
  Set app = CreateObject("PCDLRN.Application")
  Set part = app.ActivePartProgram
  Set cmds = part.Commands
   
  Dim x As Integer
  Dim y As Integer
  Dim a As Integer
  Dim b As Integer
  
  Dim PartName As String
  'RSet PartName
  
  Dim LookForChar
  LookForChar = "."
  Dim Pos
  Dim s As String
  Dim i As Integer
  Dim Run As Integer
  Dim Offset As Integer
  Dim r As Integer
  s = ""
  
 
   Run = ActiveSheet.Cells(1, 9)
   Offset = ActiveSheet.Cells(2, 9)
   
     
   
   Run = Run + 1
   ActiveSheet.Cells(1, 9) = Run
   
  
  i = 8
  r = 1
  
               
  
  For Each cmd In cmds
    If cmd.IsDimension Then
        If cmd.Type = (DIMENSION_TRUE_START_POSITION Or DIMENSION_START_LOCATION) Or s <> cmd.Id Then
            i = i + 1
            
            If Run < 39 Then
            
                For r = 1 To 39
               
                ActiveSheet.Cells(i, r).Font.Bold = True
                Next r
               
               ActiveSheet.Cells(i, 1).ColumnWidth = 11.43
               ActiveSheet.Cells(i, 1) = "Location of "
               
               ActiveSheet.Cells(i, 2).HorizontalAlignment = xlLeft
               ActiveSheet.Cells(i, 2) = cmd.DimensionCommand.feat1
               
               
            End If
            
               
            r = 0
            
            i = i + 1
            
            If Run < 39 Then
            
               
               ActiveSheet.Cells(i, 1).Font.Bold = True
               ActiveSheet.Cells(i, 1) = "AXIS"
               
               ActiveSheet.Cells(i, 2).Font.Bold = True
               ActiveSheet.Cells(i, 2) = "NOMINAL"
               
               ActiveSheet.Cells(i, 3).Font.Bold = True
               ActiveSheet.Cells(i, 3) = "UT"
               
               ActiveSheet.Cells(i, 4).Font.Bold = True
               ActiveSheet.Cells(i, 4) = "LT"
               
            
                        
            End If
            
            ActiveSheet.Cells(i, (Offset + 6)).Font.Bold = True
            ActiveSheet.Cells(i, (Offset + 6)) = "MEAS"
            
            ActiveSheet.Cells(i, (Offset + 7)).Font.Bold = True
            ActiveSheet.Cells(i, (Offset + 7)) = "DEV"
            
            ActiveSheet.Cells(i, (Offset + 8)).Font.Bold = True
            ActiveSheet.Cells(i, (Offset + 8)) = "OOT"
            
                       
            
            r = 0
            
            
                
            s = cmd.Id
            If cmd.Type <> DIMENSION_TRUE_START_POSITION And cmd.Type <> DIMENSION_START_LOCATION Then
             i = i + 1
             
             If Run < 2 Then
             
                ActiveSheet.Cells(i, 1).Font.Bold = True
                          
          Select Case cmd.Type
             
             Case DIMENSION_X_LOCATION
                            ActiveSheet.Cells(i, 1) = "X"
             Case DIMENSION_R_LOCATION
                            ActiveSheet.Cells(i, 1) = "R"
             Case DIMENSION_Y_LOCATION
                             ActiveSheet.Cells(i, 1) = "Y"
             Case DIMENSION_Z_LOCATION
                            ActiveSheet.Cells(i, 1) = "Z"
             Case DIMENSION_D_LOCATION
                            ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_T_LOCATION
                            ActiveSheet.Cells(i, 1) = "T"
             Case DIMENSION_L_LOCATION
                          ActiveSheet.Cells(i, 1) = "L"
             Case DIMENSION_TRUE_DIAM_LOCATION
                           ActiveSheet.Cells(i, 1) = "TP"
             Case DIMENSION_TRUE_D1_LOCATION
                           ActiveSheet.Cells(i, 1) = "D1"
             Case DIMENSION_TRUE_D2_LOCATION
                           ActiveSheet.Cells(i, 1) = "D2"
             Case DIMENSION_TRUE_D3_LOCATION
                           ActiveSheet.Cells(i, 1) = "D3"
             Case DIMENSION_TRUE_DD_LOCATION
                           ActiveSheet.Cells(i, 1) = "DD"
             Case DIMENSION_TRUE_DF_LOCATION
                           ActiveSheet.Cells(i, 1) = "DF"
             Case DIMENSION_TRUE_END_POSITION
                           ActiveSheet.Cells(i, 1) = "END"
             Case DIMENSION_TRUE_LD_LOCATION
                           ActiveSheet.Cells(i, 1) = "LD"
             Case DIMENSION_TRUE_LF_LOCATION
                           ActiveSheet.Cells(i, 1) = "LF"
             Case DIMENSION_TRUE_PA_LOCATION
                           ActiveSheet.Cells(i, 1) = "PA"
             Case DIMENSION_TRUE_PR_LOCATION
                           ActiveSheet.Cells(i, 1) = "PR"
             Case DIMENSION_TRUE_START_POSITION
                           ActiveSheet.Cells(i, 1) = "START"
             Case DIMENSION_TRUE_WD_LOCATION
                           ActiveSheet.Cells(i, 1) = "WD"
             Case DIMENSION_TRUE_WF_LOCATION
                           ActiveSheet.Cells(i, 1) = "WF"
             Case DIMENSION_TRUE_X_LOCATION
                           ActiveSheet.Cells(i, 1) = "X"
             Case DIMENSION_TRUE_Y_LOCATION
                           ActiveSheet.Cells(i, 1) = "Y"
             Case DIMENSION_TRUE_Z_LOCATION
                           ActiveSheet.Cells(i, 1) = "Z"
             Case DIMENSION_V_LOCATION
                           ActiveSheet.Cells(i, 1) = "V"
             Case DIMENSION_FLATNESS
                           ActiveSheet.Cells(i, 1) = "F"
             Case DIMENSION_KEYIN
                           ActiveSheet.Cells(i, 1) = "K"
             Case DIMENSION_PA_LOCATION
                           ActiveSheet.Cells(i, 1) = "PA"
             Case DIMENSION_PR_LOCATION
                           ActiveSheet.Cells(i, 1) = "PR"
             Case DIMENSION_2D_DISTANCE
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_3D_ANGLE
                           ActiveSheet.Cells(i, 1) = "A"
             Case DIMENSION_3D_DISTANCE
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_A_LOCATION
                           ActiveSheet.Cells(i, 1) = "A"
             Case DIMENSION_ANGULARITY
                           ActiveSheet.Cells(i, 1) = "A"
             Case DIMENSION_COAXIALITY
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_CONCENTRICITY
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_D_LOCATION
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_PARALLELISM
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_PERPENDICULARITY
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_PROFILE
                           ActiveSheet.Cells(i, 1) = "P"
             Case DIMENSION_ROUNDNESS
                           ActiveSheet.Cells(i, 1) = "R"
             Case DIMENSION_RUNOUT
                           ActiveSheet.Cells(i, 1) = "R"
             Case DIMENSION_STRAIGHTNESS
                           ActiveSheet.Cells(i, 1) = "S"

                            
             Case Else
                        ActiveSheet.Cells(i, 1) = "M"
                               
             End Select
             
          End If
                
            If Run < 39 Then
            ActiveSheet.Cells(i, 2) = cmd.DimensionCommand.Nominal
            ActiveSheet.Cells(i, 3) = cmd.DimensionCommand.Plus
            ActiveSheet.Cells(i, 4) = cmd.DimensionCommand.Minus
            
                For r = 2 To 4
                    ActiveSheet.Cells(i, r).NumberFormat = "0.000"
                     Next r
            End If
            
            For r = 6 To 8
                
                ActiveSheet.Cells(i, (Offset + r)).NumberFormat = "0.000"
                Next r
                
            
               
            ActiveSheet.Cells(i, (Offset + 6)) = cmd.DimensionCommand.Measured
            ActiveSheet.Cells(i, (Offset + 7)) = cmd.DimensionCommand.Deviation
            ActiveSheet.Cells(i, (Offset + 8)) = cmd.DimensionCommand.OutTol
            ActiveSheet.Cells(i, (Offset + 8)).FormatConditions.Add Type:=xlCellValue, Operator:=xlNotEqual, _
            Formula1:="0"
            ActiveSheet.Cells(i, (Offset + 8)).FormatConditions(1).Interior.ColorIndex = 3
            ActiveSheet.Cells(i, (Offset + 8)).FormatConditions(1).Font.Bold = True
            
             
                
            End If
            
        ElseIf cmd.Type <> DIMENSION_TRUE_END_POSITION Or DIMENSION_END_LOCATION Then
            
            If Run < 39 Then
            
            ActiveSheet.Cells(i, 1).Font.Bold = True
            
            Select Case cmd.Type
             
             Case DIMENSION_X_LOCATION
                            ActiveSheet.Cells(i, 1) = "X"
             Case DIMENSION_R_LOCATION
                            ActiveSheet.Cells(i, 1) = "R"
             Case DIMENSION_Y_LOCATION
                             ActiveSheet.Cells(i, 1) = "Y"
             Case DIMENSION_Z_LOCATION
                            ActiveSheet.Cells(i, 1) = "Z"
             Case DIMENSION_D_LOCATION
                            ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_T_LOCATION
                            ActiveSheet.Cells(i, 1) = "T"
             Case DIMENSION_L_LOCATION
                          ActiveSheet.Cells(i, 1) = "L"
             Case DIMENSION_TRUE_DIAM_LOCATION
                           ActiveSheet.Cells(i, 1) = "TP"
             Case DIMENSION_TRUE_D1_LOCATION
                           ActiveSheet.Cells(i, 1) = "D1"
             Case DIMENSION_TRUE_D2_LOCATION
                           ActiveSheet.Cells(i, 1) = "D2"
             Case DIMENSION_TRUE_D3_LOCATION
                           ActiveSheet.Cells(i, 1) = "D3"
             Case DIMENSION_TRUE_DD_LOCATION
                           ActiveSheet.Cells(i, 1) = "DD"
             Case DIMENSION_TRUE_DF_LOCATION
                           ActiveSheet.Cells(i, 1) = "DF"
             Case DIMENSION_TRUE_END_POSITION
                           ActiveSheet.Cells(i, 1) = "END"
             Case DIMENSION_TRUE_LD_LOCATION
                           ActiveSheet.Cells(i, 1) = "LD"
             Case DIMENSION_TRUE_LF_LOCATION
                           ActiveSheet.Cells(i, 1) = "LF"
             Case DIMENSION_TRUE_PA_LOCATION
                           ActiveSheet.Cells(i, 1) = "PA"
             Case DIMENSION_TRUE_PR_LOCATION
                           ActiveSheet.Cells(i, 1) = "PR"
             Case DIMENSION_TRUE_START_POSITION
                           ActiveSheet.Cells(i, 1) = "START"
             Case DIMENSION_TRUE_WD_LOCATION
                           ActiveSheet.Cells(i, 1) = "WD"
             Case DIMENSION_TRUE_WF_LOCATION
                           ActiveSheet.Cells(i, 1) = "WF"
             Case DIMENSION_TRUE_X_LOCATION
                           ActiveSheet.Cells(i, 1) = "X"
             Case DIMENSION_TRUE_Y_LOCATION
                           ActiveSheet.Cells(i, 1) = "Y"
             Case DIMENSION_TRUE_Z_LOCATION
                           ActiveSheet.Cells(i, 1) = "Z"
             Case DIMENSION_V_LOCATION
                           ActiveSheet.Cells(i, 1) = "V"
             Case DIMENSION_FLATNESS
                           ActiveSheet.Cells(i, 1) = "F"
             Case DIMENSION_KEYIN
                           ActiveSheet.Cells(i, 1) = "K"
             Case DIMENSION_PA_LOCATION
                           ActiveSheet.Cells(i, 1) = "PA"
             Case DIMENSION_PR_LOCATION
                           ActiveSheet.Cells(i, 1) = "PR"
             Case DIMENSION_2D_DISTANCE
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_3D_ANGLE
                           ActiveSheet.Cells(i, 1) = "A"
             Case DIMENSION_3D_DISTANCE
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_A_LOCATION
                           ActiveSheet.Cells(i, 1) = "A"
             Case DIMENSION_ANGULARITY
                           ActiveSheet.Cells(i, 1) = "A"
             Case DIMENSION_COAXIALITY
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_CONCENTRICITY
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_D_LOCATION
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_PARALLELISM
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_PERPENDICULARITY
                           ActiveSheet.Cells(i, 1) = "D"
             Case DIMENSION_PROFILE
                           ActiveSheet.Cells(i, 1) = "P"
             Case DIMENSION_ROUNDNESS
                           ActiveSheet.Cells(i, 1) = "R"
             Case DIMENSION_RUNOUT
                           ActiveSheet.Cells(i, 1) = "R"
             Case DIMENSION_STRAIGHTNESS
                           ActiveSheet.Cells(i, 1) = "S"
             Case Else
                        ActiveSheet.Cells(i, 1) = "M"
                               
             End Select
             
            
                       
                
                End If
                
                If Run < 39 Then
            ActiveSheet.Cells(i, 2) = cmd.DimensionCommand.Nominal
            ActiveSheet.Cells(i, 3) = cmd.DimensionCommand.Plus
            ActiveSheet.Cells(i, 4) = cmd.DimensionCommand.Minus
            
            For r = 2 To 4
                    
                    ActiveSheet.Cells(i, r).NumberFormat = "0.000"
                Next r
                End If
                
            For r = 6 To 8
                    
                    ActiveSheet.Cells(i, (r + Offset)).NumberFormat = "0.000"
                Next r
                
                        
        
               
            ActiveSheet.Cells(i, (Offset + 6)) = cmd.DimensionCommand.Measured
            ActiveSheet.Cells(i, (Offset + 7)) = cmd.DimensionCommand.Deviation
            ActiveSheet.Cells(i, (Offset + 8)) = cmd.DimensionCommand.OutTol
                     
                         
            ActiveSheet.Cells(i, (Offset + 8)).FormatConditions.Delete
            ActiveSheet.Cells(i, (Offset + 8)).FormatConditions.Add Type:=xlCellValue, Operator:=xlNotEqual, _
            Formula1:="0"
            ActiveSheet.Cells(i, (Offset + 8)).FormatConditions(1).Interior.ColorIndex = 3
            ActiveSheet.Cells(i, (Offset + 8)).FormatConditions(1).Font.Bold = True
            
                
            
        End If
        i = i + 1
    End If
  Next cmd
  
   If Run < 40 Then
   Offset = Offset + 5
   ActiveSheet.Cells(2, 9) = Offset
   
    End If

End Sub


main
