#Requires AutoHotkey v2.0
#Include <Color>

if (col := ColorPicker.RunDefault())
    Showcase(col, "Picked Color")