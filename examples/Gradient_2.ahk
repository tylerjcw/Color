#Requires AutoHotkey v2.0
#Include <Color>
#Include <ImagePut>

steps := 360 ; Gradient total steps
g_size  := 400  ; HBITMAP width and height

; Create a gradient
grad := Gradient(steps, Color(0x78FF0000), Color.Yellow, Color(0xAA00FF00), Color.Aqua, Color(0xDD0000FF), Color.Fuchsia, Color(0x00000000))
grad.Amplitude := 0
grad.Wavelength := 0
grad.Repetitions := 1
grad.Vertices := 3
grad.EdgeSharpness := 0
grad.Angle := 0

; Linear Gradient Bitmap
grad.Type := Gradient.Type.Linear
p1 := grad.ToHBITMAP(g_size, g_size)
ImageShow(p1)

; Radial Gradient Bitmap
grad.Type := Gradient.Type.Radial
p2 := grad.ToHBITMAP(g_size, g_size)
ImageShow(p2)

; Conical Gradient Bitmap
grad.Type := Gradient.Type.Conical
p3 := grad.ToHBITMAP(g_size, g_size)
ImageShow(p3)