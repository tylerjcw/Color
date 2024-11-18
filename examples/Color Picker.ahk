#Requires AutoHotkey v2.0
#Include <Color>

col := ColorPicker.RunDefault()
if (col)
    Showcase(col, "Picked Color")