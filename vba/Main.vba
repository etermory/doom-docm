Option Explicit

Private Declare PtrSafe Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As LongPtr

Private Declare PtrSafe Sub doomgeneric_Create Lib "doomgeneric_docm.dll" (ByVal dir As LongPtr)
Private Declare PtrSafe Sub doomgeneric_Tick Lib "doomgeneric_docm.dll" ()

Private Sub DrawFrame()
    Dim shp As InlineShape
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
    FileCopy outputFramePath, framePath
    ThisDocument.InlineShapes.AddPicture FileName:=framePath, LinkToFile:=True, SaveWithDocument:=False
End Sub

Sub Doom()
    Static hasRun As Boolean
    Dim dllPath As String
    Dim dirPath As String
    Dim hModule As LongPtr
    Dim result As Long
    Dim dirAnsi As String
    Dim dirPtr As LongPtr

    If hasRun Then
        Exit Sub
    End If
    hasRun = True

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
        DrawFrame
        DoEvents
    Loop
End Sub
