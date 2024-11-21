#Requires AutoHotkey v2.0
#Include <Color>
#Include <ImagePut>

; Load the Gradient JSON data
jsonData := FileRead("gradient.json")

; Deserialize the Gradient from JSON
grad := Gradient.Deserialize(jsonData)

; Convert the Gradient to a 512x512 pixel HBITMAP and display it
ImageShow(grad.ToHBITMAP(512, 512))

; Show a preview of, and some information about, the gradient.
Showcase(grad, "Deserialized Gradient")