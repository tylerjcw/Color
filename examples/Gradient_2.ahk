#Requires AutoHotkey v2.0
#Include <Color>
#Include <ImagePut>

steps := 360 ; Gradient total steps
g_size  := 400  ; HBITMAP width and height

; Create a gradient
grad := Gradient(steps, Color(0xFFFF0000), Color.Yellow, Color(0xFF00FF00), Color.Aqua, Color(0xFF0000FF), Color.Fuchsia, Color.Red)
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

grad.Type := Gradient.Type.Linear
grad2 := Gradient(360,
                  ColorStop(0   , Color.White),
                  ColorStop(0.45, Color(0x01FFFFFF)),
                  ColorStop(0.50, Color.BlackTransparent),
                  ColorStop(0.55, Color(0x01000000)),
                  ColorStop(1   , Color.Black))

for stop in grad2.ColorStops
    FileAppend(A_Index ": " stop.Position "`n   " stop.Color.ToString("Hex") "`n`n", "*")

grad2.Angle := 90
c1 := Canvas.FromGradient(grad, g_size, g_size)
c2 := Canvas.FromGradient(grad2, g_size, g_size)
c1.OverlayCanvas(c2, 0, 0)
c1.SetAlpha(255)
ImageShow(c1.ToHBITMAP())

grad.Type := Gradient.Type.Conical
grad2.Type := Gradient.Type.Radial
c1 := Canvas.FromGradient(grad, g_size, g_size)
c2 := Canvas.FromGradient(grad2, g_size, g_size)
c1.OverlayCanvas(c2, 0, 0)
c1.SetAlpha(255)
ImageShow(c1.ToHBITMAP())
