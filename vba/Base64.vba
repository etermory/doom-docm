Option Explicit

Function DecodeBase64(base64Str As String) As Byte()
    Dim XML As Object
    Set XML = CreateObject("MSXML2.DOMDocument")
    Dim Node As Object
    Set Node = XML.createElement("base64")
    Node.DataType = "bin.base64"
    Node.Text = base64Str
    DecodeBase64 = Node.nodeTypedValue
End Function
