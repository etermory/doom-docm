import base64

chunk_size = 738
sub_size = 1024


def generate_sub(filename: str, name: str) -> str:
    with open(filename, "rb") as f:
        data = f.read()
        chunks = [
            base64.b64encode(data[i:i + chunk_size]).decode("utf-8")
            for i in range(0, len(data), chunk_size)
        ]
        subchunks = [
            chunks[i:i + sub_size] for i in range(0, len(chunks), sub_size)
        ]

    out = ""

    for i, sub in enumerate(subchunks):
        out += (f"Private Sub {name}{i}(fileNum as Integer)" + ''.join(
            (f'\n    Put #fileNum, , DecodeBase64("{chunk}")'
             for chunk in sub)) + "\nEnd Sub"
                "\n"
                "\n")

    out += (
        f"Sub {name}()"
        "\n    Dim fileNum As Integer"
        "\n    FileNum = FreeFile()"
        f'\n    Open Environ("TEMP") & "\{filename}" For Binary Access Write As #fileNum'
        + ''.join((f'\n    {name}{i} fileNum'
                   for i in range(len(subchunks)))) + "\n    Close #fileNum"
        "\nEnd Sub")

    return out


print(
    generate_sub("doomgeneric_docm.dll", "CreateDoomGenericDocmDll") + '\n'
    '\n' + generate_sub("doom1.wad", "CreateDoom1Wad"))
