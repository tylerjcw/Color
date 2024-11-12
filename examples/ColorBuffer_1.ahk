#Requires AutoHotkey v2.0

#Include <Color>
#Include <ImagePut>

; Color matrices must be 5x5
; Example Color Matrix, this one will do nothing if applied
matrix := [
    [ 1.000,  0.000,  0.000,  0.000,  0.000], ; Red
    [ 0.000,  1.000,  0.000,  0.000,  0.000], ; Green
    [ 0.000,  0.000,  1.000,  0.000,  0.000], ; Blue
    [ 0.000,  0.000,  0.000,  1.000,  0.000], ; Alpha
    [ 0.000,  0.000,  0.000,  0.000,  1.000]  ; You can safely disregard this row
]   ; Red     Green   Blue    Alpha   Scale

; Inversion Matrix
invertMatrix := [
    [-1.000,  0.000,  0.000,  0.000,  1.000],
    [ 0.000, -1.000,  0.000,  0.000,  1.000],
    [ 0.000,  0.000, -1.000,  0.000,  1.000],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; "Heat Vision"
heatVisionMatrix := [
    [ 2.000, -1.000, -1.000,  0.000,  0.300],
    [-1.000,  1.500, -1.000,  0.000,  0.200],
    [ 1.000, -1.000,  2.000,  0.000,  0.100],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; "Night Vision"
nightVisionMatrix := [
    [ 0.100,  0.400,  0.000,  0.000,  0.100],
    [ 0.300,  1.500,  0.200,  0.000,  0.200],
    [ 0.000,  0.200,  0.100,  0.000,  0.000],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; Applies an effect reminiscent of Polaroid photographs
polaroidMatrix := [
    [ 1.438, -0.062, -0.062,  0.000,  0.000],
    [-0.122,  1.378, -0.122,  0.000,  0.000],
    [-0.016, -0.016,  1.483,  0.000,  0.000],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; Applies a technicolor effect
technicolorMatrix := [
    [ 1.600, -0.400, -0.200,  0.000,  0.000],
    [-0.200,  1.400, -0.200,  0.000,  0.000],
    [-0.200, -0.400,  1.600,  0.000,  0.000],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; Applies an effect similar to vintage film
vintageFilmMatrix := [
    [ 0.900,  0.100,  0.100,  0.000,  0.000],
    [ 0.100,  0.900,  0.100,  0.000,  0.000],
    [ 0.100,  0.200,  0.700,  0.000,  0.000],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; Applies a kodachrome-like effect
kodachromeMatrix := [
    [ 1.200, -0.100,  0.000,  0.000,  0.100],
    [-0.050,  1.100,  0.000,  0.000,  0.050],
    [ 0.000, -0.100,  1.000,  0.000,  0.000],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; Applies an effect that imitates cross-processing
crossProcessMatrix := [
    [ 1.000,  0.200,  0.100,  0.000,  0.000],
    [ 0.000,  1.100, -0.100,  0.000,  0.000],
    [ 0.100,  0.100,  1.000,  0.000,  0.000],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; Applies a blue-channel grayscale effect
blueChannelGrayscaleMatrix := [
    [ 0.000,  0.000,  1.000,  0.000,  0.000],
    [ 0.000,  0.000,  1.000,  0.000,  0.000],
    [ 0.000,  0.000,  1.000,  0.000,  0.000],
    [ 0.000,  0.000,  0.000,  1.000,  0.000],
    [ 0.000,  0.000,  0.000,  0.000,  1.000]
]

; Name of image to load
filename := "lizard.jpg"

; Load the image as an HBITMAP, using ImagePut
hBitmap := ImagePutHBitmap(filename)

; Create a ColorBuffer from the HBITMAP
cBuffer := ColorBuffer.FromHBITMAP(hBitmap)

; Apply a Matrix to the ColorBuffer
cBuffer.ApplyMatrix(invertMatrix)

; Display the original, then modified images
ImageShow(hBitmap)
ImageShow(cBuffer.ToHBITMAP())