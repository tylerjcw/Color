#Requires AutoHotkey v2.0
#Include <Color>

class GradientGui extends Gui
{
    Width := 600
    Height := 600

    __New()
    {
        super.__New("+Resize")
        this.BackColor := "282C34"

        ; Add controls at the top
        this.AddGroupBox("x10 y5 w" this.Width - 20 " h170 Background" this.BackColor " cWhite", "Gradient Controls")

        ; Type selection
        this.AddText("x20 y25 w70 h20 cWhite", "Type:")
        this.GradType := this.AddDropDownList("x90 y25 w100 Choose1", ["Linear", "Radial", "Conical"])

        ; Angle control
        this.AddText("x20 y50 w70 h20 cWhite", "Angle:")
        this.Angle := this.AddEdit("x90 y50 w60")
        this.AngleUpDown := this.AddUpDown("Range-360-360", 0)

        ; Edge Sharpness
        this.AddText("x20 y75 w70 h20 cWhite", "Sharpness:")
        this.EdgeSharpness := this.AddEdit("x90 y75 w60")
        this.SharpnessUpDown := this.AddUpDown("Range-100-100", 0)

        ; Pattern Wavelength
        this.AddText("x20 y100 w70 h20 cWhite", "Wavelength:")
        this.Wavelength := this.AddEdit("x90 y100 w60")
        this.WavelengthUpDown := this.AddUpDown("Range0-20", 0)

        ; Pattern Amplitude
        this.AddText("x20 y125 w70 h20 cWhite", "Amplitude:")
        this.AmplitudeUpDown := this.AddEdit("x90 y125 w60")
        this.AmplitudeUpDown := this.AddUpDown("Range-100-100", 0)

        ; Pattern Vertices
        this.AddText("x20 y150 w70 h20 cWhite", "Vertices:")
        this.Vertices := this.AddEdit("x90 y150 w60")
        this.VerticesUpDown := this.AddUpDown("Range0-1000", 0)

        ; Focus controls
        this.AddText("x200 y25 w70 h20 cWhite", "Focus X/Y:")
        this.FocusX := this.AddEdit("x270 y25 w60")
        this.FocusXUpDown := this.AddUpDown("Range-100-100", 0)
        this.FocusY := this.AddEdit("x270 y50 w60")
        this.FocusYUpDown := this.AddUpDown("Range-100-100", 0)

        ; Add animation checkboxes
        this.AddText("x200 y75 w70 h20 cWhite", "Animate:")
        this.AnimateAngle := this.AddCheckbox("x270 y75 w100 cWhite", "Angle")
        this.AnimateWavelength := this.AddCheckbox("x270 y100 w100 cWhite", "Wavelength")
        this.AnimateAmplitude := this.AddCheckbox("x270 y125 w100 cWhite", "Amplitude")

        this.Show("w" this.Width " h" this.Height + 180)

        ; Create gradient with color stops
        this.grad := Gradient(360, Color.Red, Color.Yellow, Color.Lime, Color.Aqua, Color.Blue, Color.Fuchsia, Color.Red)

        ; Bind events
        this.GradType.OnEvent("Change", this.UpdateGradient.Bind(this))
        this.AngleUpDown.OnEvent("Change", this.UpdateGradient.Bind(this))
        this.SharpnessUpDown.OnEvent("Change", this.UpdateGradient.Bind(this))
        this.AmplitudeUpDown.OnEvent("Change", this.UpdateGradient.Bind(this))
        this.FocusXUpDown.OnEvent("Change", this.UpdateGradient.Bind(this))
        this.FocusYUpDown.OnEvent("Change", this.UpdateGradient.Bind(this))
        this.WavelengthUpDown.OnEvent("Change", this.UpdateGradient.Bind(this))
        this.VerticesUpDown.OnEvent("Change", this.UpdateGradient.Bind(this))

        ; Start animation
        this.angle := 0
        this.distortionPhase := 0
        this.freqPhase := 0
        SetTimer(this.Animate.Bind(this), 16)
    }

    UpdateGradient(*)
    {
        this.grad.Type := this.GradType.Value - 1
        this.grad.Angle := this.AngleUpDown.Value
        this.grad.EdgeSharpness := this.SharpnessUpDown.Value
        this.grad.Wavelength := this.WavelengthUpDown.Value
        this.grad.Focus := {x: this.FocusXUpDown.Value / 100, y: this.FocusYUpDown.Value / 100}
        this.grad.Vertices := this.VerticesUpDown.Value
        this.grad.Amplitude := this.AmplitudeUpDown.Value
        this.grad.Draw(this.Hwnd, 0, 180, this.Width, this.Height)
    }

    Animate()
    {
        if (this.AnimateAngle.Value)
        {
            this.angle := Mod(this.angle + 1, 360)
            this.grad.Angle := this.angle
        }

        if (this.AnimateWavelength.Value)
        {
            this.distortionPhase := Mod(this.distortionPhase + 0.05, 2 * 3.14159)
            this.grad.Wavelength := Sin(this.distortionPhase) * this.WavelengthUpDown.Value
        }

        if (this.AnimateAmplitude.Value)
        {
            this.freqPhase := Mod(this.freqPhase + 0.02, 2 * 3.14159)
            this.grad.Amplitude := this.AmplitudeUpDown.Value * (1 + Sin(this.freqPhase) * 0.5)
        }

        this.grad.ShiftHue(1)
        this.grad.Draw(this.Hwnd, 0, 180, this.Width, this.Height)
    }
}

myGui := GradientGui()
myGui.OnEvent("Close", (*) => ExitApp())