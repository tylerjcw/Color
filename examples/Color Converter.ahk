#Requires AutoHotkey v2.0
#SingleInstance Force
#Include <Color>

/**
 * This example will allow you to pick a color or type a color in using either
 * the color function syntax (eg: "rgb(123, 61, 93)", "ncol(B20, 40%, 5%)", "hsl(120, 70%, 50%)", etc...)
 * or using Hex (RRGGBB, or AARRGGBB with "0x" or "#" preceding), or you can use
 * RGB or RGBA ("R, G, B" or "R, G, B, A"). If the input is valid, it will convert it to all supported
 * formats of the Color class. You can then click on any of the group boxes to copy the full color string
 * to the clipboard.
 */

MainGui := Gui()
MainGui.Title := "Color Converter"
MainGui.SetFont("s10")

MainGui.Add("Text", "x10 y10 w100", "Input Color:")
inputEdit := MainGui.Add("Edit", "x10 y30 w200 vInputColor")
inputEdit.SetFont("s10", "Consolas")
pickBtn    := MainGui.Add("Button", "x10 y+7 w200", "Pick Color")
convertBtn := MainGui.Add("Button", "x220 y10 w80 h80", "Convert")
colorPreview := MainGui.Add("Progress", "x310 y10 w500 h80 +Background000000")

labels := ["Hex", "RGB", "LinearSRGB", "ProPhotoRGB", "AdobeRGB", "HSL", "HSV", "HSI", "HWB", "CMYK", "NCol", "XYZ", "Lab", "LCHab", "YIQ", "Luv", "LCHuv", "YPbPr"]

labelControls := Map()
componentControls := Map()

gridWidth := 3
gridHeight := 6
boxWidth := 260
boxHeight := 120
marginX := 10
marginY := 10

for index, label in labels
{
    col := Mod(index - 1, gridWidth)
    row := (index - 1) // gridWidth
    x := 10 + col * (boxWidth + marginX)
    y := 100 + row * (boxHeight + marginY)

    qgroupBox := MainGui.Add("GroupBox", "x" x " y" y " w" boxWidth " h" boxHeight, label)
    labelControls[label] := MainGui.Add("Text", "x" (x+10) " y" (y+20) " w240 vResult" label)
    labelControls[label].SetFont("s10", "Consolas")
    componentControls[label] := Map()
    componentControls[label]["1"] := MainGui.Add("Text", "x" (x+10) " y" (y+40) " w180 c707070 vComponents" label "1")
    componentControls[label]["2"] := MainGui.Add("Text", "x" (x+10) " y" (y+60) " w180 c707070 vComponents" label "2")
    componentControls[label]["3"] := MainGui.Add("Text", "x" (x+10) " y" (y+80) " w180 c707070 vComponents" label "3")
    componentControls[label]["1"].SetFont("s10", "Consolas")
    componentControls[label]["2"].SetFont("s10", "Consolas")
    componentControls[label]["3"].SetFont("s10", "Consolas")

    if (label == "CMYK")
    {
        componentControls[label]["4"] := MainGui.Add("Text", "x" (x+10) " y" (y+100) " w180 c707070 vComponents" label "4")
        componentControls[label]["4"].SetFont("s10", "Consolas")
    }

    clickArea := MainGui.Add("Text", "x" x " y" y " w" boxWidth " h" boxHeight " +BackgroundTrans")
    clickArea.OnEvent("Click", CopyColorValue.Bind(label))
}

picker := ColorPicker()
picker.OnUpdate(PickerUpdate)
picker.OnExit(PickerExit)
PickerExit(Color.Black) ; Set up the initial color to display

MainGui.Show()

#HotIf picker.IsActive()

LButton::
Space::picker.Stop()
h::picker.ToggleHighlight()
m::picker.ToggleViewMode()
NumPadAdd::picker.ZoomIn()
NumpadSub::picker.ZoomOut()
NumpadDiv::picker.DecreaseCaptureSize()
NumpadMult::picker.IncreaseCaptureSize()
Left::picker.Move(-1, 0)
Up::picker.Move(0, -1)
Right::picker.Move(1, 0)
Down::picker.Move(0, 1)

#HotIf

convertBtn.OnEvent("Click", ConvertColor)
pickBtn.OnEvent("Click", (*) => picker.Start())
MainGui.OnEvent("Close", (*) => ExitApp())

PickerUpdate(_col)
{
    if _col is Color
    {
        colorPreview.Opt("+Background" Format("{:06X}", _col.ToInt(1)))
        picker.TextBGColor := _col
        picker.TextFGColor := _col.IsLight() ? Color.Black : Color.White
    }
}

PickerExit(_col)
{
    if _col is Color
    {
        global col := _col
        inputEdit.Value := col.Full
        ConvertColor(col)
    }
}

CopyColorValue(colorType, *)
{
    fullValue := labelControls[colorType].Text
    A_Clipboard := fullValue
    ToolTip("Copied: " fullValue)
    SetTimer(() => ToolTip(), -2000)
}

ConvertColor(*)
{
    input := StrLower(inputEdit.Value)

    try
    {
        ; build this RegEx to match all color formats except hex, and pull out their type and channels
        chT  := "(?<type>[a-z]+)\("                 ; Matches the origin color type "ncol", "rgb", "hsl", etc...
        ch1  := "(?<ch1>[RYGCBMrygcbm]??\d+(\.\d+)?)%?, ?" ; The first channel of the color
        ch2  := "(?<ch2>-?\d+(\.\d+)?)%?, ?"        ; The second channel of the color
        ch3  := "(?<ch3>-?\d+(\.\d+)?)%?(\)|, )?"   ; The third channel of the color
        ch4  := "(?<ch4>-?\d+(\.\d+)?)?%?\)"        ; The fourth channel of the color (if color supports it)
        funcNeedle := chT . ch1 . ch2 . ch3 . ch4

        ; build this RegEx to match Hex (with or without 0x or #), RGB (in R, G, B format), and RGBA (in R, G, B, A format)
        hex := "(?<hexSign>#|0x)?(?<hexVal>[0-9a-fA-F]{3,8})(?!,)" ; Hex value, including an optional preceding "0x" or "#"
        rCh := "(?<rgb>(?<r>\d{1,3}),\s*" ; Red
        gCh := "(?<g>\d{1,3}),\s*"        ; Green
        bCh := "(?<b>\d{1,3})"            ; Blue
        aCh := "(?:,\s*(?<a>\d{1,3}))?)"  ; Alpha
        rgbaNeedle := hex "|" rCh . gCh . bCh . aCh

        if (RegExMatch(input, funcNeedle, &match))
        {
            switch (match.type)
            {
                case "rgb":
                    col := Color(match.ch1, match.ch2, match.ch3)
                case "rgba":
                    col := Color(match.ch1, match.ch2, match.ch3, match.ch4)
                case "hsl":
                    col := Color.FromHSL(match.ch1, match.ch2, match.ch3)
                case "hsv":
                    col := Color.FromHSV(match.ch1, match.ch2, match.ch3)
                case "hwb":
                    col := Color.FromHWB(match.ch1, match.ch2, match.ch3)
                case "cmyk":
                    col := Color.FromCMYK(match.ch1, match.ch2, match.ch3, match.ch4)
                case "ncol":
                    col := Color.FromNCol(StrUpper(match.ch1), match.ch2, match.ch3)
                case "xyz":
                    col := Color.FromXYZ_D65(match.ch1, match.ch2, match.ch3)
                case "lab":
                    col := Color.FromLab(match.ch1, match.ch2, match.ch3)
                case "yiq":
                    col := Color.FromYIQ(match.ch1, match.ch2, match.ch3)
                case "srgb":
                    col := Color.FromLinearSRGB(match.ch1, match.ch2, match.ch3)
                case "adobe":
                    col := Color.FromAdobeRGB(match.ch1, match.ch2, match.ch3)
                case "prophoto":
                    col := Color.FromProPhotoRGB(match.ch1, match.ch2, match.ch3)
                case "lchuv":
                    col := Color.FromLCHuv(match.ch1, match.ch2, match.ch3)
                case "lchab":
                    col := Color.FromLCHab(match.ch1, match.ch2, match.ch3)
                case "luv":
                    col := Color.FromLuv(match.ch1, match.ch2, match.ch3)
                case "ypbpr":
                    col := Color.FromYPbPr(match.ch1, match.ch2, match.ch3)
                default:
                    throw Error("Error in color syntax (function).")
            }
        }
        else if RegExMatch(input, rgbaNeedle, &match)
        {
            if match.hexSign and match.hexVal
                col := Color(Integer("0x" match.hexVal))
            else if match.rgb and match.a
                col := Color(match.r, match.g, match.b, match.a)
            else if match.rgb
                col := Color(match.r, match.g, match.b, match.a != "" 255 ? match.a : 255)
            else
                throw Error("Error in color syntax (non-function).")
        }
        else
        {
            throw Error("Error in color syntax (unknown format).")
        }

        colorPreview.Opt("+Background" Format("{:06X}", col.ToInt(1)))

        hex := col.ToHex()
        labelControls["Hex"].Text := col.ToString("Hex")
        componentControls["Hex"]["1"].Text := "R: " hex.R
        componentControls["Hex"]["2"].Text := "G: " hex.G
        componentControls["Hex"]["3"].Text := "B: " hex.B

        labelControls["RGB"].Text := col.ToString("RGB")
        componentControls["RGB"]["1"].Text := "R: " col.R
        componentControls["RGB"]["2"].Text := "G: " col.G
        componentControls["RGB"]["3"].Text := "B: " col.B

        hsl := col.ToHSL()
        labelControls["HSL"].Text := col.ToString("HSL")
        componentControls["HSL"]["1"].Text := "H: " Round(hsl.H, 2)
        componentControls["HSL"]["2"].Text := "S: " Round(hsl.S, 2)
        componentControls["HSL"]["3"].Text := "L: " Round(hsl.L, 2)

        hsv := col.ToHSV()
        labelControls["HSV"].Text := col.ToString("HSV")
        componentControls["HSV"]["1"].Text := "H: " Round(hsv.H, 2)
        componentControls["HSV"]["2"].Text := "S: " Round(hsv.S, 2)
        componentControls["HSV"]["3"].Text := "V: " Round(hsv.V, 2)

        hsi := col.ToHSI()
        labelControls["HSI"].Text := col.ToString("HSI")
        componentControls["HSI"]["1"].Text := "H: " . Round(hsi.H, 2)
        componentControls["HSI"]["2"].Text := "S: " . Round(hsi.S, 2)
        componentControls["HSI"]["3"].Text := "I: " . Round(hsi.I, 2)

        hwb := col.ToHWB()
        labelControls["HWB"].Text := col.ToString("HWB")
        componentControls["HWB"]["1"].Text := "H: " Round(hwb.H, 2)
        componentControls["HWB"]["2"].Text := "W: " Round(hwb.W, 2)
        componentControls["HWB"]["3"].Text := "B: " Round(hwb.B, 2)

        cmyk := col.ToCMYK()
        labelControls["CMYK"].Text := col.ToString("CMYK")
        componentControls["CMYK"]["1"].Text := "C: " Round(cmyk.C, 2)
        componentControls["CMYK"]["2"].Text := "M: " Round(cmyk.M, 2)
        componentControls["CMYK"]["3"].Text := "Y: " Round(cmyk.Y, 2)
        componentControls["CMYK"]["4"].Text := "K: " Round(cmyk.K, 2)

        ncol := col.ToNCol()
        labelControls["NCol"].Text := col.ToString("NCol")
        componentControls["NCol"]["1"].Text := "H: " ncol.H
        componentControls["NCol"]["2"].Text := "W: " Round(ncol.W, 2)
        componentControls["NCol"]["3"].Text := "B: " Round(ncol.B, 2)

        xyz := col.ToXYZ_D65()
        labelControls["XYZ"].Text := col.ToString("XYZ")
        componentControls["XYZ"]["1"].Text := "X: " Round(xyz.X, 2)
        componentControls["XYZ"]["2"].Text := "Y: " Round(xyz.Y, 2)
        componentControls["XYZ"]["3"].Text := "Z: " Round(xyz.Z, 2)

        lab := col.ToLab()
        labelControls["Lab"].Text := col.ToString("Lab")
        componentControls["Lab"]["1"].Text := "L: " Round(lab.L, 2)
        componentControls["Lab"]["2"].Text := "a: " Round(lab.a, 2)
        componentControls["Lab"]["3"].Text := "b: " Round(lab.b, 2)

        yiq := col.ToYIQ()
        labelControls["YIQ"].Text := col.ToString("YIQ")
        componentControls["YIQ"]["1"].Text := "Y: " Round(yiq.Y, 4)
        componentControls["YIQ"]["2"].Text := "I: " Round(yiq.I, 4)
        componentControls["YIQ"]["3"].Text := "Q: " Round(yiq.Q, 4)

        srgb := col.ToLinearSRGB()
        labelControls["LinearSRGB"].Text := col.ToString("LinearSRGB")
        componentControls["LinearSRGB"]["1"].Text := "R: " . Round(srgb.R, 4)
        componentControls["LinearSRGB"]["2"].Text := "G: " . Round(srgb.G, 4)
        componentControls["LinearSRGB"]["3"].Text := "B: " . Round(srgb.B, 4)

        prophoto := col.ToProPhotoRGB()
        labelControls["ProPhotoRGB"].Text := col.ToString("ProPhotoRGB")
        componentControls["ProPhotoRGB"]["1"].Text := "R: " . Round(prophoto.R, 4)
        componentControls["ProPhotoRGB"]["2"].Text := "G: " . Round(prophoto.G, 4)
        componentControls["ProPhotoRGB"]["3"].Text := "B: " . Round(prophoto.B, 4)

        adobe := col.ToAdobeRGB()
        labelControls["AdobeRGB"].Text := col.ToString("AdobeRGB")
        componentControls["AdobeRGB"]["1"].Text := "R: " . Round(adobe.R, 4)
        componentControls["AdobeRGB"]["2"].Text := "G: " . Round(adobe.G, 4)
        componentControls["AdobeRGB"]["3"].Text := "B: " . Round(adobe.B, 4)

        lchuv := col.ToLCHuv()
        labelControls["LCHuv"].Text := col.ToString("LCHuv")
        componentControls["LCHuv"]["1"].Text := "L: " . Round(lchuv.L, 2)
        componentControls["LCHuv"]["2"].Text := "C: " . Round(lchuv.C, 2)
        componentControls["LCHuv"]["3"].Text := "H: " . Round(lchuv.H, 2)

        lchab := col.ToLCHab()
        labelControls["LCHab"].Text := col.ToString("LCHab")
        componentControls["LCHab"]["1"].Text := "L: " . Round(lchab.L, 2)
        componentControls["LCHab"]["2"].Text := "C: " . Round(lchab.C, 2)
        componentControls["LCHab"]["3"].Text := "H: " . Round(lchab.H, 2)

        luv := col.ToLUV()
        labelControls["Luv"].Text := col.ToString("Luv")
        componentControls["Luv"]["1"].Text := "L: " . Round(luv.L, 2)
        componentControls["Luv"]["2"].Text := "u: " . Round(luv.u, 2)
        componentControls["Luv"]["3"].Text := "v: " . Round(luv.v, 2)

        ypbpr := col.ToYPbPr()
        labelControls["YPbPr"].Text := col.ToString("YPbPr")
        componentControls["YPbPr"]["1"].Text := "Y: " . Round(ypbpr.Y, 4)
        componentControls["YPbPr"]["2"].Text := "Pb: " . Round(ypbpr.Cb, 4)
        componentControls["YPbPr"]["3"].Text := "Pr: " . Round(ypbpr.Cr, 4)
    }
    catch Error as err
    {
        MsgBox("Error converting color: " err.Message)
    }
}