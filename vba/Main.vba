Option Explicit
Option Base 0

Private Declare PtrSafe Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As LongPtr

Private Declare PtrSafe Sub doomgeneric_Create Lib "doomgeneric_docm.dll" (ByVal dir As LongPtr)
Private Declare PtrSafe Sub doomgeneric_Tick Lib "doomgeneric_docm.dll" ()

Private Sub drawFrame()
    Dim shp As InlineShape
    Dim width As Single
    Dim img As InlineShape
    Dim dirPath As String
    Dim outputFramePath As String
    Dim framePath As String

    dirPath = Environ("TEMP")
    outputFramePath = dirPath & "\frame.bmp"
    framePath = dirPath & "\frame_word.bmp"

    ThisDocument.Content.Delete
    For Each shp In ThisDocument.InlineShapes
        shp.Delete
    Next shp
    If dir(framePath) <> "" Then
        Kill framePath
    End If
    FileCopy outputFramePath, framePath
    Set img = ThisDocument.InlineShapes.AddPicture( _
        FileName:=framePath, _
        LinkToFile:=True, SaveWithDocument:=False)
End Sub

Sub doom()
    Dim dllPath As String
    Dim dirPath As String
    Dim hModule As LongPtr
    Dim result As Long
    Dim dirAnsi As String
    Dim dirPtr As LongPtr

    CreateDoomGenericDocmDll
    CreateDoom1Wad

    dirPath = Environ("TEMP")
    dllPath = dirPath & "\doomgeneric_docm.dll"
    dirAnsi = StrConv(dirPath, vbFromUnicode)
    dirPtr = StrPtr(dirAnsi)

    hModule = LoadLibrary(dllPath)
    If hModule = 0 Then
        MsgBox "Failed to load DLL: " & dllPath, vbCritical
        Exit Sub
    End If

    doomgeneric_Create dirPtr

    Do While True
        doomgeneric_Tick
        drawFrame
        DoEvents
    Loop
End Sub
