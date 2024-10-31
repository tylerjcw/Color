#Requires AutoHotkey v2.0

class __ColorLib
{
    static hModule := 0

    static __New()
    {
        dll_path := StrReplace(A_LineFile, ".ahk", ".dll")
        if FileExist(dll_path) and not __ColorLib.hModule
        {
            __ColorLib.hModule := DllCall("LoadLibrary", "Str", dll_path, "Ptr")
            if not __ColorLib.hModule
                throw Error("Failed to load Color.dll, please verify it is in the same folder as Color.ahk")
        }
    }

    static __Delete()
    {
        if __ColorLib.hModule
            DllCall("FreeLibrary", "Ptr", __ColorLib.hModule)
        else
            throw Error("Color.dll not unloaded successfully")
    }
}

class Color
{
    /**
     * Creates a new Color object.
     * @param {...Number} args - Either a single 32-bit hexadecimal integer, or Numbers representing `R`, `G`, `B`, and optionally `A`.
     * @throws {Error} If invalid arguments are provided.
     */
    __New(args*)
    {
        switch args.Length
        {
            case 1:
                if args[1] is Number
                    this.Ptr := DllCall("Color\CreateColorFromInt", "UInt", args[1])
                else if (args[1] is String) and Color.HasOwnProp(args[1]) and (Color.%args[1]% is Color)
                    this.Ptr := DllCall("Color\CreateColorFromInt", "UInt", Color.%args[1]%.ToInt())
                else
                    throw Error("Color Constructor: Invalid Color arguments")
            case 3:
                this.Ptr := DllCall("Color\CreateColorFromRGBA", "UChar", args[1], "UChar", args[2], "UChar", args[3], "UChar", 255, "Ptr")
            case 4:
                this.Ptr := DllCall("Color\CreateColorFromRGBA", "UChar", args[1], "UChar", args[2], "UChar", args[3], "UChar", args[4], "Ptr")
            default:
                throw Error("Color Constructor: Invalid Color arguments")
        }
    }

    /**
     * Deletes the Color object and frees associated resources.
     */
    __Delete() => this.Ptr ? DllCall("Color\DeleteColor", "Ptr", this.Ptr) : true

    /**
     * Gets or sets the alpha component of the color.
     * @type {Number}
     */
    A
    {
        get => DllCall("Color\GetColorAlpha", "Ptr", this.Ptr, "Int")
        set => DllCall("Color\SetColorAlpha", "Ptr", this.Ptr, "Int", value)
    }

    /**
     * Gets or sets the red component of the color.
     * @type {Number}
     */
    R
    {
        get => DllCall("Color\GetColorRed", "Ptr", this.Ptr, "Int")
        set => DllCall("Color\SetColorRed", "Ptr", this.Ptr, "Int", value)
    }

    /**
     * Gets or sets the green component of the color.
     * @type {Number}
     */
    G
    {
        get => DllCall("Color\GetColorGreen", "Ptr", this.Ptr, "Int")
        set => DllCall("Color\SetColorGreen", "Ptr", this.Ptr, "Int", value)
    }

    /**
     * Gets or sets the blue component of the color.
     * @type {Number}
     */
    B
    {
        get => DllCall("Color\GetColorBlue", "Ptr", this.Ptr, "Int")
        set => DllCall("Color\SetColorBlue", "Ptr", this.Ptr, "Int", value)
    }

    /**
     * Gets or sets the format string used for color representation.
     *
     * Default is `rgba({R}, {G}, {B}, {A})`
     * @type {string}
     */
    FormatString
    {
        get => StrGet(DllCall("Color\GetColorFormatString", "Ptr", this.Ptr, "Ptr"), "UTF-8")
        set => DllCall("Color\SetColorFormatString", "Ptr", this.Ptr, "Str", value)
    }

    /**
     * Gets the Full RGB string representation of the color.
     * ___
     * Example: `rgba(255, 34, 96, 128)`
     * ___
     * The format is controlled by `this.FormatString`
     * @type {Number}
     */
    Full  => this.ToString("RGB", this.FormatString)

    /**
     * Converts the color to an integer value.
     * @param {String} format - The format to convert to. Defaults to `0` (`0xAARRGGBB`).
     * ___
     * Format values:
     *
     * `0` : `0xAARRGGBB` (default)
     *
     * `1` : `0xRRGGBB`
     *
     * `2` : `0xAABBGGRR`
     *
     * `3` : `0xBBGGRR`
     *
     * `4` : `0xRRGGBBAA`
     *
     * `5` : `0xBBGGRRAA`
     */
    ToInt(format  := 0) => DllCall("Color\ColorToInt", "Ptr", this.Ptr, "Int", format, "UInt")

    /**
     * Converts the color to a string representation.
     * @param {string} type - The color space type (e.g., "RGB", "Hex", "HSL", "HWB", "XYZ", "Lab", "YIQ", "NCol", "CMYK", "HSV", "YPbPr", "LCHab", "LCHuv", "AdobeRGB", "ProPhotoRGB", "Luv").
     * @param {string} [format] - The format string to use. If not provided, uses the default format for the specified type.
     * @returns {string}
     */
    ToString(type, format := this.FormatString)
    {
        if format == ""
        {
            switch type
            {
                case "RGB":         format := Color.DefaultFormatString.RGB
                case "LinearSRGB":  format := Color.DefaultFormatString.LinearSRGB
                case "HEX":         format := Color.DefaultFormatString.Hex
                case "HSL":         format := Color.DefaultFormatString.HSL
                case "HWB":         format := Color.DefaultFormatString.HWB
                case "XYZ":         format := Color.DefaultFormatString.XYZ
                case "Lab":         format := Color.DefaultFormatString.Lab
                case "YIQ":         format := Color.DefaultFormatString.YIQ
                case "NCol":        format := Color.DefaultFormatString.NCol
                case "CMYK":        format := Color.DefaultFormatString.CMYK
                case "HSV":         format := Color.DefaultFormatString.HSV
                case "YPbPr":       format := Color.DefaultFormatString.YPbPr
                case "LCHab":       format := Color.DefaultFormatString.LCHab
                case "LCHuv":       format := Color.DefaultFormatString.LCHuv
                case "AdobeRGB":    format := Color.DefaultFormatString.AdobeRGB
                case "ProPhotoRGB": format := Color.DefaultFormatString.ProPhotoRGB
                case "Luv":         format := Color.DefaultFormatString.Luv
            }
        }

        fullStr := Buffer(256)
        typeBuf := Buffer(StrPut(type, "UTF-8"))
        StrPut(type, typeBuf, "UTF-8")
        formatBuf := Buffer(StrPut(format, "UTF-8"))
        StrPut(format, formatBuf, "UTF-8")
        DllCall("Color\ColorToString", "Ptr", this.Ptr, "Ptr", typeBuf, "Ptr", formatBuf, "Ptr", fullStr, "Cdecl")
        return StrGet(fullStr, "UTF-8")
    }

    /**
     * Compares this color with another color for equality.
     * @param {Color} other - The color to compare with.
     * @returns {boolean}
     */
    IsEqual(other) => this.ToInt() == other.ToInt()

    /**
     * Creates a random color.
     * @returns {Color}
     */
    static Random(alphaRand := true) => Color.FromPtr(DllCall("Color\CreateRandomColor", "Int", alphaRand, "Ptr"))

    static AliceBlue => Color.FromPtr(DllCall("Color\CreateAliceBlueColor", "Ptr"))
    static AntiqueWhite => Color.FromPtr(DllCall("Color\CreateAntiqueWhiteColor", "Ptr"))
    static Aqua => Color.FromPtr(DllCall("Color\CreateAquaColor", "Ptr"))
    static Aquamarine => Color.FromPtr(DllCall("Color\CreateAquamarineColor", "Ptr"))
    static Azure => Color.FromPtr(DllCall("Color\CreateAzureColor", "Ptr"))
    static Beige => Color.FromPtr(DllCall("Color\CreateBeigeColor", "Ptr"))
    static Bisque => Color.FromPtr(DllCall("Color\CreateBisqueColor", "Ptr"))
    static Black => Color.FromPtr(DllCall("Color\CreateBlackColor", "Ptr"))
    static BlackTransparent => Color.FromPtr(DllCall("Color\CreateBlackTransparentColor", "Ptr"))
    static BlanchedAlmond => Color.FromPtr(DllCall("Color\CreateBlanchedAlmondColor", "Ptr"))
    static Blue => Color.FromPtr(DllCall("Color\CreateBlueColor", "Ptr"))
    static BlueViolet => Color.FromPtr(DllCall("Color\CreateBlueVioletColor", "Ptr"))
    static Brown => Color.FromPtr(DllCall("Color\CreateBrownColor", "Ptr"))
    static BurlyWood => Color.FromPtr(DllCall("Color\CreateBurlyWoodColor", "Ptr"))
    static CadetBlue => Color.FromPtr(DllCall("Color\CreateCadetBlueColor", "Ptr"))
    static Chartreuse => Color.FromPtr(DllCall("Color\CreateChartreuseColor", "Ptr"))
    static Chocolate => Color.FromPtr(DllCall("Color\CreateChocolateColor", "Ptr"))
    static Coral => Color.FromPtr(DllCall("Color\CreateCoralColor", "Ptr"))
    static CornflowerBlue => Color.FromPtr(DllCall("Color\CreateCornflowerBlueColor", "Ptr"))
    static Cornsilk => Color.FromPtr(DllCall("Color\CreateCornsilkColor", "Ptr"))
    static Crimson => Color.FromPtr(DllCall("Color\CreateCrimsonColor", "Ptr"))
    static Cyan => Color.FromPtr(DllCall("Color\CreateCyanColor", "Ptr"))
    static DarkBlue => Color.FromPtr(DllCall("Color\CreateDarkBlueColor", "Ptr"))
    static DarkCyan => Color.FromPtr(DllCall("Color\CreateDarkCyanColor", "Ptr"))
    static DarkGoldenRod => Color.FromPtr(DllCall("Color\CreateDarkGoldenRodColor", "Ptr"))
    static DarkGray => Color.FromPtr(DllCall("Color\CreateDarkGrayColor", "Ptr"))
    static DarkGreen => Color.FromPtr(DllCall("Color\CreateDarkGreenColor", "Ptr"))
    static DarkKhaki => Color.FromPtr(DllCall("Color\CreateDarkKhakiColor", "Ptr"))
    static DarkMagenta => Color.FromPtr(DllCall("Color\CreateDarkMagentaColor", "Ptr"))
    static DarkOliveGreen => Color.FromPtr(DllCall("Color\CreateDarkOliveGreenColor", "Ptr"))
    static DarkOrange => Color.FromPtr(DllCall("Color\CreateDarkOrangeColor", "Ptr"))
    static DarkOrchid => Color.FromPtr(DllCall("Color\CreateDarkOrchidColor", "Ptr"))
    static DarkRed => Color.FromPtr(DllCall("Color\CreateDarkRedColor", "Ptr"))
    static DarkSalmon => Color.FromPtr(DllCall("Color\CreateDarkSalmonColor", "Ptr"))
    static DarkSeaGreen => Color.FromPtr(DllCall("Color\CreateDarkSeaGreenColor", "Ptr"))
    static DarkSlateBlue => Color.FromPtr(DllCall("Color\CreateDarkSlateBlueColor", "Ptr"))
    static DarkSlateGray => Color.FromPtr(DllCall("Color\CreateDarkSlateGrayColor", "Ptr"))
    static DarkTurquoise => Color.FromPtr(DllCall("Color\CreateDarkTurquoiseColor", "Ptr"))
    static DarkViolet => Color.FromPtr(DllCall("Color\CreateDarkVioletColor", "Ptr"))
    static DeepPink => Color.FromPtr(DllCall("Color\CreateDeepPinkColor", "Ptr"))
    static DeepSkyBlue => Color.FromPtr(DllCall("Color\CreateDeepSkyBlueColor", "Ptr"))
    static DimGray => Color.FromPtr(DllCall("Color\CreateDimGrayColor", "Ptr"))
    static DodgerBlue => Color.FromPtr(DllCall("Color\CreateDodgerBlueColor", "Ptr"))
    static FireBrick => Color.FromPtr(DllCall("Color\CreateFireBrickColor", "Ptr"))
    static FloralWhite => Color.FromPtr(DllCall("Color\CreateFloralWhiteColor", "Ptr"))
    static ForestGreen => Color.FromPtr(DllCall("Color\CreateForestGreenColor", "Ptr"))
    static Fuchsia => Color.FromPtr(DllCall("Color\CreateFuchsiaColor", "Ptr"))
    static Gainsboro => Color.FromPtr(DllCall("Color\CreateGainsboroColor", "Ptr"))
    static GhostWhite => Color.FromPtr(DllCall("Color\CreateGhostWhiteColor", "Ptr"))
    static Gold => Color.FromPtr(DllCall("Color\CreateGoldColor", "Ptr"))
    static GoldenRod => Color.FromPtr(DllCall("Color\CreateGoldenRodColor", "Ptr"))
    static Gray => Color.FromPtr(DllCall("Color\CreateGrayColor", "Ptr"))
    static Green => Color.FromPtr(DllCall("Color\CreateGreenColor", "Ptr"))
    static GreenYellow => Color.FromPtr(DllCall("Color\CreateGreenYellowColor", "Ptr"))
    static Honeydew => Color.FromPtr(DllCall("Color\CreateHoneydewColor", "Ptr"))
    static HotPink => Color.FromPtr(DllCall("Color\CreateHotPinkColor", "Ptr"))
    static IndianRed => Color.FromPtr(DllCall("Color\CreateIndianRedColor", "Ptr"))
    static Indigo => Color.FromPtr(DllCall("Color\CreateIndigoColor", "Ptr"))
    static Ivory => Color.FromPtr(DllCall("Color\CreateIvoryColor", "Ptr"))
    static Khaki => Color.FromPtr(DllCall("Color\CreateKhakiColor", "Ptr"))
    static Lavender => Color.FromPtr(DllCall("Color\CreateLavenderColor", "Ptr"))
    static LavenderBlush => Color.FromPtr(DllCall("Color\CreateLavenderBlushColor", "Ptr"))
    static LawnGreen => Color.FromPtr(DllCall("Color\CreateLawnGreenColor", "Ptr"))
    static LemonChiffon => Color.FromPtr(DllCall("Color\CreateLemonChiffonColor", "Ptr"))
    static LightBlue => Color.FromPtr(DllCall("Color\CreateLightBlueColor", "Ptr"))
    static LightCoral => Color.FromPtr(DllCall("Color\CreateLightCoralColor", "Ptr"))
    static LightCyan => Color.FromPtr(DllCall("Color\CreateLightCyanColor", "Ptr"))
    static LightGoldenrodYellow => Color.FromPtr(DllCall("Color\CreateLightGoldenrodYellowColor", "Ptr"))
    static LightGray => Color.FromPtr(DllCall("Color\CreateLightGrayColor", "Ptr"))
    static LightGreen => Color.FromPtr(DllCall("Color\CreateLightGreenColor", "Ptr"))
    static LightGrey => Color.FromPtr(DllCall("Color\CreateLightGreyColor", "Ptr"))
    static LightPink => Color.FromPtr(DllCall("Color\CreateLightPinkColor", "Ptr"))
    static LightSalmon => Color.FromPtr(DllCall("Color\CreateLightSalmonColor", "Ptr"))
    static LightSeaGreen => Color.FromPtr(DllCall("Color\CreateLightSeaGreenColor", "Ptr"))
    static LightSkyBlue => Color.FromPtr(DllCall("Color\CreateLightSkyBlueColor", "Ptr"))
    static LightSlateGray => Color.FromPtr(DllCall("Color\CreateLightSlateGrayColor", "Ptr"))
    static LightSteelBlue => Color.FromPtr(DllCall("Color\CreateLightSteelBlueColor", "Ptr"))
    static LightYellow => Color.FromPtr(DllCall("Color\CreateLightYellowColor", "Ptr"))
    static Lime => Color.FromPtr(DllCall("Color\CreateLimeColor", "Ptr"))
    static LimeGreen => Color.FromPtr(DllCall("Color\CreateLimeGreenColor", "Ptr"))
    static Linen => Color.FromPtr(DllCall("Color\CreateLinenColor", "Ptr"))
    static Magenta => Color.FromPtr(DllCall("Color\CreateMagentaColor", "Ptr"))
    static Maroon => Color.FromPtr(DllCall("Color\CreateMaroonColor", "Ptr"))
    static MediumAquamarine => Color.FromPtr(DllCall("Color\CreateMediumAquamarineColor", "Ptr"))
    static MediumBlue => Color.FromPtr(DllCall("Color\CreateMediumBlueColor", "Ptr"))
    static MediumOrchid => Color.FromPtr(DllCall("Color\CreateMediumOrchidColor", "Ptr"))
    static MediumPurple => Color.FromPtr(DllCall("Color\CreateMediumPurpleColor", "Ptr"))
    static MediumSeaGreen => Color.FromPtr(DllCall("Color\CreateMediumSeaGreenColor", "Ptr"))
    static MediumSlateBlue => Color.FromPtr(DllCall("Color\CreateMediumSlateBlueColor", "Ptr"))
    static MediumSpringGreen => Color.FromPtr(DllCall("Color\CreateMediumSpringGreenColor", "Ptr"))
    static MediumTurquoise => Color.FromPtr(DllCall("Color\CreateMediumTurquoiseColor", "Ptr"))
    static MediumVioletRed => Color.FromPtr(DllCall("Color\CreateMediumVioletRedColor", "Ptr"))
    static MidnightBlue => Color.FromPtr(DllCall("Color\CreateMidnightBlueColor", "Ptr"))
    static MintCream => Color.FromPtr(DllCall("Color\CreateMintCreamColor", "Ptr"))
    static MistyRose => Color.FromPtr(DllCall("Color\CreateMistyRoseColor", "Ptr"))
    static Moccasin => Color.FromPtr(DllCall("Color\CreateMoccasinColor", "Ptr"))
    static NavajoWhite => Color.FromPtr(DllCall("Color\CreateNavajoWhiteColor", "Ptr"))
    static Navy => Color.FromPtr(DllCall("Color\CreateNavyColor", "Ptr"))
    static OldLace => Color.FromPtr(DllCall("Color\CreateOldLaceColor", "Ptr"))
    static Olive => Color.FromPtr(DllCall("Color\CreateOliveColor", "Ptr"))
    static OliveDrab => Color.FromPtr(DllCall("Color\CreateOliveDrabColor", "Ptr"))
    static Orange => Color.FromPtr(DllCall("Color\CreateOrangeColor", "Ptr"))
    static OrangeRed => Color.FromPtr(DllCall("Color\CreateOrangeRedColor", "Ptr"))
    static Orchid => Color.FromPtr(DllCall("Color\CreateOrchidColor", "Ptr"))
    static PaleGoldenrod => Color.FromPtr(DllCall("Color\CreatePaleGoldenrodColor", "Ptr"))
    static PaleGreen => Color.FromPtr(DllCall("Color\CreatePaleGreenColor", "Ptr"))
    static PaleTurquoise => Color.FromPtr(DllCall("Color\CreatePaleTurquoiseColor", "Ptr"))
    static PaleVioletRed => Color.FromPtr(DllCall("Color\CreatePaleVioletRedColor", "Ptr"))
    static PapayaWhip => Color.FromPtr(DllCall("Color\CreatePapayaWhipColor", "Ptr"))
    static PeachPuff => Color.FromPtr(DllCall("Color\CreatePeachPuffColor", "Ptr"))
    static Peru => Color.FromPtr(DllCall("Color\CreatePeruColor", "Ptr"))
    static Pink => Color.FromPtr(DllCall("Color\CreatePinkColor", "Ptr"))
    static Plum => Color.FromPtr(DllCall("Color\CreatePlumColor", "Ptr"))
    static PowderBlue => Color.FromPtr(DllCall("Color\CreatePowderBlueColor", "Ptr"))
    static Purple => Color.FromPtr(DllCall("Color\CreatePurpleColor", "Ptr"))
    static RebeccaPurple => Color.FromPtr(DllCall("Color\CreateRebeccaPurpleColor", "Ptr"))
    static Red => Color.FromPtr(DllCall("Color\CreateRedColor", "Ptr"))
    static RosyBrown => Color.FromPtr(DllCall("Color\CreateRosyBrownColor", "Ptr"))
    static RoyalBlue => Color.FromPtr(DllCall("Color\CreateRoyalBlueColor", "Ptr"))
    static SaddleBrown => Color.FromPtr(DllCall("Color\CreateSaddleBrownColor", "Ptr"))
    static Salmon => Color.FromPtr(DllCall("Color\CreateSalmonColor", "Ptr"))
    static SandyBrown => Color.FromPtr(DllCall("Color\CreateSandyBrownColor", "Ptr"))
    static SeaGreen => Color.FromPtr(DllCall("Color\CreateSeaGreenColor", "Ptr"))
    static Seashell => Color.FromPtr(DllCall("Color\CreateSeashellColor", "Ptr"))
    static Sienna => Color.FromPtr(DllCall("Color\CreateSiennaColor", "Ptr"))
    static Silver => Color.FromPtr(DllCall("Color\CreateSilverColor", "Ptr"))
    static SkyBlue => Color.FromPtr(DllCall("Color\CreateSkyBlueColor", "Ptr"))
    static SlateBlue => Color.FromPtr(DllCall("Color\CreateSlateBlueColor", "Ptr"))
    static SlateGray => Color.FromPtr(DllCall("Color\CreateSlateGrayColor", "Ptr"))
    static Snow => Color.FromPtr(DllCall("Color\CreateSnowColor", "Ptr"))
    static SpringGreen => Color.FromPtr(DllCall("Color\CreateSpringGreenColor", "Ptr"))
    static SteelBlue => Color.FromPtr(DllCall("Color\CreateSteelBlueColor", "Ptr"))
    static Tan => Color.FromPtr(DllCall("Color\CreateTanColor", "Ptr"))
    static Teal => Color.FromPtr(DllCall("Color\CreateTealColor", "Ptr"))
    static Thistle => Color.FromPtr(DllCall("Color\CreateThistleColor", "Ptr"))
    static Tomato => Color.FromPtr(DllCall("Color\CreateTomatoColor", "Ptr"))
    static Transparent => Color.FromPtr(DllCall("Color\CreateTransparentColor", "Ptr"))
    static Turquoise => Color.FromPtr(DllCall("Color\CreateTurquoiseColor", "Ptr"))
    static Violet => Color.FromPtr(DllCall("Color\CreateVioletColor", "Ptr"))
    static Wheat => Color.FromPtr(DllCall("Color\CreateWheatColor", "Ptr"))
    static White => Color.FromPtr(DllCall("Color\CreateWhiteColor", "Ptr"))
    static WhiteSmoke => Color.FromPtr(DllCall("Color\CreateWhiteSmokeColor", "Ptr"))
    static WhiteTransparent => Color.FromPtr(DllCall("Color\CreateWhiteTransparentColor", "Ptr"))
    static Yellow => Color.FromPtr(DllCall("Color\CreateYellowColor", "Ptr"))
    static YellowGreen => Color.FromPtr(DllCall("Color\CreateYellowGreenColor", "Ptr"))

    /**
     * Creates a Color object from a pointer.
     * @param {Ptr} Ptr - The pointer to the color data.
     * @returns {Color}
     */
    static FromPtr(Ptr) => {base: Color.Prototype, Ptr: Ptr}

    /**
     * Creates a Color object from Adobe RGB values.
     * @param {number} r - The red value (0-1).
     * @param {number} g - The green value (0-1).
     * @param {number} b - The blue value (0-1).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromAdobeRGB(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromAdobeRGB", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from HSL values.
     * @param {number} h - The hue value (0-360).
     * @param {number} s - The saturation value (0-100).
     * @param {number} l - The lightness value (0-100).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromHSL(h, s, l, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHSL" , "Double", h, "Double", s, "Double", l, "Double", a, "Ptr"))

    /**
     * Creates a Color object from HSV values.
     * @param {number} h - The hue value (0-360).
     * @param {number} s - The saturation value (0-100).
     * @param {number} v - The value (0-100).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromHSV(h, s, v, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHSV", "Double", h, "Double", s / 100, "Double", v / 100, "Double", a, "Ptr"))

    /**
     * Creates a Color object from HSV values.
     * @param {number} h - The hue value (0-360).
     * @param {number} s - The saturation value (0-100).
     * @param {number} i - The intensity (0-100).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromHSI(h, s, i, a := 255) => Color(DllCall("Color\ColorFromHSI", "Double", h, "Double", s, "Double", i, "Int", a, "Ptr", "Cdecl"))

    /**
     * Creates a Color object from HWB values.
     * @param {number} h - The hue value (0-360).
     * @param {number} w - The whiteness value (0-100).
     * @param {number} b - The blackness value (0-100).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromHWB(h, w, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHWB" , "Double", h, "Double", w, "Double", b, "Double", a, "Ptr"))

    /**
     * Creates a Color object from XYZ D50 values.
     * @param {number} x - The X value.
     * @param {number} y - The Y value.
     * @param {number} z - The Z value.
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromXYZ_D50(x, y, z, a := 255) => Color.FromPtr(DllCall("Color\ColorFromXYZ_D50" , "Double", x, "Double", y, "Double", z, "Double", a, "Ptr"))

    /**
     * Creates a Color object from XYZ D65 values.
     * @param {number} x - The X value.
     * @param {number} y - The Y value.
     * @param {number} z - The Z value.
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromXYZ_D65(x, y, z, a := 255) => Color.FromPtr(DllCall("Color\ColorFromXYZ_D65" , "Double", x, "Double", y, "Double", z, "Double", a, "Ptr"))

    /**
     * Creates a Color object from CIE Lab values.
     * @param {number} l - The L value.
     * @param {number} a - The a value.
     * @param {number} b - The b value.
     * @param {number} [t=255] - The alpha value (0-255).
     * @returns {Color} A new Color object.
     */
    static FromLab(l, a, b, t := 255) => Color.FromPtr(DllCall("Color\ColorFromLab" , "Double", l, "Double", a, "Double", b, "Double", t, "Ptr"))

    /**
     * Creates a Color object from LCHuv values.
     * @param {number} L - The lightness value.
     * @param {number} a - The a value.
     * @param {number} b - The b value.
     * @param {number} [alpha=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromLCHuv(l, c, h, a := 255) => Color.FromPtr(DllCall("Color\ColorFromLCHuv", "Double", l, "Double", c, "Double", h, "Int", a, "Ptr"))

    /**
     * Creates a Color object from CIE Luv values.
     * @param {number} L - The L value.
     * @param {number} u - The u value.
     * @param {number} v - The v value.
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromLuv(L, u, v, a := 255) => Color.FromPtr(DllCall("Color\ColorFromLuv", "Double", L, "Double", u, "Double", v, "Int", a, "Ptr"))

    /**
     * Creates a Color object from LCH values.
     * @param {number} l - The Lightness value (0-100).
     * @param {number} c - The Chroma value (0-100).
     * @param {number} h - The Hue value (0-360).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromLCHab(l, c, h, a := 255) => Color.FromPtr(DllCall("Color\ColorFromLCHab", "Double", l, "Double", c, "Double", h, "Double", a, "Ptr"))

    /**
     * Creates a Color object from YPbPr values.
     * @param {number} y - The Y value (0-1).
     * @param {number} cb - The Cb value (0-1).
     * @param {number} cr - The Cr value (0-1).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromYPbPr(y, cb, cr, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYPbPr", "Double", y, "Double", cb, "Double", cr, "Double", a, "Ptr"))

    /**
     * Creates a Color object from YIQ values.
     * @param {number} y - The Y value.
     * @param {number} i - The I value.
     * @param {number} q - The Q value.
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromYIQ(y, i, q, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYIQ" , "Double", y, "Double", i, "Double", q, "Double", a, "Ptr"))

    /**
     * Creates a Color object from CMYK values.
     * @param {number} c - The cyan value (0-100).
     * @param {number} m - The magenta value (0-100).
     * @param {number} y - The yellow value (0-100).
     * @param {number} k - The key (black) value (0-100).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromCMYK(c, m, y, k, a := 255) => Color.FromPtr(DllCall("Color\ColorFromCMYK", "Double", c, "Double", m, "Double", y, "Double", k, "Double", a, "Ptr"))

    /**
     * Creates a Color object from a color temperature in Kelvin.
     * @param {number} kelvin - The color temperature in Kelvin.
     * @returns {Color}
     */
    static FromTemp(kelvin) => Color.FromPtr(DllCall("Color\ColorFromTemp", "Double", kelvin, "Ptr"))

    /**
     * Creates a Color object from Linear sRGB values.
     * @param {number} r - The red value (0-1).
     * @param {number} g - The green value (0-1).
     * @param {number} b - The blue value (0-1).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromLinearSRGB(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromLinearSRGB", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from ProPhoto RGB values.
     * @param {number} r - The red value (0-1).
     * @param {number} g - The green value (0-1).
     * @param {number} b - The blue value (0-1).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromProPhotoRGB(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromProPhotoRGB", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from NCol values.
     * @param {string} h - The hue string (e.g., "R", "Y", "G", "C", "B", "M").
     * @param {number} w - The whiteness value (0-100).
     * @param {number} b - The blackness value (0-100).
     * @param {number} [a=255] - The alpha value (0-255).
     * @returns {Color}
     */
    static FromNCol(h, w, b, a := 255)
    {
        hBuf := Buffer(StrPut(h, "UTF-8"))
        StrPut(h, hBuf, "UTF-8")
        return Color.FromPtr(DllCall("Color\ColorFromNCol", "Ptr", hBuf, "Double", w, "Double", b, "Int", a, "Ptr"))
    }

    /**
     * Mixes two colors with the specified weight.
     * @returns {Color}
     */
    static Mix(color1, color2, weight := 0.5) => Color.FromPtr(DllCall("Color\ColorMix", "Ptr", color1.Ptr, "Ptr", color2.Ptr, "Double", weight, "Ptr"))

    /**
     * Screens two colors.
     * @returns {Color}
     */
    static Screen(color1, color2) => Color.FromPtr(DllCall("Color\ColorScreen", "Ptr", color1.Ptr, "Ptr", color2.Ptr, "Ptr"))

    /**
     * Multiplies two colors.
     * @returns {Color}
     */
    static Multiply(color1, color2) => Color.FromPtr(DllCall("Color\ColorMultiply", "Ptr", color1.Ptr, "Ptr", color2.Ptr, "Ptr"))

    /**
     * Overlays two colors.
     * @returns {Color}
     */
    static Overlay(color1, color2) => Color.FromPtr(DllCall("Color\ColorOverlay", "Ptr", color1.Ptr, "Ptr", color2.Ptr, "Ptr"))

    /**
     * Averages many colors.
     * @returns {Color}
     */
    static Average(colors*)
    {
        colorPtrs := Buffer(A_PtrSize * colors.Length)
        for i, col in colors
            NumPut("Ptr", col.Ptr, colorPtrs, (i - 1) * A_PtrSize)
        return Color.FromPtr(DllCall("Color\ColorAverage", "Ptr", colorPtrs, "Int", colors.Length, "Ptr"))
    }

    /**
     * Inverts the color.
     * @returns {Color}
     */
    Invert() => (DllCall("Color\InvertColor", "Ptr", this.Ptr), this)

    /**
     * Converts the color to grayscale.
     * @returns {Color}
     */
    Grayscale() => (DllCall("Color\GrayscaleColor", "Ptr", this.Ptr), this)

    /**
     * Applies a sepia tone to the color.
     * @returns {Color}
     */
    Sepia(factor) => (DllCall("Color\SepiaColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a cross process matrix to the color.
     * @returns {Color}
     */
    CrossProcess(factor) => (DllCall("Color\CrossProcessColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a moonlight matrix to the color.
     * @returns {Color}
     */
    Moonlight(factor) => (DllCall("Color\MoonlightColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a vintage film matrix to the color.
     * @returns {Color}
     */
    VintageFilm(factor) => (DllCall("Color\VintageFilmColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a technicolor matrix to the color.
     * @returns {Color}
     */
    Technicolor(factor) => (DllCall("Color\TechnicolorColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a Polaroid matrix to the color.
     * @returns {Color}
     */
    Polaroid(factor) => (DllCall("Color\PolaroidColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Shifts the hue of the color by the specified number of degrees.
     * @param {number} degrees - The number of degrees to shift the hue.
     * @returns {Color}
     */
    ShiftHue(degrees) => Color.FromPtr(DllCall("Color\ShiftHueColor", "Ptr", this.Ptr, "Double", degrees, "Ptr"))

    /**
     * Shifts the saturation of the color by the specified amount.
     * @param {number} amount - The amount to shift the saturation.
     * @returns {Color}
     */
    ShiftSaturation(amount) => Color.FromPtr(DllCall("Color\ShiftColorSaturation", "Ptr", this.Ptr, "Double", amount, "Ptr"))

    /**
     * Shifts the lightness of the color by the specified amount.
     * @param {number} amount - The amount to shift the lightness.
     * @returns {Color}
     */
    ShiftLightness(amount) => Color.FromPtr(DllCall("Color\ShiftLightnessColor", "Ptr", this.Ptr, "Double", amount, "Ptr"))

    /**
     * Shifts the whiteness of the color by the specified amount.
     * @param {number} amount - The amount to shift the whiteness.
     * @returns {Color}
     */
    ShiftWhiteness(amount) => Color.FromPtr(DllCall("Color\ShiftColorWhiteness", "Ptr", this.Ptr, "Double", amount, "Ptr"))

    /**
     * Shifts the blackness of the color by the specified amount.
     * @param {number} amount - The amount to shift the blackness.
     * @returns {Color}
     */
    ShiftBlackness(amount) => Color.FromPtr(DllCall("Color\ShiftColorBlackness", "Ptr", this.Ptr, "Double", amount, "Ptr"))

    /**
     * Calculates the contrast ratio between this color and another color.
     * @param {Color} other - The color to compare against.
     * @returns {number}
     */
    GetContrast(other) => DllCall("Color\ColorGetContrast", "Ptr", this.Ptr, "Ptr", other.Ptr, "Double")

    /**
     * Calculates the complementary color.
     * @returns {Color}
     */
    Complement() => Color.FromPtr(DllCall("Color\ColorComplement", "Ptr", this.Ptr, "Ptr"))

    /**
     * Determines if the color is accessible when used with the given background color.
     * @param {Color} background - The background color to check against.
     * @param {string} [level="AA"] - The accessibility level to check ("AA" or "AAA").
     * @returns {boolean} True if the color is accessible, false otherwise.
     */
    IsAccessible(background, level := "AA") => DllCall("Color\ColorIsAccessible", "Ptr", this.Ptr, "Ptr", background.Ptr, "Int", level == "AA" ? 0 : 1, "Int")

    /**
     * Calculates the luminance of the color.
     * @returns {number} The luminance value between 0 and 1.
     */
    GetLuminance() => DllCall("Color\GetColorLuminance", "Ptr", this.Ptr, "Double")

    /**
     * Determines if the color is considered light.
     * @returns {boolean} True if the color is light, false otherwise.
     */
    IsLight() => DllCall("Color\IsColorLight", "Ptr", this.Ptr, "Int")

    /**
     * Determines if the color is considered dark.
     * @returns {boolean} True if the color is dark, false otherwise.
     */
    IsDark() => DllCall("Color\IsColorDark", "Ptr", this.Ptr, "Int")

    /**
     * Normalizes the color values to the range [0, 1].
     * @returns {void}
     */
    Normalize() => (DllCall("Color\ColorNormalize", "Ptr", this.Ptr, "DoubleP", &r := 0, "DoubleP", &g := 0, "DoubleP", &b := 0), {R: r, G: g, B: b})

    /**
     * Converts the color to RGB format.
     * @returns {Object} An object containing R, G, B, and A values (0-255).
     */
    ToRGB() => (DllCall("Color\ColorToRGB", "Ptr", this.Ptr, "Ptr", r := Buffer(4, 0), "Ptr", g := Buffer(4, 0), "Ptr", b := Buffer(4, 0), "Ptr", a := Buffer(4, 0)), {R: NumGet(r, "Int"), G: NumGet(g, "Int"), B: NumGet(b, "Int"), A: NumGet(a, "Int")})

    /**
     * Converts the color to sRGB format.
     * @returns {Object} An object containing r, g, b (0-1), and A (0-255) values.
     */
    ToLinearSRGB() => (DllCall("Color\ColorToLinearSRGB", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {r: NumGet(r, "Double"), g: NumGet(g, "Double"), b: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to ProPhoto RGB format.
     * @returns {Object}
     */
    ToProPhotoRGB() => (DllCall("Color\ColorToProPhotoRGB", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {r: NumGet(r, "Double"), g: NumGet(g, "Double"), b: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to Adobe RGB format.
     * @returns {Object} An object containing r, g, b (0-1), and A (0-255) values.
     */
    ToAdobeRGB() => (DllCall("Color\ColorToAdobeRGB", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {r: NumGet(r, "Double"), g: NumGet(g, "Double"), b: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to hexadecimal format.
     * @returns {Object} An object containing A, R, G, and B values as hexadecimal strings.
     */
    ToHex() => (DllCall("Color\ColorToHex", "Ptr", this.Ptr, "Ptr", a := Buffer(3, 0), "Ptr", r := Buffer(3, 0), "Ptr", g := Buffer(3, 0), "Ptr", b := Buffer(3, 0)), {A: StrGet(a, "UTF-8"), R: StrGet(r, "UTF-8"), G: StrGet(g, "UTF-8"), B: StrGet(b, "UTF-8")})

    /**
     * Converts the color to HSL format.
     * @returns {Object} An object containing H (0-360), S (0-100), L (0-100), and A (0-255) values.
     */
    ToHSL() => (DllCall("Color\ColorToHSL"    , "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0)  , "Ptr", s := Buffer(8, 0), "Ptr", l := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), S: NumGet(s, "Double"), L: NumGet(l, "Double")      , A: NumGet(a, "Int")})

    /**
     * Converts the color to HSV format.
     * @returns {Object} An object containing H (0-360), S (0-100), V (0-100), and A (0-255) values.
     */
    ToHSV() => (DllCall("Color\ColorToHSV", "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0), "Ptr", s := Buffer(8, 0), "Ptr", v := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), S: NumGet(s, "Double"), V: NumGet(v, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to HSI format.
     * @returns {Object} An object containing H (0-360), S (0-100), I (0-100), and A (0-255) values.
     */
    ToHSI() => (DllCall("Color\ColorToHSI", "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0), "Ptr", s := Buffer(8, 0), "Ptr", i := Buffer(8, 0), "Ptr", a := Buffer(4, 0), "Cdecl"), {H: NumGet(h, "Double"), S: NumGet(s, "Double"), I: NumGet(i, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to HWB format.
     * @returns {Object} An object containing H (0-360), W (0-100), B (0-100), and A (0-255) values.
     */
    ToHWB() => (DllCall("Color\ColorToHWB"    , "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0)  , "Ptr", w := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), W: NumGet(w, "Double"), B: NumGet(b, "Double")      , A: NumGet(a, "Int")})

    /**
     * Converts the color to XYZ D50 format.
     * @returns {Object} An object containing X, Y, Z, and A values.
     */
    ToXYZ_D50() => (DllCall("Color\ColorToXYZ_D50"    , "Ptr", this.Ptr, "Ptr", x := Buffer(8, 0)  , "Ptr", y := Buffer(8, 0), "Ptr", z := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {X: NumGet(x, "Double"), Y: NumGet(y, "Double"), Z: NumGet(z, "Double")      , A: NumGet(a, "Int")})

    /**
     * Converts the color to XYZ D65 format.
     * @returns {Object} An object containing X, Y, Z, and A values.
     */
    ToXYZ_D65() => (DllCall("Color\ColorToXYZ_D65"    , "Ptr", this.Ptr, "Ptr", x := Buffer(8, 0)  , "Ptr", y := Buffer(8, 0), "Ptr", z := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {X: NumGet(x, "Double"), Y: NumGet(y, "Double"), Z: NumGet(z, "Double")      , A: NumGet(a, "Int")})

    /**
     * Converts the color to CIELab format.
     * @returns {Object} An object containing L, A, B, and T (alpha) values.
     */
    ToLab() => (DllCall("Color\ColorToLab"    , "Ptr", this.Ptr, "Ptr", l := Buffer(8, 0)  , "Ptr", a := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", t := Buffer(4, 0)), {L: NumGet(l, "Double"), A: NumGet(a, "Double"), B: NumGet(b, "Double")      , T: NumGet(t, "Int")})

    /**
     * Converts the color to LCHuv format.
     * @returns {Object} An object containing L, C, H, and A (alpha) values.
     */
    ToLCHuv() => (DllCall("Color\ColorToLCHuv", "Ptr", this.Ptr, "Ptr", l := Buffer(8, 0), "Ptr", c := Buffer(8, 0), "Ptr", h := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {L: NumGet(l, "Double"), C: NumGet(c, "Double"), H: NumGet(h, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to CIE Luv format.
     * @returns {Object} An object containing L, u, v, and A (alpha) values.
     */
    ToLuv() => (DllCall("Color\ColorToLuv", "Ptr", this.Ptr, "Ptr", L := Buffer(8, 0), "Ptr", u := Buffer(8, 0), "Ptr", v := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {L: NumGet(L, "Double"), u: NumGet(u, "Double"), v: NumGet(v, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to LCH format.
     * @returns {Object} An object containing L (0-100), C (0-100), H (0-360), and A (0-255) values.
     */
    ToLCHab() => (DllCall("Color\ColorToLCHab", "Ptr", this.Ptr, "Ptr", l := Buffer(8, 0), "Ptr", c := Buffer(8, 0), "Ptr", h := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {L: NumGet(l, "Double"), C: NumGet(c, "Double"), H: NumGet(h, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to YPbPr format.
     * @returns {Object} An object containing Y (0-1), Cb (0-1), Cr (0-1), and A (0-255) values.
     */
    ToYPbPr() => (DllCall("Color\ColorToYPbPr", "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0), "Ptr", cb := Buffer(8, 0), "Ptr", cr := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Y: NumGet(y, "Double"), Cb: NumGet(cb, "Double"), Cr: NumGet(cr, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to YIQ format.
     * @returns {Object} An object containing Y, I, Q, and A values.
     */
    ToYIQ() => (DllCall("Color\ColorToYIQ"    , "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0)  , "Ptr", i := Buffer(8, 0), "Ptr", q := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Y: NumGet(y, "Double"), I: NumGet(i, "Double"), Q: NumGet(q, "Double")      , A: NumGet(a, "Int")})

    /**
     * Converts the color to NCol format.
     * @returns {Object} An object containing H (hue name), W (whiteness), B (blackness), and A values.
     */
    ToNCol() => (DllCall("Color\ColorToNCol"   , "Ptr", this.Ptr, "Ptr", h := Buffer(16, 0) , "Ptr", w := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: StrGet(h, "UTF-8") , W: NumGet(w, "Double"), B: 100 - NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to CMYK format.
     * @returns {Object} An object containing C, M, Y, K, and A values.
     */
    ToCMYK() => (DllCall("Color\ColorToCMYK"   , "Ptr", this.Ptr, "Ptr", c := Buffer(8, 0)  , "Ptr", m := Buffer(8, 0), "Ptr", y := Buffer(8, 0), "Ptr", k := Buffer(8, 0) , "Ptr", a := Buffer(4, 0)), {C: NumGet(c, "Double"), M: NumGet(m, "Double"), Y: NumGet(y, "Double"), K: NumGet(k, "Double"), A: NumGet(a, "Int")})

    /**
     * Generates an analogous color scheme.
     * @returns {Color[]} An array of Color objects representing the color scheme.
     */
    Analogous(angle := 30, count := 3)
    {
        pColors := DllCall("Color\ColorAnalogous", "Ptr", this.ptr, "Double", angle, "Int", count, "Ptr", Buffer(count * A_PtrSize, 0), "Ptr")

        colors := []
        Loop count
            colors.Push(Color.FromPtr(NumGet(pColors, (A_Index - 1) * A_PtrSize, "Ptr")))

        return colors
    }

    /**
     * Generates a triadic color scheme.
     * @returns {Color[]} An array of three Color objects representing the triadic color scheme.
     */
    Triadic(angle := 30)
    {
        pColors := DllCall("Color\ColorTriadic", "Ptr", this.ptr, "Double", angle, "Ptr", Buffer(3 * A_PtrSize, 0), "Ptr")

        colors := []
        Loop 3
            colors.Push(Color.FromPtr(NumGet(pColors, (A_Index - 1) * A_PtrSize, "Ptr")))

        return colors
    }

    /**
     * Generates a tetradic color scheme.
     * @returns {Color[]} An array of four Color objects representing the tetradic color scheme.
     */
    Tetradic(angle := 45)
    {
        pColors := DllCall("Color\ColorTetradic", "Ptr", this.ptr, "Double", angle, "Ptr", Buffer(4 * A_PtrSize, 0), "Ptr")

        colors := []
        Loop 4
            colors.Push(Color.FromPtr(NumGet(pColors, (A_Index - 1) * A_PtrSize, "Ptr")))

        return colors
    }

    /**
     * Generates a monochromatic color scheme.
     * @param {number} [count=5] - The number of colors to generate in the scheme.
     * @returns {Color[]} An array of Color objects representing the monochromatic color scheme.
     */
    Monochromatic(count := 5)
    {
        pColors := DllCall("Color\ColorMonochromatic", "Ptr", this.ptr, "Int", count, "Ptr", Buffer(count * A_PtrSize, 0), "Ptr")

        colors := []
        Loop count
            colors.Push(Color.FromPtr(NumGet(pColors, (A_Index - 1) * A_PtrSize, "Ptr")))

        return colors
    }

    /**
     * Generates a square color scheme.
     * @returns {Color[]} An array of Color objects representing the square color scheme.
     */
    Square() => this.Analogous(90, 4)

    ApplyColorMatrix(matrix)
    {
        return Color.FromPtr(DllCall("Color\ColorApplyMatrix", "Ptr", this.Ptr,
            "Double", matrix[1][1], "Double", matrix[1][2], "Double", matrix[1][3], "Double", matrix[1][4], "Double", matrix[1][5],
            "Double", matrix[2][1], "Double", matrix[2][2], "Double", matrix[2][3], "Double", matrix[2][4], "Double", matrix[2][5],
            "Double", matrix[3][1], "Double", matrix[3][2], "Double", matrix[3][3], "Double", matrix[3][4], "Double", matrix[3][5],
            "Double", matrix[4][1], "Double", matrix[4][2], "Double", matrix[4][3], "Double", matrix[4][4], "Double", matrix[4][5],
            "Double", matrix[5][1], "Double", matrix[5][2], "Double", matrix[5][3], "Double", matrix[5][4], "Double", matrix[5][5],
            "Ptr"))
    }

    /**
     * Defines default format strings for various color representations.
     */
    class DefaultFormatString
    {
        /**
         * Default format string for RGB color representation.
         * @type {string}
         */
        static RGB  := "rgba({R}, {G}, {B}, {A})"

        static LinearSRGB := "srgb({R:3}, {G:3}, {B:3})"

        /**
         * Default format string for hexadecimal color representation.
         * @type {string}
         */
        static Hex  := "0x{A}{R}{G}{B}"

        /**
         * Default format string for HSL color representation.
         * @type {string}
         */
        static HSL  := "hsl({H}, {S}%, {L}%)"

        /**
         * Default format string for HWB color representation.
         * @type {string}
         */
        static HWB  := "hwb({H}, {W}%, {B}%)"

        /**
         * Default format string for XYZ color representation.
         * @type {string}
         */
        static XYZ  := "xyz({X}, {Y}, {Z})"

        /**
         * Default format string for Lab color representation.
         * @type {string}
         */
        static Lab  := "lab({L}, {A}, {B})"

        /**
         * Default format string for YIQ color representation.
         * @type {string}
         */
        static YIQ  := "yiq({Y:3}, {I:3}, {Q:3})"

        /**
         * Default format string for NCol color representation.
         * @type {string}
         */
        static NCol := "ncol({H}, {W}%, {B}%)"

        /**
         * Default format string for CMYK color representation.
         * @type {string}
         */
        static CMYK := "cmyk({C}, {M}, {Y}, {K})"

        /**
         * Default format string for HSV color representation.
         * @type {string}
         */
        static HSV := "hsv({H}, {S}%, {V}%)"

        static HSI := "hsi({H}, {S}%, {I})"

        /**
         * Default format string for YPbPr color representation.
         * @type {string}
         */
        static YPbPr := "ypbpr({Y:3}, {Cb:3}, {Cr:3})"

        /**
         * Default format string for LCH color representation.
         * @type {string}
         */
        static LCHab := "lchab({L}, {C}, {H})"

        static LCHuv := "lchuv({L}, {C}, {H})"

        /**
         * Default format string for Adobe RGB color representation.
         * @type {string}
         */
        static AdobeRGB := "adobe({R:3}, {G:3}, {B:3})"

        /**
         * Default format string for ProPhoto RGB color representation.
         * @type {string}
         */
        static ProPhotoRGB := "prophoto({R:3}, {G:3}, {B:3})"

        /**
         * Default format string for CIE Luv color representation.
         * @type {string}
         */
        static Luv := "luv({L}, {U}, {V})"
    }
}

class ColorBuffer
{
    __New(gradientOrWidth, height?, col?)
    {
        if gradientOrWidth is Gradient  ; If first param is a Gradient
        {
            if !IsSet(height)
                throw ValueError("Height parameter required when creating ColorBuffer from Gradient")
            this.Ptr := DllCall("Color\CreateColorBufferFromGradient", "Ptr", gradientOrWidth.Ptr, "Int", height, "Int", color ?? height, "Ptr")
        }
        else  ; Existing width/height constructor
        {
            this.Ptr := DllCall("Color\CreateColorBufferFromWHC", "Int", gradientOrWidth, "Int", height, "Ptr", (col ?? Color.Transparent).Ptr, "Ptr")
        }
    }

    /**
     * Deletes the ColorBuffer and frees associated resources.
     */
    __Delete() => DllCall("Color\DeleteColorBuffer", "Ptr", this.Ptr)

    /**
     * Gets or sets a color at the specified coordinates.
     * @param {number} x - The x-coordinate.
     * @param {number} [y] - The y-coordinate. If omitted, x is treated as a linear index.
     * @returns {Color} The color at the specified coordinates.
     */
    __Item[x, y]
    {
        get => this.GetColor(x, y)
        set => this.SetColor(x, y, value)
    }

    /**
     * Provides enumeration functionality for the ColorBuffer.
     * @param {number} num - The number of values to return for each iteration.
     * @returns {function} An enumerator function based on the specified number.
     * @throws {ValueError} If an invalid enumerator type is specified.
     */
    __Enum(num)
    {
        bufferSize := DllCall("Color\ColorBufferSize", "Ptr", this.Ptr, "Ptr")

        enumColor(&col)
        {
            static index := 0
            if (index >= bufferSize)
            {
                index := 0
                return false
            }
            col := Color.FromPtr(DllCall("Color\ColorBufferGetAt", "Ptr", this.Ptr, "Ptr", index, "Ptr"))
            index++
            return true
        }

        enumXY(&x, &y)
        {
            static index := 0
            if (index >= bufferSize)
            {
                index := 0
                return false
            }
            DllCall("Color\ColorBufferGetXY", "Ptr", this.Ptr, "Ptr", index, "Int*", &x := 0, "Int*", &y := 0)
            index++
            return true
        }

        enumXYColor(&x, &y, &col)
        {
            static index := 0
            if (index >= bufferSize)
            {
                index := 0
                return false
            }
            DllCall("Color\ColorBufferGetXY", "Ptr", this.Ptr, "Ptr", index, "Int*", &x := 0, "Int*", &y := 0)
            col := Color.FromPtr(DllCall("Color\ColorBufferGetAt", "Ptr", this.Ptr, "Ptr", index, "Ptr"))
            index++
            return true
        }

        switch num
        {
            case 1: return enumColor
            case 2: return enumXY
            case 3: return enumXYColor
            default: throw ValueError("Invalid enumerator type")
        }
    }

    /**
     * Gets the width of the ColorBuffer.
     * @returns {number}
     */
    Width
    {
        get => DllCall("Color\GetColorBufferWidth", "Ptr", this.Ptr, "Int")
    }

    /**
     * Gets the height of the ColorBuffer.
     * @returns {number}
     */
    Height
    {
        get => DllCall("Color\GetColorBufferHeight", "Ptr", this.Ptr, "Int")
    }

    /**
     * Gets the total number of pixels in the ColorBuffer.
     * @returns {number}
     */
    Size
    {
        get => DllCall("Color\GetColorBufferSize", "Ptr", this.Ptr, "Int")
    }

    Stride
    {
        get => DllCall("Color\GetColorBufferStride", "Ptr", this.Ptr, "Int")
    }

    /**
     * Creates a ColorBuffer from a BitmapBuffer object.
     * @static
     * @param {ImagePut.BitmapBuffer} bBuffer - The BitmapBuffer object to create the ColorBuffer from.
     * @returns {ColorBuffer}
     */
    static FromBitmapBuffer(bBuffer)
    {
        if Type(bBuffer) == "ImagePut.BitmapBuffer"
            return ColorBuffer.FromPtr(DllCall("Color\CreateColorBufferFromBuffer", "Ptr", bBuffer.Ptr, "Int", bBuffer.Width, "Int", bBuffer.Height, "Ptr"))
        else
            throw ValueError("Invalid BitmapBuffer object or incorrect type.")
    }

    /**
     * Creates a ColorBuffer from a 2D array of Color objects.
     * @static
     * @param {Color[[],[],...]} colors - A 2D array of Color objects.
     * @returns {ColorBuffer}
     */
    static FromArray(colors)
    {
        if (colors is Array) and (colors[1] is Array)
        {
            width := colors[1][1].Length
            height := colors[1].Length
            colorArray := Buffer(width * height * A_PtrSize)

            for y, row in colors[1]
                for x, c in row
                    NumPut("Ptr", c.Ptr, colorArray, ((y - 1) * width + (x - 1)) * A_PtrSize)

            this.Ptr := DllCall("Color\CreateColorBufferFromArray", "Ptr", colorArray.Ptr, "Int", width, "Int", height, "Ptr")
        }
    }

    /**
     * Creates a ColorBuffer from an HBITMAP handle.
     * @static
     * @param {Ptr} hBitmap - The HBITMAP handle.
     * @param {number} [width=0] - The desired width of the ColorBuffer. If 0, uses the bitmap's width.
     * @param {number} [height=0] - The desired height of the ColorBuffer. If 0, uses the bitmap's height.
     * @returns {ColorBuffer} A new ColorBuffer instance created from the HBITMAP.
     */
    static FromHBITMAP(hBitmap, width := 0, height := 0) => ColorBuffer.FromPtr(DllCall("Color\CreateColorBufferFromHBITMAP", "Ptr", hBitmap, "Int", width, "Int", height, "Ptr"))

    /**
     * Exports the ColorBuffer as an HBITMAP.
     * @param {number} [width=0] - The desired width of the HBITMAP. If 0, uses the ColorBuffer's width.
     * @param {number} [height=0] - The desired height of the HBITMAP. If 0, uses the ColorBuffer's height.
     * @returns {Ptr} The HBITMAP handle of the exported image.
     */
    ToHBITMAP(width := 0, height := 0) => DllCall("Color\ExportColorBufferAsHBitmap", "Ptr", this.Ptr, "Int", width, "Int", height, "Ptr")

    /**
     * Resizes the ColorBuffer to the specified dimensions.
     * @param {number} width - The new width.
     * @param {number} height - The new height.
     * @returns {ColorBuffer}
     */
    Resize(width, height, resizeImage := true, fillColor := Color.Transparent) => (DllCall("Color\ResizeColorBuffer", "Ptr", this.Ptr, "Int", width, "Int", height, "Int", resizeImage, "Ptr", fillColor.Ptr), this)

    Scale(scale) => (DllCall("Color\ScaleColorBuffer", "Ptr", this.Ptr, "Double", scale), this)

    /**
     * Rotates the ColorBuffer by the specified angle.
     * @param {number} angle - The rotation angle in degrees.
     * @returns {ColorBuffer}
     */
    Rotate(angle) => (DllCall("Color.dll\RotateColorBuffer", "Ptr", this.Ptr, "Double", angle), this)

    /**
     * Gets the color as an integer (0xAARRGGBB) at the specified coordinates.
     * @param {number} x - The x-coordinate.
     * @param {number} y - The y-coordinate.
     * @returns {number}
     */
    GetInt(x, y) => DllCall("Color\GetColorIntFromBuffer", "Ptr", this.Ptr, "Int", x, "Int", y, "UInt")

    /**
     * Sets the color as an integer (0xAARRGGBB) at the specified coordinates.
     * @param {number} x - The x-coordinate.
     * @param {number} y - The y-coordinate.
     * @param {number} col
     */
    SetInt(x, y, col) => DllCall("Color\SetColorIntInBuffer", "Ptr", this.Ptr, "Int", x, "Int", y, "UInt", col)

    /**
     * Gets the Color object at the specified coordinates.
     * @param {number} x - The x-coordinate.
     * @param {number} y - The y-coordinate.
     * @returns {Color}
     */
    GetColor(x, y) => Color.FromPtr(DllCall("Color\GetColorFromBuffer", "Ptr", this.Ptr, "Int", x, "Int", y, "Ptr"))

    /**
     * Sets the Color object at the specified coordinates.
     * @param {number} x - The x-coordinate.
     * @param {number} y - The y-coordinate.
     * @param {Color} color - The Color object to set.
     * @returns {ColorBuffer}
     */
    SetColor(x, y, color) => (DllCall("Color\SetColorInBuffer", "Ptr", this.Ptr, "Int", x, "Int", y, "Ptr", color.Ptr), this)

    /**
     * Shifts the red component of all colors in the buffer by the specified amount.
     * @param {number} amount - The amount to shift the red component.
     * @returns {ColorBuffer}
     */
    ShiftRed(amount) => (DllCall("Color\ShiftRedColorBuffer", "Ptr", this.Ptr, "Int", amount), this)

    /**
     * Shifts the green component of all colors in the buffer by the specified amount.
     * @param {number} amount - The amount to shift the green component.
     * @returns {ColorBuffer}
     */
    ShiftGreen(amount) => (DllCall("Color\ShiftGreenColorBuffer", "Ptr", this.Ptr, "Int", amount), this)

    /**
     * Shifts the blue component of all colors in the buffer by the specified amount.
     * @param {number} amount - The amount to shift the blue component.
     * @returns {ColorBuffer}
     */
    ShiftBlue(amount) => (DllCall("Color\ShiftBlueColorBuffer", "Ptr", this.Ptr, "Int", amount), this)

    /**
     * Shifts the alpha component of all colors in the buffer by the specified amount.
     * @param {number} amount - The amount to shift the alpha component.
     * @returns {ColorBuffer}
     */
    ShiftAlpha(amount) => (DllCall("Color\ShiftAlphaColorBuffer", "Ptr", this.Ptr, "Int", amount), this)

    /**
     * Sets the red component of all colors in the buffer to the specified value.
     * @param {number} value - The value to set for the red component.
     * @returns {ColorBuffer}
     */
    SetRed(value) => (DllCall("Color\SetRedColorBuffer", "Ptr", this.Ptr, "Int", value), this)

    /**
     * Sets the green component of all colors in the buffer to the specified value.
     * @param {number} value - The value to set for the green component.
     * @returns {ColorBuffer}
     */
    SetGreen(value) => (DllCall("Color\SetGreenColorBuffer", "Ptr", this.Ptr, "Int", value), this)

    /**
     * Sets the blue component of all colors in the buffer to the specified value.
     * @param {number} value - The value to set for the blue component.
     * @returns {ColorBuffer}
     */
    SetBlue(value) => (DllCall("Color\SetBlueColorBuffer", "Ptr", this.Ptr, "Int", value), this)

    /**
     * Sets the alpha component of all colors in the buffer to the specified value.
     * @param {number} value - The value to set for the alpha component.
     * @returns {ColorBuffer}
     */
    SetAlpha(value) => (DllCall("Color\SetAlphaColorBuffer", "Ptr", this.Ptr, "Int", value), this)

    ApplyMatrix(matrix)
    {
        DllCall("Color\ColorBufferApplyMatrix", "Ptr", this.Ptr,
            "Double", matrix[1][1], "Double", matrix[1][2], "Double", matrix[1][3], "Double", matrix[1][4], "Double", matrix[1][5],
            "Double", matrix[2][1], "Double", matrix[2][2], "Double", matrix[2][3], "Double", matrix[2][4], "Double", matrix[2][5],
            "Double", matrix[3][1], "Double", matrix[3][2], "Double", matrix[3][3], "Double", matrix[3][4], "Double", matrix[3][5],
            "Double", matrix[4][1], "Double", matrix[4][2], "Double", matrix[4][3], "Double", matrix[4][4], "Double", matrix[4][5],
            "Double", matrix[5][1], "Double", matrix[5][2], "Double", matrix[5][3], "Double", matrix[5][4], "Double", matrix[5][5])
        return this
    }

    /**
     * Inverts all colors in the buffer.
     * @returns {ColorBuffer}
     */
    Invert() => (DllCall("Color\InvertColorBuffer", "Ptr", this.Ptr), this)

    /**
     * Applies the complement effect to all colors in the buffer.
     * @returns {ColorBuffer}
     */
    Complement() => (DllCall("Color\ComplementColorBuffer", "Ptr", this.Ptr), this)

    /**
     * Shifts the hue of all colors in the buffer by the specified degrees.
     * @param {number} degrees - The amount to shift the hue in degrees.
     * @returns {ColorBuffer}
     */
    ShiftHue(degrees) => (DllCall("Color\ShiftHueColorBuffer", "Ptr", this.Ptr, "Double", degrees), this)

    /**
     * Converts all colors in the buffer to grayscale.
     * @returns {ColorBuffer}
     */
    Grayscale() => (DllCall("Color\GrayscaleColorBuffer", "Ptr", this.Ptr), this)

    /**
     * Applies a sepia tone effect to all colors in the buffer.
     * @returns {ColorBuffer}
     */
    Sepia(factor) => (DllCall("Color\SepiaColorBuffer", "Ptr", this.Ptr, "Double", factor), this)

    CrossProcess(factor) => (DllCall("Color\CrossProcessColorBuffer", "Ptr", this.Ptr, "Double", factor), this)

    Moonlight(factor) => (DllCall("Color\MoonlightColorBuffer", "Ptr", this.Ptr, "Double", factor), this)

    VintageFilm(factor) => (DllCall("Color\VintageFilmColorBuffer", "Ptr", this.Ptr, "Double", factor), this)

    Technicolor(factor) => (DllCall("Color\TechnicolorColorBuffer", "Ptr", this.Ptr, "Double", factor), this)

    Polaroid(factor) => (DllCall("Color\PolaroidColorBuffer", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Increases the saturation of all colors in the buffer.
     * @param {number} amount - The amount to increase saturation.
     * @returns {ColorBuffer}
     */
    ShiftSaturation(amount) => (DllCall("Color\ShiftSaturationColorBuffer", "Ptr", this.Ptr, "Double", amount), this)

    /**
     * Lightens all colors in the buffer.
     * @param {number} amount - The amount to lighten the colors.
     * @returns {ColorBuffer}
     */
    ShiftLightness(amount) => (DllCall("Color\ShiftLightnessColorBuffer", "Ptr", this.Ptr, "Double", amount), this)

    ShiftValue(amount) => (DllCall("Color\ShiftValueColorBuffer", "Ptr", this.Ptr, "Double", amount), this)

    ShiftIntensity(amount) => (DllCall("Color\ShiftIntensityColorBuffer", "Ptr", this.Ptr, "Double", amount), this)

    ShiftWhiteLevel(amount) => (DllCall("Color\ShiftWhiteLevelColorBuffer", "Ptr", this.Ptr, "Double", amount), this)

    ShiftBlackLevel(amount) => (DllCall("Color\ShiftBlackLevelColorBuffer", "Ptr", this.Ptr, "Double", amount), this)

    ShiftContrast(amount) => (DllCall("Color\ShiftContrastColorBuffer", "Ptr", this.Ptr, "Double", amount), this)

    /**
     * Applies a pixelation effect to the buffer.
     * @param {number} pixelSize - The size of each "pixel" in the pixelation effect.
     * @returns {ColorBuffer}
     */
    Pixelate(pixelSize) => (DllCall("Color.dll\PixelateColorBuffer", "Ptr", this.Ptr, "Int", pixelSize), this)

    /**
     * Applies a blur effect to the buffer.
     * @param {number} radius - The radius of the blur effect.
     * @returns {ColorBuffer}
     */
    Blur(radius) => (DllCall("Color.dll\BlurColorBuffer", "Ptr", this.Ptr, "Int", radius), this)

    GaussianBlur(sigma) => (DllCall("Color.dll\GaussianBlurColorBuffer", "Ptr", this.Ptr, "Float", sigma), this)

    /**
     * Applies a sharpening effect to the buffer.
     * @param {number} amount - The intensity of the sharpening effect.
     * @returns {ColorBuffer}
     */
    Sharpen(amount) => (DllCall("Color.dll\SharpenColorBuffer", "Ptr", this.Ptr, "Float", amount), this)

    /**
     * Adjusts the contrast of all colors in the buffer.
     * @param {number} factor - The contrast adjustment factor.
     * @returns {ColorBuffer}
     */
    AdjustContrast(factor) => (DllCall("Color\AdjustContrastColorBuffer", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Adjusts the color balance of the buffer.
     * @param {number} redFactor - The adjustment factor for the red channel.
     * @param {number} greenFactor - The adjustment factor for the green channel.
     * @param {number} blueFactor - The adjustment factor for the blue channel.
     * @returns {ColorBuffer}
     */
    ColorBalance(redFactor, greenFactor, blueFactor) => (DllCall("Color\AdjustColorBalanceColorBuffer", "Ptr", this.Ptr, "Double", redFactor, "Double", greenFactor, "Double", blueFactor), this)

    /**
     * Overlays another ColorBuffer onto this one.
     * @param {ColorBuffer} overlayBuffer - The ColorBuffer to overlay.
     * @param {number} x - The x-coordinate to place the overlay.
     * @param {number} y - The y-coordinate to place the overlay.
     * @param {number} opacity - The opacity of the overlay (0 to 255).
     * @returns {ColorBuffer}
     */
    OverlayColorBuffer(overlayBuffer, x, y, opacity := 1.0) => (DllCall("Color\OverlayImageColorBuffer", "Ptr", this.Ptr, "Ptr", overlayBuffer.Ptr, "Int", x, "Int", y, "Double", opacity), this)

    /**
     * Applies an emboss effect to the buffer.
     * @param {number} [depth=1.0] - The depth of the emboss effect.
     * @param {number} [angle=45.0] - The angle of the emboss effect in degrees.
     * @returns {ColorBuffer}
     */
    Emboss(depth := 1.0, angle := 45.0) => (DllCall("Color\ApplyEmbossColorBuffer", "Ptr", this.Ptr, "Double", depth, "Double", angle), this)

    /**
     * Applies an edge detection effect to the buffer.
     * @returns {ColorBuffer}
     */
    EdgeDetect() => (DllCall("Color\ApplyEdgeDetectColorBuffer", "Ptr", this.Ptr), this)

    /**
     * Flips the buffer horizontally or vertically.
     * @param {boolean} [horizontal=true] - If true, flips horizontally; if false, flips vertically.
     * @returns {ColorBuffer}
     */
    Flip(horizontal := true) => (DllCall("Color.dll\FlipColorBuffer", "Ptr", this.Ptr, "Int", horizontal), this)

    /**
     * Crops the buffer to the specified dimensions.
     * @param {number} x - The x-coordinate of the top-left corner of the crop area.
     * @param {number} y - The y-coordinate of the top-left corner of the crop area.
     * @param {number} width - The width of the crop area.
     * @param {number} height - The height of the crop area.
     * @returns {ColorBuffer}
     */
    Crop(x, y, width, height) => (DllCall("Color.dll\CropColorBuffer", "Ptr", this.Ptr, "Int", x, "Int", y, "Int", width, "Int", height), this)

    /**
     * Calculates the average color of the entire buffer.
     * @returns {Color}
     */
    AverageColor() => Color.FromPtr((DllCall("Color\AverageColorBuffer", "Ptr", this.Ptr, "Ptr")))

    /**
     * Applies a vignette effect to the buffer.
     * @param {number} [strength=0.3] - The strength of the vignette effect (0.0 to 1.0).
     * @param {number} [radius=1.0] - The radius of the vignette effect.
     * @returns {ColorBuffer} The current ColorBuffer instance (for method chaining).
     */
    Vignette(strength := 0.3, radius := 1.0) => (DllCall("Color.dll\ApplyVignetteColorBuffer", "Ptr", this.Ptr, "Double", strength, "Double", radius), this)

    /**
     * Applies a two-color noise effect to the buffer.
     * @param {number} [density=0.05] - The density of the noise (0.0 to 1.0).
     * @param {Color} [saltColor=Color.White] - The first color.
     * @param {Color} [pepperColor=Color.Black] - The second color.
     * @returns {ColorBuffer}
     */
    TwoColorNoise(density := 0.05, saltColor := Color.White, pepperColor := Color.Black) => (DllCall("Color.dll\ApplyTwoColorNoiseColorBuffer", "Ptr", this.Ptr, "Double", density, "Ptr", saltColor.Ptr, "Ptr", pepperColor.Ptr), this)

    /**
     * Applies Gaussian noise to the buffer.
     * @param {number} [mean=0.0] - The mean of the Gaussian distribution.
     * @param {number} [stdDev=10.0] - The standard deviation of the Gaussian distribution.
     * @returns {ColorBuffer}
     */
    GaussianNoise(mean := 0.0, stdDev := 10.0) => (DllCall("Color.dll\ApplyGaussianNoiseColorBuffer", "Ptr", this.Ptr, "Double", mean, "Double", stdDev), this)

    /**
     * Applies Perlin noise to the color buffer.
     * @param {number} frequency - Controls the scale of the noise pattern. Higher values create smaller, more frequent patterns.
     * @param {number} amplitude - Determines the strength of the noise effect. Higher values create more pronounced changes.
     * @param {number} octaves - The number of noise layers to combine. More octaves add finer details but increase computation time.
     * @param {number} persistence - Controls how quickly the amplitude decreases for each octave. `0-1`, where lower values create smoother noise.
     * @param {number} lacunarity - Determines how quickly the frequency increases for each octave. Values greater than 1 increase detail in higher octaves.
     * @returns {this} The ColorBuffer object, allowing for method chaining.
     */
    PerlinNoise(frequency := 4.0, amplitude := 1.0, octaves := 4, persistence := 0.5, lacunarity := 2.0) => (DllCall("Color.dll\ApplyPerlinNoiseColorBuffer", "Ptr", this.Ptr, "Double", frequency, "Double", amplitude, "Int", octaves, "Double", persistence, "Double", lacunarity), this)

    SimplexNoise(frequency := 4.0, amplitude := 1.0, octaves := 4, persistence := 0.5, lacunarity := 2.0) => (DllCall("Color.dll\ApplySimplexNoiseColorBuffer", "Ptr", this.Ptr, "Double", frequency, "Double", amplitude, "Int", octaves, "Double", persistence, "Double", lacunarity), this)

    FractalBrownianMotion(frequency := 4.0, amplitude := 1.0, octaves := 4, persistence := 0.5, lacunarity := 2.0) => (DllCall("Color.dll\ApplyFractalBrownianMotionColorBuffer", "Ptr", this.Ptr, "Double", frequency, "Double", amplitude, "Int", octaves, "Double", persistence, "Double", lacunarity), this)

    /**
     * Applies a Voronoi diagram effect to the color buffer.
     * @param {number} [numPoints=20] - The number of seed points for the Voronoi diagram. More points create a more complex cellular pattern.
     * @param {number} [falloff=1.0] - Controls the sharpness of cell edges. Higher values create sharper edges, while lower values create smoother transitions.
     * @param {number} [strength=1] - How strongly the effect is applied. Higher values create more pronounced changes.
     * @returns {this} The ColorBuffer object, allowing for method chaining.
     */
    VoronoiDiagram(points := 20, falloff := 1, strength := 1) => (DllCall("Color.dll\ApplyVoronoiDiagramColorBuffer", "Ptr", this.Ptr, "Int", points, "Double", falloff, "Double", strength), this)

    /**
     * Applies a static Plasma effect to the color buffer, creating vibrant, swirling patterns.
     * @param {number} [frequency=5.0] - Controls the density of the plasma pattern. Higher values create more intricate patterns.
     * @param {number} [phase=0.0] - Adjusts the overall pattern shift. Different values create unique variations of the pattern.
     * @returns {this} The ColorBuffer object, allowing for method chaining.
     */
    Plasma(frequency := 5, phase := 0) => (DllCall("Color.dll\PlasmaEffectColorBuffer", "Ptr", this.Ptr, "Double", frequency, "Double", phase), this)

    DiamondSquare(roughness := 5, waterLevel := 1, levelsPerStop := 5) => (DllCall("Color.dll\DiamondSquareEffectColorBuffer", "Ptr", this.Ptr, "Double", roughness, "Double", waterLevel, "Double", levelsPerStop), this)

    Posterize(levels := 4) => (DllCall("Color.dll\PosterizeColorBuffer", "Ptr", this.Ptr, "Int", levels), this)

    /**
     * Creates a deep copy of the ColorBuffer.
     * @returns {ColorBuffer} A new ColorBuffer instance that is a copy of the current one.
     */
    Copy() => ColorBuffer.FromPtr(DllCall("Color\CopyColorBuffer", "Ptr", this.Ptr, "Ptr"))

    /**
     * Creates a copy of a specific region of the ColorBuffer.
     * @param {number} x - The x-coordinate of the top-left corner of the region.
     * @param {number} y - The y-coordinate of the top-left corner of the region.
     * @param {number} w - The width of the region.
     * @param {number} h - The height of the region.
     * @returns {ColorBuffer} A new ColorBuffer instance containing the specified region.
     */
    CopyRegion(x, y, w, h) => ColorBuffer.FromPtr(DllCall("Color\CopyColorBufferRegion", "Ptr", this.Ptr, "Int", x, "Int", y, "Int", w, "Int", h, "Ptr"))

    /**
     * Applies a mapping function to a specific region of the ColorBuffer.
     * @param {number} x - The x-coordinate of the top-left corner of the region.
     * @param {number} y - The y-coordinate of the top-left corner of the region.
     * @param {number} width - The width of the region.
     * @param {number} height - The height of the region.
     * @param {function} mapFunction - A function that takes (x, y, Color) as arguments and returns a new Color.
     * @returns {ColorBuffer}
     */
    MapRegion(x, y, width, height, mapFunction)
    {
        callbackPtr := CallbackCreate((x, y, c) => mapFunction(x, y, Color(c)).ToInt(), "F", 3)
        DllCall("Color\MapColorsInBuffer", "Ptr", this.Ptr, "Int", x, "Int", y, "Int", width, "Int", height, "Ptr", callbackPtr)
        CallbackFree(callbackPtr)
        return this
    }

    /**
     * Applies a mapping function to the entire ColorBuffer.
     * @param {function} mapFunction - A function that takes (x, y, Color) as arguments and returns a new Color.
     * @returns {ColorBuffer}
     */
    Map(mapFunction) => this.MapRegion(0, 0, this.Width, this.Height, mapFunction)

    ForEach(func) => (callbackPtr := CallbackCreate(func), DllCall("Color\ColorBufferForEach", "Ptr", this.Ptr, "Ptr", callbackPtr), CallbackFree(callbackPtr), this)

    Find(color) => DllCall("Color\ColorBufferFind", "Ptr", this.Ptr, "Ptr", color.Ptr, "Int")

    FindLast(color) => DllCall("Color\ColorBufferFindLast", "Ptr", this.Ptr, "Ptr", color.Ptr, "Int")

    FindAll(color)
    {
        countPtr := Buffer(4, 0)
        resultPtr := DllCall("Color\ColorBufferFindAll", "Ptr", this.Ptr, "Ptr", color.Ptr, "Ptr", countPtr, "Ptr")
        count := NumGet(countPtr, 0, "Int")
        result := []

        Loop count
            result.Push(NumGet(resultPtr + (A_Index - 1) * 4, "Int"))

        DllCall("Color\FreeIntArray", "Ptr", resultPtr)
        return result
    }

    Swap(index1, index2) => (DllCall("Color\ColorBufferSwap", "Ptr", this.Ptr, "Int", index1, "Int", index2), this)

    Filter(predicate) => (callbackPtr := CallbackCreate(predicate), resultPtr := DllCall("Color\ColorBufferFilter", "Ptr", this.Ptr, "Ptr", callbackPtr, "Ptr"), CallbackFree(callbackPtr), ColorBuffer.FromPtr(resultPtr))

    Count(color) => DllCall("Color\ColorBufferCount", "Ptr", this.Ptr, "Ptr", color.Ptr, "Int")

    Shuffle() => (DllCall("Color\ColorBufferShuffle", "Ptr", this.Ptr), this)

    Clear() => (DllCall("Color\ColorBufferClear", "Ptr", this.Ptr), this)

    Sort(compare) => (callbackPtr := CallbackCreate(compare), DllCall("Color\ColorBufferSort", "Ptr", this.Ptr, "Ptr", callbackPtr), CallbackFree(callbackPtr))

    AppendRight(other) => (DllCall("Color\ColorBufferAppendRight", "Ptr", this.Ptr, "Ptr", other.Ptr), this)

    AppendBottom(other) => (DllCall("Color\ColorBufferAppendBottom", "Ptr", this.Ptr, "Ptr", other.Ptr), this)

    /**
     * Converts a linear index to x and y coordinates.
     * @param {number} index - The linear index to convert.
     * @param {number} &x - Output parameter for the x-coordinate.
     * @param {number} &y - Output parameter for the y-coordinate.
     * @returns {ColorBuffer}
     */
    XYFromIndex(index, &x, &y) => (DllCall("Color\ColorBufferGetXY", "Ptr", this.Ptr, "Int", index, "Int*", &x, "Int*", &y), this)

    /**
     * Converts x and y coordinates to a linear index.
     * @param {number} x - The x-coordinate.
     * @param {number} y - The y-coordinate.
     * @param {number} `&index` - Output parameter for the linear index.
     * @returns {ColorBuffer}
     */
    IndexFromXY(x, y, &index) => (DllCall("Color\ColorBufferGetIndex", "Ptr", this.Ptr, "Int", x, "Int", y, "Int*", &index), this)

    /**
     * Creates a ColorBuffer instance from a pointer.
     * @param {Ptr} Ptr - The pointer to the ColorBuffer data.
     * @returns {ColorBuffer} A new ColorBuffer instance.
     */
    static FromPtr(Ptr) => {base: ColorBuffer.Prototype, Ptr: Ptr}
}

class Gradient
{
    static Type := {Linear: 0, Radial: 1, Conical: 2}
    static Repetition := {Repeat: 0, Reflect: 1, Pad: 2}

    /**
     * Gets the total number of steps in the gradient.
     * @returns {number}
     */
    Length => DllCall("Color\GradientGetTotalSteps", "Ptr", this.Ptr, "Int")

    /**
     * Alias for Length. Gets the total number of steps in the gradient.
     * @returns {number}
     */
    TotalSteps => this.Length

    /**
     * @constructor
     * @param {number} totalSteps - The total number of steps in the gradient.
     * @param {...Color} colors - The colors to initialize the gradient with.
     */
    __New(totalSteps, colors*)
    {
        colorInts := []

        for col in colors
            colorInts.Push(col.ToInt())

        colorBuffer := Buffer(4 * colorInts.Length)
        for i, colorInt in colorInts
            NumPut("UInt", colorInt, colorBuffer, (i-1) * 4)

        this.Ptr := DllCall("Color\CreateGradient", "Int", totalSteps, "Ptr", colorBuffer.Ptr, "Int", colorInts.Length, "Ptr")
    }

    /**
     * Deletes the Gradient object and frees associated resources.
     * @returns {void}
     */
    __Delete() => DllCall("Color\DeleteGradient", "Ptr", this.Ptr)

    /**
     * Retrieves a Color object at the specified index in the gradient.
     * @param {number} index - The index of the color to retrieve (1-based).
     * @returns {Color} The Color object at the specified index.
     */
    __Item[index]
    {
        get => Color.FromPtr(DllCall("Color\GradientGetColorAtStep", "Ptr", this.Ptr, "Int", index - 1, "Ptr"))
    }

    /**
     * Provides enumeration functionality for the Gradient.
     * @param {number} num - The number of values to return for each iteration (1 or 2).
     * @returns {function} An enumerator function based on the specified number.
     * @throws {ValueError} If an invalid enumerator type is specified.
     */
    __Enum(num)
    {
        totalSteps := DllCall("Color\GradientGetTotalSteps", "Ptr", this.Ptr, "Int")

        enumColor(&col)
        {
            static index := 0
            if (index >= totalSteps)
            {
                index := 0
                return false
            }

            col := Color.FromPtr(DllCall("Color\GradientGetColorAtStep", "Ptr", this.Ptr, "Int", index, "Ptr"))
            index++
            return true
        }

        enumIndexColor(&index, &col)
        {
            static currentIndex := 0
            if (currentIndex >= totalSteps)
            {
                currentIndex := 0
                return false
            }

            index := currentIndex
            col := Color.FromPtr(DllCall("Color\GradientGetColorAtStep", "Ptr", this.Ptr, "Int", currentIndex, "Ptr"))
            currentIndex++
            return true
        }

        switch num
        {
            case 1: return enumColor
            case 2: return enumIndexColor
            default: throw ValueError("Invalid enumerator type")
        }
    }

    /**
     * @method SetType
     * @param {number} type - The type of gradient (Linear, Radial, or Conical).
     */
    SetType(type) => (DllCall("Color\GradientSetType", "Ptr", this.Ptr, "Int", type), this)

    /**
     * @method SetRepetition
     * @param {number} repetition - The repetition mode (Repeat, Reflect, or Pad).
     */
    SetRepetition(repetition) => (DllCall("Color\GradientSetRepetition", "Ptr", this.Ptr, "Int", repetition), this)

    /**
     * @method AddColorStop
     * @param {Color} col - The color to add at the stop.
     * @param {number} position - The position of the color stop (0.0 to 1.0).
     */
    AddColorStop(col, position) => (DllCall("Color\GradientAddColorStop", "Ptr", this.Ptr, "UInt", col.ToInt(), "Float", position), this)
    /**
     * @method RemoveColorStop
     * @param {number} position - The position of the color stop to remove.
     */
    RemoveColorStop(position) => (DllCall("Color\GradientRemoveColorStop", "Ptr", this.Ptr, "Float", position), this)

    /**
     * @method GetColorAt
     * @param {number} position - The position to get the color at (0.0 to 1.0).
     * @returns {Color} The color at the specified position.
     */
    GetColorAt(position) => Color(DllCall("Color\GradientGetColorAt", "Ptr", this.Ptr, "Float", position, "UInt"))

    /**
     * @method Rotate
     * @param {number} angle - The angle to rotate the gradient by.
     */
    Rotate(angle) => (DllCall("Color\GradientRotate", "Ptr", this.Ptr, "Float", angle), this)

    /**
     * @method Reverse
     * @description Reverses the order of colors in the gradient.
     */
    Reverse() => (DllCall("Color\GradientReverse", "Ptr", this.Ptr), this)

    /**
     * @method Shift
     * @param {number} amount - The amount to shift the gradient by.
     */
    Shift(amount) => (DllCall("Color\GradientShift", "Ptr", this.Ptr, "Float", amount), this)

    /**
     * @method Serialize
     * @returns {string} A serialized string representation of the gradient.
     */
    Serialize() => StrGet(DllCall("Color\GradientSerialize", "Ptr", this.Ptr, "Ptr"), "UTF-8")

    /**
     * @method Deserialize
     * @static
     * @param {string} data - The serialized gradient data.
     * @returns {Gradient} A new Gradient instance from the serialized data.
     */
    static Deserialize(data)
    {
        dataBuf := Buffer(StrPut(data, "UTF-8"))
        StrPut(data, dataBuf, "UTF-8")
        ptr := DllCall("Color\GradientDeserialize", "Ptr", dataBuf, "Ptr")
        return Gradient.FromPtr(ptr)
    }

    static TestSerialization(input) {
        buf := Buffer(StrPut(input, "UTF-8"))
        StrPut(input, buf, "UTF-8")
        return StrGet(DllCall("Color\GradientTestSerialization", "Ptr", buf, "Ptr"), "UTF-8")
    }

    /**
     * @method ToHBITMAP
     * @param {number} width - The width of the bitmap.
     * @param {number} height - The height of the bitmap.
     * @returns {number} A handle to the created bitmap.
     */
    ToHBITMAP(width, height) => DllCall("Color\GradientCreateHBITMAP", "Ptr", this.Ptr, "Int", width, "Int", height, "Ptr")

    /**
     * @method ShiftHue
     * @param {number} degrees - The number of degrees to shift the hue by.
     * @returns {Gradient} A new Gradient with shifted hue.
     */
    ShiftHue(degrees) => (DllCall("Color\GradientShiftHue", "Ptr", this.Ptr, "Double", degrees), this)

    /**
     * @method ShiftSaturation
     * @param {number} amount - The amount to shift the saturation by.
     * @returns {Gradient} A new Gradient with shifted saturation.
     */
    ShiftSaturation(amount) => (DllCall("Color\GradientShiftSaturation", "Ptr", this.Ptr, "Double", amount, "Ptr"), this)

    /**
     * @method ShiftLightness
     * @param {number} amount - The amount to shift the lightness by.
     * @returns {Gradient} A new Gradient with shifted lightness.
     */
    ShiftLightness(amount) => (DllCall("Color\GradientShiftLightness", "Ptr", this.Ptr, "Double", amount, "Ptr"), this)

    /**
     * @method ShiftWhiteness
     * @param {number} amount - The amount to shift the whiteness by.
     * @returns {Gradient} A new Gradient with shifted whiteness.
     */
    ShiftWhiteness(amount) => (DllCall("Color\GradientShiftWhiteness", "Ptr", this.Ptr, "Double", amount, "Ptr"), this)

    /**
     * @method ShiftBlackness
     * @param {number} amount - The amount to shift the blackness by.
     * @returns {Gradient} A new Gradient with shifted blackness.
     */
    ShiftBlackness(amount) => (DllCall("Color\GradientShiftBlackness", "Ptr", this.Ptr, "Double", amount, "Ptr"), this)

    /**
     * @method ShiftBlackness
     * @param {number} amount - The amount to shift the blackness by.
     * @returns {Gradient} A new Gradient with shifted blackness.
     */
    ShiftValue(amount) => (DllCall("Color\GradientShiftValue", "Ptr", this.Ptr, "Double", amount, "Ptr"), this)

    /**
     * @method Grayscale
     * @returns {Gradient} A new Gradient converted to grayscale.
     */
    Grayscale() => (DllCall("Color\GradientGrayscale", "Ptr", this.Ptr, "Ptr"), this)

    /**
     * @returns {Gradient} A new Gradient with sepia effect applied.
     */
    Sepia(factor) => (DllCall("Color\GradientSepia", "Ptr", this.Ptr, "Double", factor, "Ptr"), this)

    /**
     * @method Invert
     * @returns {Gradient} A new Gradient with colors inverted.
     */
    Invert() => (DllCall("Color\GradientInvert", "Ptr", this.Ptr, "Ptr"), this)

    Complement() => (DllCall("Color\GradientComplement", "Ptr", this.Ptr, "Ptr"), this)

    /**
     * Creates a Gradient instance from a pointer to existing gradient data.
     * @static
     * @param {Ptr} Ptr - A pointer to the existing gradient data.
     * @returns {Gradient} A new Gradient instance with the base set to Gradient.Prototype and the given Ptr.
     */
    static FromPtr(Ptr) => {base: Gradient.Prototype, Ptr: Ptr}
}

class ColorPicker
{
    ; Configuration variables
    /** @property {String} FontName The font to use for the color preview text. Can be any font installed on your system. */
    FontName := "Maple Mono"

    /** @property {Number} FontSize The font size for the color preview text. */
    FontSize := 16

    /** @property {String} ViewMode The view mode of the color picker. Can be "crosshair", "grid", or any other value which will result in no overlay. */
    ViewMode := "grid"

    /** @property {Integer} UpdateInterval The interval at which the preview will update, in milliseconds. 16ms = ~60 updates / second. */
    UpdateInterval := 16

    /** @property {Boolean} HighlightCenter If True, highlights the pixel that the color is copied from. */
    HighlightCenter := True

    /** @property {Integer} BorderWidth Thickness of preview border, in pixels. */
    BorderWidth := 4

    /** @property {Integer} CrosshairWidth Thickness of crosshair lines, in pixels. */
    CrosshairWidth := 1

    /** @property {Integer} GridWidth Thickness of grid lines, in pixels. */
    GridWidth := 1

    /** @property {Integer} CenterDotRadius Radius of the Center Dot when not in "grid" or "crosshair" mode, in pixels. */
    CenterDotRadius := 2

    /** @property {Integer} TextPadding The padding added above and below the preview Hex String, in pixels (half above, half below) */
    TextPadding := 6

    /** @property {Integer} DefaultCaptureSize The size of area you want to capture around the cursor in pixels (N by N square) */
    DefaultCaptureSize := 19

    /** @property {Integer} DefaultZoomFactor How much to multiply each pixel by. Default is 10x. */
    DefaultZoomFactor := 10

    /** @property {Integer} LargeJumpAmount How many pixels to move the preview window by when holding shift and moving it with the keyboard. */
    LargeJumpAmount := 16

    /** @property {Integer} PreviewXOffset Horizontal offset of the preview window from the cursor, in pixels. */
    PreviewXOffset := 10

    /** @property {Integer} PreviewYOffset Vertical offset of the preview window from the cursor, in pixels. */
    PreviewYOffset := 10

    /** @property {String} HexFormat The format string used to display and output the hex color value. Can use {R}, {G}, {B}, and {A}. */
    HexFormat := "#{R}{G}{B}"

    /** @property {String} RGBFormat The format string used for output of the RGB color value. Can use {R}, {G}, {B}, and {A}. */
    RGBFormat := "rgb({R}, {G}, {B})"

    /** @property {Boolean} Anchored Whether or not the picker should be anchored in place. */
    Anchored := False

    /** @property {Boolean} CanUnanchor Whether or not the picker should be able to be un-anchored. */
    CanUnanchor := True

    /** @property {Integer} AnchorTarget HWND of the window to anchor the picker to, if Anchored is True. */

    /** @property {Integer} AnchoredX If anchored, the `X` position at which the picker should be anchored
     * Relative to the `AnchorTarget`'s position. */
    AnchoredX := 0

    /** @property {Integer} AnchoredY If anchored, the `Y` position at which the picker should be anchored.
     * Relative to the `AnchorTarget`'s position. */
    AnchoredY := 0

    ; Color Configuration. Press "i" to cycle between the two color sets.
    ;===========================  SET 1  ===  SET 2  ================================;
    /** @property {Color[]} TextFGColors Text Foreground colors. Supports 2 indices, any more will be ignored. */
    TextFGColors    := [ Color.White, Color.Black ]

    /** @property {Color[]} TextBGColors Text Background colors. Supports 2 indices, any more will be ignored. */
    TextBGColors    := [ Color.Black, Color.White ]

    /** @property {Color[]} BorderColors Border colors. Supports 2 indices, any more will be ignored. */
    BorderColors    := [ Color.Black, Color.White ]

    /** @property {Color[]} CrosshairColors Crosshair Color. Supports 2 indices, any more will be ignored. */
    CrosshairColors := [ Color.Black, Color.White ]

    /** @property {Color[]} GridColors Grid Color. Supports 2 indices, any more will be ignored. */
    GridColors      := [ Color.Black, Color.White ]

    /** @property {Color[]} HighlightColors Highlight Color for selected grid square. Supports 2 indices, any more will be ignored. */
    HighlightColors := [ Color.White, Color.Black ]

    ; Nothing below this line should need to be changed
    ;===========================================================================;
    /** @property {Object} Color An object containing the current RGB color. Has methods to convert to and from Hex, HSL, HWB, CMYK, and NCol */
    Color := 0 ;Color()

    /** @property {Boolean} Clip Whether to automatically copy the selected color to clipboard. */
    Clip := False

    /** @property {Number} TargetHWND The window or control handle to confine the color picker to. Default is 0 */
    TargetHWND := 0

    /** @property {Function} OnStart The function called when the picker starts. */
    OnStart := 0

    /** @property {Function} OnUpdate The function called when the picker is updated. Passed the `ColorPicker.Color` object. */
    OnUpdate := 0

    /** @property {Function} OnExit The function called when the picker is closed. */
    OnExit := 0

    /** @property {Integer} `0` or `1` to select between the two color sets. */
    ColorSet := 0

    /** @property {Color} TextFGColor Gets or Sets the currently selected color set's Text Foreground Color */
    TextFGColor
    {
        get => this.TextFGColors[this.ColorSet + 1]
        set => this.TextFGColors[this.ColorSet + 1] := value
    }

    /** @property {Color} TextBGColor Gets or Sets the currently selected color set's Text Background Color */
    TextBGColor
    {
        get => this.TextBGColors[this.ColorSet + 1]
        set => this.TextBGColors[this.ColorSet + 1] := value
    }

    /** @property {Color} BorderColor Gets or Sets the currently selected color set's Border Color */
    BorderColor
    {
        get => this.BorderColors[this.ColorSet + 1]
        set => this.BorderColors[this.ColorSet + 1] := value
    }

    /** @property {Color} CrosshairColor Gets or Sets the currently selected color set's Crosshair Color */
    CrosshairColor
    {
        get => this.CrosshairColors[this.ColorSet + 1]
        set => this.CrosshairColors[this.ColorSet + 1] := value
    }

    /** @property {Color} GridColor Gets or Sets the currently selected color set's Grid Color */
    GridColor
    {
        get => this.GridColors[this.ColorSet + 1]
        set => this.GridColors[this.ColorSet + 1] := value
    }

    /** @property {Color} HighlightColor Gets or Sets the currently selected color set's Highlight Color */
    HighlightColor
    {
        get => this.HighlightColors[this.ColorSet + 1]
        set => this.HighlightColors[this.ColorSet + 1] := value
    }

    /**
     * Creates a new instance of `ColorPicker`
     * @param {boolean} [clip=False] Whether to copy the selected color to clipboard.
     * @param {number} [hwnd=0] The handle of the target window to confine the picker to.
     * @param {Function} [callback=0] A callback function to be called with the selected color.
     */
    __New(clip := False, hwnd := 0, callback := 0)
    {
        if (hwnd != 0) and (WinExist(hwnd))
            this.TargetHWND := hwnd

        if (callback != 0) and (callback is func)
            this.OnUpdate := callback

        this.Clip := clip
    }

    /**
     * Runs the `ColorPicker` with default settings.
     * @param {boolean} [clip=True] Whether to copy the selected color to clipboard.
     * @param {number} [hwnd=0] The handle of the target window to confine the picker to.
     * @param {Function} [callback=0] A callback function to be called with the selected color.
     * @returns {Boolean | Object} The `Color` object if a color was chosen, `False` otherwise.
     */
    static Run(clip := True, hwnd := 0, callback := 0)
    {
        picker := ColorPicker(clip, hwnd, callback)
        return picker.Start()
    }

    /**
     * Starts the current instance of `ColorPicker`.
     * @returns {Boolean | Color} The `Color` object if a color was chosen, `False` otherwise.
     */
    Start()
    {
        if this.OnStart is Func
            this.OnStart.Call()

        startColor := this.Color

        try dpiContext := DllCall("SetThreadDpiAwarenessContext", "ptr", -3, "ptr")

        GetDpiScale(guiHwnd)
        {
            dpi := DllCall("User32.dll\GetDpiForWindow", "Ptr", guiHwnd, "UInt")
            return dpi / 96
        }

        BlockLButton(*)
        {
            KeyWait("LButton", "D")
            return
        }

        CaptureAndPreview(*)
        {
            if not frozen
            {
                ; Get cursor position
                CoordMode "Mouse", "Screen"
                CoordMode "Pixel", "Screen"
                MouseGetPos(&cursorX, &cursorY)
                dpiScale := GetDpiScale(previewGui.Hwnd)

                ; Calculate capture region
                halfSize := (captureSize - 1) // 2
                left     := cursorX - halfSize
                top      := cursorY - halfSize
                width    := captureSize
                height   := captureSize

                ; Capture screen region
                hDC := DllCall("GetDC", "Ptr", 0, "Ptr")
                hMemDC := DllCall("CreateCompatibleDC", "Ptr", hDC, "Ptr")
                hBitmap := DllCall("CreateCompatibleBitmap", "Ptr", hDC, "Int", width, "Int", height, "Ptr")
                DllCall("SelectObject", "Ptr", hMemDC, "Ptr", hBitmap)
                DllCall("BitBlt", "Ptr", hMemDC, "Int", 0, "Int", 0, "Int", width, "Int", height, "Ptr", hDC, "Int", left, "Int", top, "UInt", 0x00CC0020)

                ; Get color of central pixel
                centralX     := width // 2
                centralY     := height // 2
                centralColor := DllCall("GetPixel", "Ptr", hMemDC, "Int", centralX, "Int", centralY, "UInt")
                hexColor     := Format("{:06X}", centralColor & 0xFFFFFF)
                this.Color   := Color(Integer(Format("0xFF{1}{2}{3}", SubStr(hexColor, 5, 2), SubStr(hexColor, 3, 2), SubStr(hexColor, 1, 2))))
                hexColor     := this.Color.ToString("Hex")

                ; Calculate preview size
                scaledZoomFactor := Round(zoomFactor * dpiScale)
                previewWidth := captureSize * scaledZoomFactor
                previewHeight := captureSize * scaledZoomFactor

                ; Prepare to draw text
                scaledFontSize := Round(this.FontSize * dpiScale)
                LOGFONT := Buffer(92, 0)
                NumPut("Int", scaledFontSize * 4, LOGFONT, 0)
                StrPut(this.FontName, LOGFONT.Ptr + 28, 32, "UTF-16")
                hFont := DllCall("CreateFontIndirect", "Ptr", LOGFONT, "Ptr")
                size := Buffer(8)
                DllCall("GetTextExtentPoint32", "Ptr", hDC, "Str", "Ay", "Int", 2, "Ptr", size)
                textHeight := Round((NumGet(size, 4, "Int") + this.TextPadding) * dpiScale)

                ; Conclude size calculations
                totalHeight := (previewHeight + textHeight)

                ; Create high-resolution memory DC
                hHighResDC := DllCall("CreateCompatibleDC", "Ptr", hDC, "Ptr")
                hHighResBitmap := DllCall("CreateCompatibleBitmap", "Ptr", hDC, "Int", previewWidth * 4, "Int", totalHeight * 4, "Ptr")
                DllCall("SelectObject", "Ptr", hHighResDC, "Ptr", hHighResBitmap)
                DllCall("SetStretchBltMode", "Ptr", hHighResDC, "Int", 4)
                DllCall("StretchBlt", "Ptr", hHighResDC, "Int", 0, "Int", 0, "Int", previewWidth * 4, "Int", previewHeight * 4, "Ptr", hMemDC, "Int", 0, "Int", 0, "Int", width, "Int", height, "UInt", 0x00CC0020)

                ; Draw background rectangle
                hBrush := DllCall("CreateSolidBrush", "UInt", this.TextBGColor.ToInt(1), "Ptr")
                rect := Buffer(16, 0)
                NumPut("Int", 0, rect, 0)
                NumPut("Int", previewHeight * 4, rect, 4)
                NumPut("Int", previewWidth * 4, rect, 8)
                NumPut("Int", totalHeight * 4, rect, 12)
                DllCall("FillRect", "Ptr", hHighResDC, "Ptr", rect, "Ptr", hBrush)
                DllCall("DeleteObject", "Ptr", hBrush)

                ; Render text at high resolution
                DllCall("SelectObject", "Ptr", hHighResDC, "Ptr", hFont)
                DllCall("SetTextColor", "Ptr", hHighResDC, "UInt", this.TextFGColor.ToInt(1))
                DllCall("SetBkColor", "Ptr", hHighResDC, "UInt", this.TextBGColor.ToInt(1))
                textWidth := DllCall("GetTextExtentPoint32", "Ptr", hHighResDC, "Str", hexColor, "Int", StrLen(hexColor), "Ptr", rect)
                textX := (previewWidth * 4 - NumGet(rect, 0, "Int")) // 2
                textY := previewHeight * 4 + (textHeight * 4 - scaledFontSize * 4) // 2
                DllCall("TextOut", "Ptr", hHighResDC, "Int", textX, "Int", textY, "Str", hexColor, "Int", StrLen(hexColor))

                ; Calculate the offset based on captureSize
                offset := (Mod(captureSize, 2) == 0) ? Round(zoomFactor * 2) : 0
                if (this.ViewMode == "crosshair")
                {
                    centerX := Round(previewWidth * 2) + offset
                    centerY := Round(previewHeight * 2) + offset
                    halfZoom := Round(zoomFactor * 2)
                    hCrosshairPen := DllCall("CreatePen", "Int", 0, "Int", Round(this.CrosshairWidth * dpiScale) * 4, "UInt", this.CrosshairColor.ToInt() & 0xFFFFFF, "Ptr")
                    DllCall("SelectObject", "Ptr", hHighResDC, "Ptr", hCrosshairPen)
                    DllCall("MoveToEx", "Ptr", hHighResDC, "Int", centerX, "Int", 0, "Ptr", 0)
                    DllCall("LineTo", "Ptr", hHighResDC, "Int", centerX, "Int", previewHeight * 4)
                    DllCall("MoveToEx", "Ptr", hHighResDC, "Int", 0, "Int", centerY, "Ptr", 0)
                    DllCall("LineTo", "Ptr", hHighResDC, "Int", previewWidth * 4, "Int", centerY)
                    if this.HighlightCenter
                    {
                        hInnerCrosshairPen := DllCall("CreatePen", "Int", 0, "Int", Round(this.CrosshairWidth * dpiScale) * 4, "UInt", this.HighlightColor.ToInt() & 0xFFFFFF, "Ptr")
                        DllCall("SelectObject", "Ptr", hHighResDC, "Ptr", hInnerCrosshairPen)
                        DllCall("MoveToEx", "Ptr", hHighResDC, "Int", centerX, "Int", centerY - halfZoom, "Ptr", 0)
                        DllCall("LineTo", "Ptr", hHighResDC, "Int", centerX, "Int", centerY + halfZoom)
                        DllCall("MoveToEx", "Ptr", hHighResDC, "Int", centerX - halfZoom, "Int", centerY, "Ptr", 0)
                        DllCall("LineTo", "Ptr", hHighResDC, "Int", centerX + halfZoom, "Int", centerY)
                        DllCall("DeleteObject", "Ptr", hInnerCrosshairPen)
                    }
                    DllCall("DeleteObject", "Ptr", hCrosshairPen)
                }
                else if (this.ViewMode == "grid")
                {
                    ; Calculate the center square
                    if Mod(captureSize, 2) == 0
                        centerIndex := captureSize // 2 + 1
                    else
                        centerIndex := captureSize // 2 + (captureSize & 1)

                    ; Draw grid
                    hGridPen := DllCall("CreatePen", "Int", 0, "Int", Round(this.GridWidth * dpiScale) * 4, "UInt", this.GridColor.ToInt() & 0xFFFFFF, "Ptr")
                    DllCall("SelectObject", "Ptr", hHighResDC, "Ptr", hGridPen)

                    Loop captureSize + 1
                    {
                        x := (A_Index - 1) * scaledZoomFactor * 4
                        DllCall("MoveToEx", "Ptr", hHighResDC, "Int", x, "Int", 0, "Ptr", 0)
                        DllCall("LineTo", "Ptr", hHighResDC, "Int", x, "Int", previewHeight * 4)
                        DllCall("MoveToEx", "Ptr", hHighResDC, "Int", 0, "Int", x, "Ptr", 0)
                        DllCall("LineTo", "Ptr", hHighResDC, "Int", previewWidth * 4, "Int", x)
                    }

                    if this.HighlightCenter
                    {
                        ; Highlight the center or lower-right of center square
                        hHighlightPen := DllCall("CreatePen", "Int", 0, "Int", Round(this.GridWidth * dpiScale) * 4, "UInt", this.HighlightColor.ToInt() & 0xFFFFFF, "Ptr")
                        DllCall("SelectObject", "Ptr", hHighResDC, "Ptr", hHighlightPen)
                        DllCall("MoveToEx", "Ptr", hHighResDC, "Int", (centerIndex - 1) * scaledZoomFactor * 4, "Int", (centerIndex - 1) * scaledZoomFactor * 4, "Ptr", 0)
                        DllCall("LineTo", "Ptr", hHighResDC, "Int", centerIndex * scaledZoomFactor * 4, "Int", (centerIndex - 1) * scaledZoomFactor * 4)
                        DllCall("LineTo", "Ptr", hHighResDC, "Int", centerIndex * scaledZoomFactor * 4, "Int", centerIndex * scaledZoomFactor * 4)
                        DllCall("LineTo", "Ptr", hHighResDC, "Int", (centerIndex - 1) * scaledZoomFactor * 4, "Int", centerIndex * scaledZoomFactor * 4)
                        DllCall("LineTo", "Ptr", hHighResDC, "Int", (centerIndex - 1) * scaledZoomFactor * 4, "Int", (centerIndex - 1) * scaledZoomFactor * 4)
                        DllCall("DeleteObject", "Ptr", hHighlightPen)
                    }

                    DllCall("DeleteObject", "Ptr", hGridPen)
                }
                else if this.HighlightCenter
                {
                    ; Draw a dot in the center
                    centerX := Round(previewWidth * 2) + offset
                    centerY := Round(previewHeight * 2) + offset
                    dotSize := Round(4 * dpiScale) * this.CenterDotRadius
                    hDotBrush := DllCall("CreateSolidBrush", "UInt", this.HighlightColor.ToInt() & 0xFFFFFF, "Ptr")
                    DllCall("SelectObject", "Ptr", hHighResDC, "Ptr", hDotBrush)
                    DllCall("Ellipse", "Ptr", hHighResDC, "Int", centerX - Round(dotSize * dpiScale), "Int", centerY - Round(dotSize * dpiScale), "Int", centerX + Round(dotSize * dpiScale), "Int", centerY + Round(dotSize * dpiScale))
                    DllCall("DeleteObject", "Ptr", hDotBrush)
                }

                ; Draw border
                hBorderPen := DllCall("CreatePen", "Int", 0, "Int", this.BorderWidth * 4, "UInt", this.BorderColor.ToInt() & 0xFFFFFF, "Ptr")
                DllCall("SelectObject", "Ptr", hHighResDC, "Ptr", hBorderPen)
                DllCall("MoveToEx", "Ptr", hHighResDC, "Int", 0, "Int", 0, "Ptr", 0)
                DllCall("LineTo", "Ptr", hHighResDC, "Int", previewWidth * 4, "Int", 0)
                DllCall("LineTo", "Ptr", hHighResDC, "Int", previewWidth * 4, "Int", totalHeight * 4)
                DllCall("LineTo", "Ptr", hHighResDC, "Int", 0, "Int", totalHeight * 4)
                DllCall("LineTo", "Ptr", hHighResDC, "Int", 0, "Int", 0)

                ; Draw separator line
                DllCall("MoveToEx", "Ptr", hHighResDC, "Int", 0, "Int", previewHeight * 4, "Ptr", 0)
                DllCall("LineTo", "Ptr", hHighResDC, "Int", previewWidth * 4, "Int", previewHeight * 4)
                DllCall("DeleteObject", "Ptr", hBorderPen)

                ; Create preview DC and scale down from high-res DC
                hPreviewDC := DllCall("CreateCompatibleDC", "Ptr", hDC, "Ptr")
                hPreviewBitmap := DllCall("CreateCompatibleBitmap", "Ptr", hDC, "Int", previewWidth, "Int", totalHeight, "Ptr")
                DllCall("SelectObject", "Ptr", hPreviewDC, "Ptr", hPreviewBitmap)
                DllCall("SetStretchBltMode", "Ptr", hPreviewDC, "Int", 4)
                DllCall("StretchBlt", "Ptr", hPreviewDC, "Int", 0, "Int", 0, "Int", previewWidth, "Int", totalHeight, "Ptr", hHighResDC, "Int", 0, "Int", 0, "Int", previewWidth * 4, "Int", totalHeight * 4, "UInt", 0x00CC0020)

                ; Update preview GUI
                hPreviewHWND := WinExist("A")
                DllCall("UpdateLayeredWindow", "Ptr", hPreviewHWND, "Ptr", 0, "Ptr", 0, "Int64*", previewWidth | (totalHeight << 32), "Ptr", hPreviewDC, "Int64*", 0, "UInt", 0, "UInt*", 0xFF << 16, "UInt", 2)

                ; Clean up
                DllCall("DeleteObject", "Ptr", hFont)
                DllCall("DeleteDC", "Ptr", hPreviewDC)
                DllCall("DeleteObject", "Ptr", hPreviewBitmap)
                DllCall("DeleteDC", "Ptr", hHighResDC)
                DllCall("DeleteObject", "Ptr", hHighResBitmap)
                DllCall("DeleteDC", "Ptr", hMemDC)
                DllCall("DeleteObject", "Ptr", hBitmap)
                DllCall("ReleaseDC", "Ptr", 0, "Ptr", hDC)

                if this.OnUpdate is Func
                    this.OnUpdate.Call(this.Color)
            }
        }

        CoordMode "Mouse", "Screen"
        CoordMode "Pixel", "Screen"
        Suspend(True)
        Hotkey("*LButton", BlockLButton, "On S")

        frozen := False, outType := "", colorSet := 0, textHeight := 0
        zoomFactor     := this.DefaultZoomFactor
        captureSize    := this.DefaultCaptureSize

        previewGui := Gui("+AlwaysOnTop -Caption +ToolWindow -DPIScale")
        if not DllCall("User32\SetWindowDisplayAffinity", "Ptr", previewGui.hWnd, "Int", 0x00000011, "Int") ; WDA_EXCLUDEFROMCAPTURE
            OutputDebug("Failed to set affinity:" A_LastError)
        previewGui.Opt(" +E0x80000")
        previewGui.Show()
        SetTimer(CaptureAndPreview, this.UpdateInterval)

        ; Set the cursor to crosshair
        hCross := DllCall("LoadCursor", "Ptr", 0, "Ptr", 32515)
        for cursorId in [32512, 32513, 32514, 32515, 32516, 32631, 32640, 32641, 32642, 32643, 32644, 32645, 32646, 32648, 32649, 32650, 32651]
            DllCall("SetSystemCursor", "Ptr", DllCall("CopyImage", "Ptr", hCross, "UInt", 2, "Int", 0, "Int", 0, "UInt", 0), "UInt", cursorId)

        ; If a Valid HWND was passed as an argument, confine the cursor to that window
        if (this.TargetHWND != 0) and WinExist("ahk_id " this.TargetHWND)
        {
            windowRect := Buffer(16)
            DllCall("GetWindowRect", "Ptr", this.TargetHWND, "Ptr", windowRect)
            confineLeft   := NumGet(windowRect, 0, "Int")
            confineTop    := NumGet(windowRect, 4, "Int")
            confineRight  := NumGet(windowRect, 8, "Int")
            confineBottom := NumGet(windowRect, 12, "Int")

            DllCall("ClipCursor", "Ptr", windowRect)
        }

        ; Main loop
        while (True)
        {
            MouseGetPos(&mouseX, &mouseY)

            if (this.TargetHWND != 0) and WinExist("ahk_id " this.TargetHWND)
            {
                mouseX := Max(confineLeft, Min(mouseX, confineRight))
                mouseY := Max(confineTop, Min(mouseY, confineBottom))
            }

            if this.Anchored
            {
                previewGui.Move(this.AnchoredX, this.AnchoredY)
            }
            else
            {
                previewWidth  := captureSize * zoomFactor + this.BorderWidth * 2
                previewHeight := captureSize * zoomFactor + this.BorderWidth * 2 + textHeight

                newX := mouseX + this.PreviewXOffset
                newY := mouseY + this.PreviewYOffset

                monitorCount := MonitorGetCount()
                dpiScale := GetDpiScale(previewGui.Hwnd)
                Loop monitorCount
                {
                    MonitorGet(A_Index, &left, &top, &right, &bottom)

                    if (mouseX >= left && mouseX < right && mouseY >= top && mouseY < bottom)
                    {
                        ; Apply DPI scaling to preview dimensions
                        scaledPreviewWidth := previewWidth * dpiScale
                        scaledPreviewHeight := previewHeight * dpiScale

                        ; Adjust for right edge
                        if (newX + scaledPreviewWidth > right)
                            newX := mouseX - this.PreviewXOffset * dpiScale - scaledPreviewWidth

                        ; Adjust for bottom edge, including taskbar
                        if (newY + scaledPreviewHeight > bottom)
                            newY := mouseY - this.PreviewYOffset * dpiScale - scaledPreviewHeight

                        ; Ensure the preview stays within the monitor bounds
                        newX := Max(left, Min(newX, right - scaledPreviewWidth))
                        newY := Max(top, Min(newY, bottom - scaledPreviewHeight))

                        break
                    }
                }

                previewGui.Move(newX, newY)
            }

            ; "LButton", "Enter", or "NumpadEnter" Captures HEX, Shift in combination with them captures RGB
            if GetKeyState("LButton", "P") or GetKeyState("Enter", "P") or GetKeyState("NumpadEnter", "P") or GetKeyState("Space", "P")
            {
                if GetKeyState("Shift", "P")
                    outType := "RGB"
                else
                    outType := "HEX"
                break
            }

            ; "Escape" or "Q" exits
            if GetKeyState("Escape", "P") or GetKeyState("q", "P")
            {
                outType := "Exit"
                break
            }

            ; "C" cycles between color schemes
            if GetKeyState("c", "P")
            {
                this.ColorSet := !this.ColorSet

                KeyWait("c")
            }

            ; "A" toggles anchoring
            if GetKeyState("a", "P") or GetKeyState("NumpadDot", "P")
            {
                if this.CanUnanchor
                {
                    this.Anchored := !this.Anchored
                    if this.Anchored
                    {
                        this.AnchoredX := mouseX + this.PreviewXOffset
                        this.AnchoredY := mouseY + this.PreviewYOffset
                    }
                }

                if !KeyWait("a") or !KeyWait("NumpadDot")
                    continue
            }

            ; "M" cycles between view modes (grid, crosshair, none)
            if GetKeyState("m", "P")
            {
                this.ViewModes := [ "grid", "crosshair", "none" ]
                index := 0

                for mode in this.ViewModes
                    if (mode == this.ViewMode)
                        index := A_Index

                if index == 0
                {
                    this.ViewMode := "none"
                    index := 3
                }

                this.ViewMode := this.ViewModes[Mod(index, this.ViewModes.Length) + 1]
                KeyWait("m")
            }

            ; "Left" or "Numpad4" moves cursor left one pixel
            if GetKeyState("Left", "P") or GetKeyState("Numpad4", "P")
            {
                if GetKeyState("Shift", "P")
                    MouseMove(-this.LargeJumpAmount, 0, 0, "R")
                else
                    MouseMove(-1, 0, 0, "R")

                if !KeyWait("Left", "T0.10") or !KeyWait("Numpad4", "T0.10")
                    continue
            }

            ; "Right" or "Numpad6" moves cursor right one pixel
            if GetKeyState("Right", "P") or GetKeyState("Numpad6", "P")
            {
                if GetKeyState("Shift", "P")
                    MouseMove(this.LargeJumpAmount, 0, 0, "R")
                else
                    MouseMove(1, 0, 0, "R")

                if !KeyWait("Right", "T0.10") or !KeyWait("Numpad6", "T0.10")
                    continue
            }

            ; "Up" or "Numpad8" moves cursor up one pixel
            if GetKeyState("Up", "P") or GetKeyState("Numpad8", "P")
            {
                if GetKeyState("Shift", "P")
                    MouseMove(0, -this.LargeJumpAmount, 0, "R")
                else
                    MouseMove(0, -1, 0, "R")

                if !KeyWait("Up", "T0.10") or !KeyWait("Numpad8", "T0.10")
                    continue
            }

            ; "Down" or "Numpad2" moves cursor down one pixel
            if GetKeyState("Down", "P") or GetKeyState("Numpad2", "P")
            {
                if GetKeyState("Shift", "P")
                    MouseMove(0, this.LargeJumpAmount, 0, "R")
                else
                    MouseMove(0, 1, 0, "R")

                if !KeyWait("Down", "T0.10") or !KeyWait("Numpad2", "T0.10")
                    continue
            }

            ; "H" toggles highlighting the center pixel
            if GetKeyState("h", "P")
            {
                this.HighlightCenter := !this.HighlightCenter
                KeyWait("h")
            }

            ; "-" or "NumpadSub" decreases capture size
            if GetKeyState("-", "P") or GetKeyState("NumpadSub", "P")
            {
                captureSize := Max(1, --captureSize)

                if !KeyWait("-") or !KeyWait("NumpadSub")
                    continue
            }

            ; "=" or "NumpadAdd" increases capture size
            if GetKeyState("=", "P") or GetKeyState("NumpadAdd", "P")
            {
                captureSize := ++captureSize

                if !KeyWait("=") or !KeyWait("NumpadAdd")
                    continue
            }

            ; "[" or "NumpadDiv" decreases zoom factor
            if GetKeyState("[", "P") or GetKeyState("NumpadDiv", "P")
            {
                zoomFactor := Max(1, --zoomFactor)

                if !KeyWait("[") or !KeyWait("NumpadDiv")
                    continue
            }

            ; "]" or "NumpadMult" increases zoom factor
            if GetKeyState("]", "P") or GetKeyState("NumpadMult", "P")
            {
                zoomFactor := ++zoomFactor
                if !KeyWait("]") or !KeyWait("NumpadMult")
                    continue
            }

            ; "0" or "Numpad0" resets zoom and capture size
            if GetKeyState("0", "P") or GetKeyState("Numpad0", "P")
            {
                zoomFactor := this.DefaultZoomFactor
                captureSize := this.DefaultCaptureSize

                if !KeyWait("0") or !KeyWait("Numpad0")
                    continue
            }

            ; "F" or "Numpad5" toggles freezing the preview update cycle
            if GetKeyState("f", "P") or GetKeyState("Numpad5", "P")
            {
                frozen := !frozen

                if !KeyWait("f") or !KeyWait("Numpad5")
                    continue
            }

            Sleep(10)
        }

        this.Color.RGBFormat := this.RGBFormat
        this.Color.HexFormat := this.HexFormat

        if (this.Clip == True) and ((outType == "HEX") or (outType == "RGB"))
            A_Clipboard := (outType == "HEX" ? this.Color.ToString("Hex") : this.Color.Full)

        ; Cleanup
        SetTimer(CaptureAndPreview, 0)  ; Turn off the timer
        DllCall("SystemParametersInfo", "UInt", 0x57, "UInt", 0, "Ptr", 0, "UInt", 0)  ; Reset cursor
        DllCall("DestroyCursor", "Ptr", hCross)
        Sleep(50)
        Hotkey("*LButton", "Off")
        Suspend(False)

        previewGui.Destroy()
        try DllCall("SetThreadDpiAwarenessContext", "ptr", dpiContext, "ptr")

        if (this.TargetHWND != 0) and WinExist("ahk_id " this.TargetHWND)
            DllCall("ClipCursor", "Ptr", 0)

        this.Color := (outType == "Exit" ? startColor : this.Color)

        if this.OnUpdate is Func
            this.OnUpdate.Call(this.Color)

        if this.OnExit is Func
            this.OnExit.Call(this.Color)

        return (outType == "Exit" ? False : this.Color)
    }
}