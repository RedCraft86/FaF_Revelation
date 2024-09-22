import os
import json
import subprocess

EXE_Path = input("Enter exe Path:").replace("& ", "").replace("\'", "")
assert EXE_Path.endswith(".exe") and os.path.isfile(EXE_Path)

JSON_Path = os.path.dirname(os.path.realpath(__file__)) + "/EXE_Details.json"
if not os.path.isfile(JSON_Path):
    JSON_Path = input("Enter json Path:").replace("& ", "").replace("\'", "")
    assert JSON_Path.endswith(".json") and os.path.isfile(JSON_Path)

with open(JSON_Path) as JsonFile:
    JsonObj = json.load(JsonFile)
    for Key in JsonObj:
        CMD = ["rcedit", EXE_Path, "--set-version-string", Key, JsonObj[Key]]
        if Key == "FileVersion": CMD = ["rcedit", EXE_Path, "--set-file-version", JsonObj[Key]]
        subprocess.run(CMD)
        print(CMD)