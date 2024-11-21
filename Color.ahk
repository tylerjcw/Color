#Requires AutoHotkey v2.0

/**
 * Automatically loads and unloads `Color.dll` when the script is initialized.
 */
class __ColorLib
{
    static hModule := 0

    static __New()
    {
        dll_path := StrReplace(A_LineFile, ".ahk", ".dll")
        if FileExist(dll_path) and not __ColorLib.hModule
            if not (__ColorLib.hModule := DllCall("LoadLibrary", "Str", dll_path, "Ptr"))
                throw Error("Failed to load Color.dll, please verify it is in the same folder as Color.ahk")
    }

    static __Delete()
    {
        if __ColorLib.hModule
            DllCall("FreeLibrary", "Ptr", __ColorLib.hModule)
        else
            throw Error("Color.dll failed to unload successfully.")
    }
}

/**
 * Represents a color. Stored in 32-Bit ARGB format.
 */
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
            case 0:
                this.Ptr := DllCall("Color\CreateColorObject", "Ptr")
            case 1:
                if args[1] is Number
                    this.Ptr := DllCall("Color\CreateColorFromInt", "UInt", args[1], "Ptr")
                else if args[1] is String
                {
                    if RegExMatch(args[1], "^(0x|#)?[0-9A-Fa-f]{6,8}$")
                        this.Ptr := DllCall("Color\CreateColorFromHexString", "AStr", args[1], "Ptr")
                    else
                        this.Ptr := DllCall("Color\CreateColorFromName", "AStr", args[1], "Ptr")
                }
            case 3:
                this.Ptr := DllCall("Color\CreateColorFromRGBA", "UChar", args[1], "UChar", args[2], "UChar", args[3], "UChar", 255, "Ptr")
            case 4:
                this.Ptr := DllCall("Color\CreateColorFromRGBA", "UChar", args[1], "UChar", args[2], "UChar", args[3], "UChar", args[4], "Ptr")
            default:
                throw Error("Color Constructor: Invalid Color arguments")
        }

        this.TypeString := "RGB"
        this.FormatString := "rgba({R}, {G}, {B}, {A})"
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
        set => (DllCall("Color\SetColorAlpha", "Ptr", this.Ptr, "Int", value), this)
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
     * Gets or sets the ARGB value of the color (0xAARRGGBB)
     * @type {Number}
     */
    ARGB
    {
        get => DllCall("Color\GetColorARGB", "Ptr", this.Ptr, "Int")
        set => DllCall("Color\SetColorARGB", "Ptr", this.Ptr, "Int", value)
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
        set => DllCall("Color\SetColorFormatString", "Ptr", this.Ptr, "AStr", value)
    }

    /**
     * Gets or sets the format string used for color representation.
     *
     * Default is `RGB`
     * @type {string}
     */
    TypeString
    {
        get => StrGet(DllCall("Color\GetColorTypeString", "Ptr", this.Ptr, "Ptr"), "UTF-8")
        set => DllCall("Color\SetColorTypeString", "Ptr", this.Ptr, "AStr", value)
    }

    /**
     * Gets the Full RGB string representation of the color.
     * ___
     * Example: `rgba(255, 34, 96, 128)`
     * ___
     * The type is controlled by `this.TypeString`
     * The format is controlled by `this.FormatString`
     * @type {Number}
     */
    Full  => this.ToString(this.TypeString, this.FormatString)

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
     * ---
     * @returns {Integer} - The requested value.
     */
    ToInt(format  := 0) => DllCall("Color\ColorToInt", "Ptr", this.Ptr, "Int", format, "UInt")

    /**
     * Converts the color to a string representation.
     * @param {String} type - The color space type (e.g.: `RGB`, `Hex`, `HSL`, `HWB`, `XYZ`, `Lab`, `...`)
     * @param {String} [format] - The format string to use. If not provided, uses the default format for the specified type.
     * ___
     * @returns {String} - The converted String representation.
     * ___
     * @example <caption>Converting a Color to a YPbPr String, with each channel rounded to 3 decimal places. Note that Formatters are case-insensitive.</caption>
     * col.ToString("YPbPr", "Y: {y:3}`nPb: {Pb:3}`nPr: {pR:3}")
     */
    ToString(type := "", format := "")
    {
        if not type
            type := this.TypeString

        if not format
        {
            switch StrLower(type)
            {
                case "rgb":         format := Color.DefaultFormatString.RGB
                case "linearsrgb":  format := Color.DefaultFormatString.LinearSRGB
                case "hex":         format := Color.DefaultFormatString.Hex
                case "hsl":         format := Color.DefaultFormatString.HSL
                case "hsi":         format := Color.DefaultFormatString.HSI
                case "hwb":         format := Color.DefaultFormatString.HWB
                case "hsp":         format := Color.DefaultFormatString.HSP
                case "hcy":         format := Color.DefaultFormatString.HCY
                case "hcg":         format := Color.DefaultFormatString.HCG
                case "tsl":         format := Color.DefaultFormatString.TSL
                case "xyz_d50":     format := Color.DefaultFormatString.XYZ_D50
                case "xyz_d65":     format := Color.DefaultFormatString.XYZ_D65
                case "ucs":         format := Color.DefaultFormatString.UCS
                case "uvw":         format := Color.DefaultFormatString.UVW
                case "xyy":         format := Color.DefaultFormatString.XYY
                case "lab":         format := Color.DefaultFormatString.Lab
                case "ncol":        format := Color.DefaultFormatString.NCol
                case "cmy":         format := Color.DefaultFormatString.CMY
                case "cmyk":        format := Color.DefaultFormatString.CMYK
                case "hsv":         format := Color.DefaultFormatString.HSV
                case "ypbpr":       format := Color.DefaultFormatString.YPbPr
                case "ycbcr":       format := Color.DefaultFormatString.YCbCr
                case "ycgco":       format := Color.DefaultFormatString.YCgCo
                case "yccbccrc":    format := Color.DefaultFormatString.YcCbcCrc
                case "ydbdr":       format := Color.DefaultFormatString.YDbDr
                case "yuv":         format := Color.DefaultFormatString.YUV
                case "yes":         format := Color.DefaultFormatString.YES
                case "jpeg":        format := Color.DefaultFormatString.JPEG
                case "yiq":         format := Color.DefaultFormatString.YIQ
                case "lchab":       format := Color.DefaultFormatString.LCHab
                case "lchuv":       format := Color.DefaultFormatString.LCHuv
                case "adobergb":    format := Color.DefaultFormatString.AdobeRGB
                case "prophotorgb": format := Color.DefaultFormatString.ProPhotoRGB
                case "luv":         format := Color.DefaultFormatString.Luv
                case "rec2020":     format := Color.DefaultFormatString.Rec2020
                case "displayp3":   format := Color.DefaultFormatString.DisplayP3
                case "oklch":       format := Color.DefaultFormatString.OKLCH
                case "oklab":       format := Color.DefaultFormatString.OKLab
                case "acescg":      format := Color.DefaultFormatString.ACEScg
                default: throw Error("Unknown type")
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
     * Converts the Color to a COLORREF and returns a pointer to it.
     * @returns {Integer}
     */
    ToCOLORREF() => DllCall("Color\ColorToCOLORREF", "Ptr", this.Ptr, "UInt")

    /**
     * Creates a Color instance from a COLORREF
     * @param colorref - A pointer to a COLORREF instance.
     * @returns {Color}
     */
    static FromCOLORREF(colorref) => Color.FromPtr(DllCall("Color\ColorFromCOLORREF", "UInt", colorref, "Ptr"))

    /**
     * Converts the Color to a GDI+ Color and returns a pointer to it.
     * @returns {Integer}
     */
    ToGdipColor() => DllCall("Color\ColorToGdipColor", "Ptr", this.Ptr, "UInt64")

    /**
     * Creates a Color instance from a GDI+ Color
     * @param colorref - A pointer to a GDI+ Color instance.
     * @returns {Color}
     */
    static FromGdipColor(gdipColor) => Color.FromPtr(DllCall("Color\ColorFromGdipColor", "UInt64", gdipColor, "Ptr"))

    /**
     * Compares this color with another color for equality.
     * @param {Color} other - The color to compare with.
     * @returns {boolean}
     */
    IsEqual(other) => (this.ToInt() == other.ToInt())

    /**
     * Creates a random color.
     * @returns {Color}
     */
    static Random(alphaRand := true) => Color.FromPtr(DllCall("Color\CreateRandomColor", "Int", alphaRand, "Ptr"))

    /** `AliceBlue = Color(0xFFF0F8FF)` */
    static AliceBlue => Color.FromPtr(DllCall("Color\CreateAliceBlueColor", "Ptr"))

    /** `AntiqueWhite = Color(0xFFFAEBD7)` */
    static AntiqueWhite => Color.FromPtr(DllCall("Color\CreateAntiqueWhiteColor", "Ptr"))

    /** `Aqua = Color(0xFF00FFFF)` */
    static Aqua => Color.FromPtr(DllCall("Color\CreateAquaColor", "Ptr"))

    /** `Aquamarine = Color(0xFF7FFFD4)` */
    static Aquamarine => Color.FromPtr(DllCall("Color\CreateAquamarineColor", "Ptr"))

    /** `Azure = Color(0xFFF0FFFF)` */
    static Azure => Color.FromPtr(DllCall("Color\CreateAzureColor", "Ptr"))

    /** `Beige = Color(0xFFF5F5DC)` */
    static Beige => Color.FromPtr(DllCall("Color\CreateBeigeColor", "Ptr"))

    /** `Bisque = Color(0xFFFFE4C4)` */
    static Bisque => Color.FromPtr(DllCall("Color\CreateBisqueColor", "Ptr"))

    /** `Black = Color(0xFF000000)` */
    static Black => Color.FromPtr(DllCall("Color\CreateBlackColor", "Ptr"))

    /** `BlackTransparent = Color(0x00000000)` */
    static BlackTransparent => Color.FromPtr(DllCall("Color\CreateBlackTransparentColor", "Ptr"))

    /** `BlanchedAlmond = Color(0xFFFFEBCD)` */
    static BlanchedAlmond => Color.FromPtr(DllCall("Color\CreateBlanchedAlmondColor", "Ptr"))

    /** `Blue = Color(0xFF0000FF)` */
    static Blue => Color.FromPtr(DllCall("Color\CreateBlueColor", "Ptr"))

    /** `BlueViolet = Color(0xFF8A2BE2)` */
    static BlueViolet => Color.FromPtr(DllCall("Color\CreateBlueVioletColor", "Ptr"))

    /** `Brown = Color(0xFFA52A2A)` */
    static Brown => Color.FromPtr(DllCall("Color\CreateBrownColor", "Ptr"))

    /** `BurlyWood = Color(0xFFDEB887)` */
    static BurlyWood => Color.FromPtr(DllCall("Color\CreateBurlyWoodColor", "Ptr"))

    /** `CadetBlue = Color(0xFF5F9EA0)` */
    static CadetBlue => Color.FromPtr(DllCall("Color\CreateCadetBlueColor", "Ptr"))

    /** `Chartreuse = Color(0xFF7FFF00)` */
    static Chartreuse => Color.FromPtr(DllCall("Color\CreateChartreuseColor", "Ptr"))

    /** `Chocolate = Color(0xFFD2691E)` */
    static Chocolate => Color.FromPtr(DllCall("Color\CreateChocolateColor", "Ptr"))

    /** `Coral = Color(0xFFFF7F50)` */
    static Coral => Color.FromPtr(DllCall("Color\CreateCoralColor", "Ptr"))

    /** `CornflowerBlue = Color(0xFF6495ED)` */
    static CornflowerBlue => Color.FromPtr(DllCall("Color\CreateCornflowerBlueColor", "Ptr"))

    /** `Cornsilk = Color(0xFFFFF8DC)` */
    static Cornsilk => Color.FromPtr(DllCall("Color\CreateCornsilkColor", "Ptr"))

    /** `Crimson = Color(0xFFDC143C)` */
    static Crimson => Color.FromPtr(DllCall("Color\CreateCrimsonColor", "Ptr"))

    /** `Cyan = Color(0xFF00FFFF)` */
    static Cyan => Color.FromPtr(DllCall("Color\CreateCyanColor", "Ptr"))

    /** `DarkBlue = Color(0xFF00008B)` */
    static DarkBlue => Color.FromPtr(DllCall("Color\CreateDarkBlueColor", "Ptr"))

    /** `DarkCyan = Color(0xFF008B8B)` */
    static DarkCyan => Color.FromPtr(DllCall("Color\CreateDarkCyanColor", "Ptr"))

    /** `DarkGoldenRod = Color(0xFFB8860B)` */
    static DarkGoldenRod => Color.FromPtr(DllCall("Color\CreateDarkGoldenRodColor", "Ptr"))

    /** `DarkGray = Color(0xFFA9A9A9)` */
    static DarkGray => Color.FromPtr(DllCall("Color\CreateDarkGrayColor", "Ptr"))

    /** `DarkGrey = Color(0xFFA9A9A9)` */
    static DarkGrey => Color.FromPtr(DllCall("Color\CreateDarkGreyColor", "Ptr"))

    /** `DarkGreen = Color(0xFF006400)` */
    static DarkGreen => Color.FromPtr(DllCall("Color\CreateDarkGreenColor", "Ptr"))

    /** `DarkKhaki = Color(0xFFBDB76B)` */
    static DarkKhaki => Color.FromPtr(DllCall("Color\CreateDarkKhakiColor", "Ptr"))

    /** `DarkMagenta = Color(0xFF8B008B)` */
    static DarkMagenta => Color.FromPtr(DllCall("Color\CreateDarkMagentaColor", "Ptr"))

    /** `DarkOliveGreen = Color(0xFF556B2F)` */
    static DarkOliveGreen => Color.FromPtr(DllCall("Color\CreateDarkOliveGreenColor", "Ptr"))

    /** `DarkOrange = Color(0xFFFF8C00)` */
    static DarkOrange => Color.FromPtr(DllCall("Color\CreateDarkOrangeColor", "Ptr"))

    /** `DarkOrchid = Color(0xFF9932CC)` */
    static DarkOrchid => Color.FromPtr(DllCall("Color\CreateDarkOrchidColor", "Ptr"))

    /** `DarkRed = Color(0xFF8B0000)` */
    static DarkRed => Color.FromPtr(DllCall("Color\CreateDarkRedColor", "Ptr"))

    /** `DarkSalmon = Color(0xFFE9967A)` */
    static DarkSalmon => Color.FromPtr(DllCall("Color\CreateDarkSalmonColor", "Ptr"))

    /** `DarkSeaGreen = Color(0xFF8FBC8F)` */
    static DarkSeaGreen => Color.FromPtr(DllCall("Color\CreateDarkSeaGreenColor", "Ptr"))

    /** `DarkSlateBlue = Color(0xFF483D8B)` */
    static DarkSlateBlue => Color.FromPtr(DllCall("Color\CreateDarkSlateBlueColor", "Ptr"))

    /** `DarkSlateGray = Color(0xFF2F4F4F)` */
    static DarkSlateGray => Color.FromPtr(DllCall("Color\CreateDarkSlateGrayColor", "Ptr"))

    /** `DarkSlateGrey = Color(0xFF2F4F4F)` */
    static DarkSlateGrey => Color.FromPtr(DllCall("Color\CreateDarkSlateGreyColor", "Ptr"))

    /** `DarkTurquoise = Color(0xFF00CED1)` */
    static DarkTurquoise => Color.FromPtr(DllCall("Color\CreateDarkTurquoiseColor", "Ptr"))

    /** `DarkViolet = Color(0xFF9400D3)` */
    static DarkViolet => Color.FromPtr(DllCall("Color\CreateDarkVioletColor", "Ptr"))

    /** `DeepPink = Color(0xFFFF1493)` */
    static DeepPink => Color.FromPtr(DllCall("Color\CreateDeepPinkColor", "Ptr"))

    /** `DeepSkyBlue = Color(0xFF00BFFF)` */
    static DeepSkyBlue => Color.FromPtr(DllCall("Color\CreateDeepSkyBlueColor", "Ptr"))

    /** `DimGray = Color(0xFF696969)` */
    static DimGray => Color.FromPtr(DllCall("Color\CreateDimGrayColor", "Ptr"))

    /** `DimGrey = Color(0xFF696969)` */
    static DimGrey => Color.FromPtr(DllCall("Color\CreateDimGreyColor", "Ptr"))

    /** `DodgerBlue = Color(0xFF1E90FF)` */
    static DodgerBlue => Color.FromPtr(DllCall("Color\CreateDodgerBlueColor", "Ptr"))

    /** `FireBrick = Color(0xFFB22222)` */
    static FireBrick => Color.FromPtr(DllCall("Color\CreateFireBrickColor", "Ptr"))

    /** `FloralWhite = Color(0xFFFFFAF0)` */
    static FloralWhite => Color.FromPtr(DllCall("Color\CreateFloralWhiteColor", "Ptr"))

    /** `ForestGreen = Color(0xFF228B22)` */
    static ForestGreen => Color.FromPtr(DllCall("Color\CreateForestGreenColor", "Ptr"))

    /** `Fuchsia = Color(0xFFFF00FF)` */
    static Fuchsia => Color.FromPtr(DllCall("Color\CreateFuchsiaColor", "Ptr"))

    /** `Gainsboro = Color(0xFFDCDCDC)` */
    static Gainsboro => Color.FromPtr(DllCall("Color\CreateGainsboroColor", "Ptr"))

    /** `GhostWhite = Color(0xFFF8F8FF)` */
    static GhostWhite => Color.FromPtr(DllCall("Color\CreateGhostWhiteColor", "Ptr"))

    /** `Gold = Color(0xFFFFD700)` */
    static Gold => Color.FromPtr(DllCall("Color\CreateGoldColor", "Ptr"))

    /** `GoldenRod = Color(0xFFDAA520)` */
    static GoldenRod => Color.FromPtr(DllCall("Color\CreateGoldenRodColor", "Ptr"))

    /** `Gray = Color(0xFF808080)` */
    static Gray => Color.FromPtr(DllCall("Color\CreateGrayColor", "Ptr"))

    /** `Grey = Color(0xFF808080)` */
    static Grey => Color.FromPtr(DllCall("Color\CreateGreyColor", "Ptr"))

    /** `Green = Color(0xFF008000)` */
    static Green => Color.FromPtr(DllCall("Color\CreateGreenColor", "Ptr"))

    /** `GreenYellow = Color(0xFFADFF2F)` */
    static GreenYellow => Color.FromPtr(DllCall("Color\CreateGreenYellowColor", "Ptr"))

    /** `Honeydew = Color(0xFFF0FFF0)` */
    static Honeydew => Color.FromPtr(DllCall("Color\CreateHoneydewColor", "Ptr"))

    /** `HotPink = Color(0xFFFF69B4)` */
    static HotPink => Color.FromPtr(DllCall("Color\CreateHotPinkColor", "Ptr"))

    /** `IndianRed = Color(0xFFCD5C5C)` */
    static IndianRed => Color.FromPtr(DllCall("Color\CreateIndianRedColor", "Ptr"))

    /** `Indigo = Color(0xFF4B0082)` */
    static Indigo => Color.FromPtr(DllCall("Color\CreateIndigoColor", "Ptr"))

    /** `Ivory = Color(0xFFFFFFF0)` */
    static Ivory => Color.FromPtr(DllCall("Color\CreateIvoryColor", "Ptr"))

    /** `Khaki = Color(0xFFF0E68C)` */
    static Khaki => Color.FromPtr(DllCall("Color\CreateKhakiColor", "Ptr"))

    /** `Lavender = Color(0xFFE6E6FA)` */
    static Lavender => Color.FromPtr(DllCall("Color\CreateLavenderColor", "Ptr"))

    /** `LavenderBlush = Color(0xFFFFF0F5)` */
    static LavenderBlush => Color.FromPtr(DllCall("Color\CreateLavenderBlushColor", "Ptr"))

    /** `LawnGreen = Color(0xFF7CFC00)` */
    static LawnGreen => Color.FromPtr(DllCall("Color\CreateLawnGreenColor", "Ptr"))

    /** `LemonChiffon = Color(0xFFFFFACD)` */
    static LemonChiffon => Color.FromPtr(DllCall("Color\CreateLemonChiffonColor", "Ptr"))

    /** `LightBlue = Color(0xFFADD8E6)` */
    static LightBlue => Color.FromPtr(DllCall("Color\CreateLightBlueColor", "Ptr"))

    /** `LightCoral = Color(0xFFF08080)` */
    static LightCoral => Color.FromPtr(DllCall("Color\CreateLightCoralColor", "Ptr"))

    /** `LightCyan = Color(0xFFE0FFFF)` */
    static LightCyan => Color.FromPtr(DllCall("Color\CreateLightCyanColor", "Ptr"))

    /** `LightGoldenrodYellow = Color(0xFFFAFAD2)` */
    static LightGoldenrodYellow => Color.FromPtr(DllCall("Color\CreateLightGoldenrodYellowColor", "Ptr"))

    /** `LightGray = Color(0xFFD3D3D3)` */
    static LightGray => Color.FromPtr(DllCall("Color\CreateLightGrayColor", "Ptr"))

    /** `LightGreen = Color(0xFF90EE90)` */
    static LightGreen => Color.FromPtr(DllCall("Color\CreateLightGreenColor", "Ptr"))

    /** `LightGrey = Color(0xFFD3D3D3)` */
    static LightGrey => Color.FromPtr(DllCall("Color\CreateLightGreyColor", "Ptr"))

    /** `LightPink = Color(0xFFFFB6C1)` */
    static LightPink => Color.FromPtr(DllCall("Color\CreateLightPinkColor", "Ptr"))

    /** `LightSalmon = Color(0xFFFFA07A)` */
    static LightSalmon => Color.FromPtr(DllCall("Color\CreateLightSalmonColor", "Ptr"))

    /** `LightSeaGreen = Color(0xFF20B2AA)` */
    static LightSeaGreen => Color.FromPtr(DllCall("Color\CreateLightSeaGreenColor", "Ptr"))

    /** `LightSkyBlue = Color(0xFF87CEFA)` */
    static LightSkyBlue => Color.FromPtr(DllCall("Color\CreateLightSkyBlueColor", "Ptr"))

    /** `LightSlateGrey = Color(0xFF778899)` */
    static LightSlateGrey => Color.FromPtr(DllCall("Color\CreateLightSlateGreyColor", "Ptr"))

    /** `LightSteelBlue = Color(0xFFB0C4DE)` */
    static LightSteelBlue => Color.FromPtr(DllCall("Color\CreateLightSteelBlueColor", "Ptr"))

    /** `LightYellow = Color(0xFFFFFFE0)` */
    static LightYellow => Color.FromPtr(DllCall("Color\CreateLightYellowColor", "Ptr"))

    /** `Lime = Color(0xFF00FF00)` */
    static Lime => Color.FromPtr(DllCall("Color\CreateLimeColor", "Ptr"))

    /** `LimeGreen = Color(0xFF32CD32)` */
    static LimeGreen => Color.FromPtr(DllCall("Color\CreateLimeGreenColor", "Ptr"))

    /** `Linen = Color(0xFFFAF0E6)` */
    static Linen => Color.FromPtr(DllCall("Color\CreateLinenColor", "Ptr"))

    /** `Magenta = Color(0xFFFF00FF)` */
    static Magenta => Color.FromPtr(DllCall("Color\CreateMagentaColor", "Ptr"))

    /** `Maroon = Color(0xFF800000)` */
    static Maroon => Color.FromPtr(DllCall("Color\CreateMaroonColor", "Ptr"))

    /** `MediumAquamarine = Color(0xFF66CDAA)` */
    static MediumAquamarine => Color.FromPtr(DllCall("Color\CreateMediumAquamarineColor", "Ptr"))

    /** `MediumBlue = Color(0xFF0000CD)` */
    static MediumBlue => Color.FromPtr(DllCall("Color\CreateMediumBlueColor", "Ptr"))

    /** `MediumOrchid = Color(0xFFBA55D3)` */
    static MediumOrchid => Color.FromPtr(DllCall("Color\CreateMediumOrchidColor", "Ptr"))

    /** `MediumPurple = Color(0xFF9370DB)` */
    static MediumPurple => Color.FromPtr(DllCall("Color\CreateMediumPurpleColor", "Ptr"))

    /** `MediumSeaGreen = Color(0xFF3CB371)` */
    static MediumSeaGreen => Color.FromPtr(DllCall("Color\CreateMediumSeaGreenColor", "Ptr"))

    /** `MediumSlateBlue = Color(0xFF7B68EE)` */
    static MediumSlateBlue => Color.FromPtr(DllCall("Color\CreateMediumSlateBlueColor", "Ptr"))

    /** `MediumSpringGreen = Color(0xFF00FA9A)` */
    static MediumSpringGreen => Color.FromPtr(DllCall("Color\CreateMediumSpringGreenColor", "Ptr"))

    /** `MediumTurquoise = Color(0xFF48D1CC)` */
    static MediumTurquoise => Color.FromPtr(DllCall("Color\CreateMediumTurquoiseColor", "Ptr"))

    /** `MediumVioletRed = Color(0xFFC71585)` */
    static MediumVioletRed => Color.FromPtr(DllCall("Color\CreateMediumVioletRedColor", "Ptr"))

    /** `MidnightBlue = Color(0xFF191970)` */
    static MidnightBlue => Color.FromPtr(DllCall("Color\CreateMidnightBlueColor", "Ptr"))

    /** `MintCream = Color(0xFFF5FFFA)` */
    static MintCream => Color.FromPtr(DllCall("Color\CreateMintCreamColor", "Ptr"))

    /** `MistyRose = Color(0xFFFFE4E1)` */
    static MistyRose => Color.FromPtr(DllCall("Color\CreateMistyRoseColor", "Ptr"))

    /** `Moccasin = Color(0xFFFFE4B5)` */
    static Moccasin => Color.FromPtr(DllCall("Color\CreateMoccasinColor", "Ptr"))

    /** `NavajoWhite = Color(0xFFFFDEAD)` */
    static NavajoWhite => Color.FromPtr(DllCall("Color\CreateNavajoWhiteColor", "Ptr"))

    /** `Navy = Color(0xFF000080)` */
    static Navy => Color.FromPtr(DllCall("Color\CreateNavyColor", "Ptr"))

    /** `OldLace = Color(0xFFFDF5E6)` */
    static OldLace => Color.FromPtr(DllCall("Color\CreateOldLaceColor", "Ptr"))

    /** `Olive = Color(0xFF808000)` */
    static Olive => Color.FromPtr(DllCall("Color\CreateOliveColor", "Ptr"))

    /** `OliveDrab = Color(0xFF6B8E23)` */
    static OliveDrab => Color.FromPtr(DllCall("Color\CreateOliveDrabColor", "Ptr"))

    /** `Orange = Color(0xFFFFA500)` */
    static Orange => Color.FromPtr(DllCall("Color\CreateOrangeColor", "Ptr"))

    /** `OrangeRed = Color(0xFFFF4500)` */
    static OrangeRed => Color.FromPtr(DllCall("Color\CreateOrangeRedColor", "Ptr"))

    /** `Orchid = Color(0xFFDA70D6)` */
    static Orchid => Color.FromPtr(DllCall("Color\CreateOrchidColor", "Ptr"))

    /** `PaleGoldenrod = Color(0xFFEEE8AA)` */
    static PaleGoldenrod => Color.FromPtr(DllCall("Color\CreatePaleGoldenrodColor", "Ptr"))

    /** `PaleGreen = Color(0xFF98FB98)` */
    static PaleGreen => Color.FromPtr(DllCall("Color\CreatePaleGreenColor", "Ptr"))

    /** `PaleTurquoise = Color(0xFFAFEEEE)` */
    static PaleTurquoise => Color.FromPtr(DllCall("Color\CreatePaleTurquoiseColor", "Ptr"))

    /** `PaleVioletRed = Color(0xFFDB7093)` */
    static PaleVioletRed => Color.FromPtr(DllCall("Color\CreatePaleVioletRedColor", "Ptr"))

    /** `PapayaWhip = Color(0xFFFFEFD5)` */
    static PapayaWhip => Color.FromPtr(DllCall("Color\CreatePapayaWhipColor", "Ptr"))

    /** `PeachPuff = Color(0xFFFFDAB9)` */
    static PeachPuff => Color.FromPtr(DllCall("Color\CreatePeachPuffColor", "Ptr"))

    /** `Peru = Color(0xFFCD853F)` */
    static Peru => Color.FromPtr(DllCall("Color\CreatePeruColor", "Ptr"))

    /** `Pink = Color(0xFFFFC0CB)` */
    static Pink => Color.FromPtr(DllCall("Color\CreatePinkColor", "Ptr"))

    /** `Plum = Color(0xFFDDA0DD)` */
    static Plum => Color.FromPtr(DllCall("Color\CreatePlumColor", "Ptr"))

    /** `PowderBlue = Color(0xFFB0E0E6)` */
    static PowderBlue => Color.FromPtr(DllCall("Color\CreatePowderBlueColor", "Ptr"))

    /** `Purple = Color(0xFF800080)` */
    static Purple => Color.FromPtr(DllCall("Color\CreatePurpleColor", "Ptr"))

    /** `RebeccaPurple = Color(0xFF663399)` */
    static RebeccaPurple => Color.FromPtr(DllCall("Color\CreateRebeccaPurpleColor", "Ptr"))

    /** `Red = Color(0xFFFF0000)` */
    static Red => Color.FromPtr(DllCall("Color\CreateRedColor", "Ptr"))

    /** `RosyBrown = Color(0xFFBC8F8F)` */
    static RosyBrown => Color.FromPtr(DllCall("Color\CreateRosyBrownColor", "Ptr"))

    /** `RoyalBlue = Color(0xFF4169E1)` */
    static RoyalBlue => Color.FromPtr(DllCall("Color\CreateRoyalBlueColor", "Ptr"))

    /** `SaddleBrown = Color(0xFF8B4513)` */
    static SaddleBrown => Color.FromPtr(DllCall("Color\CreateSaddleBrownColor", "Ptr"))

    /** `Salmon = Color(0xFFFA8072)` */
    static Salmon => Color.FromPtr(DllCall("Color\CreateSalmonColor", "Ptr"))

    /** `SandyBrown = Color(0xFFF4A460)` */
    static SandyBrown => Color.FromPtr(DllCall("Color\CreateSandyBrownColor", "Ptr"))

    /** `SeaGreen = Color(0xFF2E8B57)` */
    static SeaGreen => Color.FromPtr(DllCall("Color\CreateSeaGreenColor", "Ptr"))

    /** `Seashell = Color(0xFFFFF5EE)` */
    static Seashell => Color.FromPtr(DllCall("Color\CreateSeashellColor", "Ptr"))

    /** `Sienna = Color(0xFFA0522D)` */
    static Sienna => Color.FromPtr(DllCall("Color\CreateSiennaColor", "Ptr"))

    /** `Silver = Color(0xFFC0C0C0)` */
    static Silver => Color.FromPtr(DllCall("Color\CreateSilverColor", "Ptr"))

    /** `SkyBlue = Color(0xFF87CEEB)` */
    static SkyBlue => Color.FromPtr(DllCall("Color\CreateSkyBlueColor", "Ptr"))

    /** `SlateBlue = Color(0xFF6A5ACD)` */
    static SlateBlue => Color.FromPtr(DllCall("Color\CreateSlateBlueColor", "Ptr"))

    /** `SlateGray = Color(0xFF708090)` */
    static SlateGray => Color.FromPtr(DllCall("Color\CreateSlateGrayColor", "Ptr"))

    /** `SlateGrey = Color(0xFF708090)` */
    static SlateGrey => Color.FromPtr(DllCall("Color\CreateSlateGreyColor", "Ptr"))

    /** `Snow = Color(0xFFFFFAFA)` */
    static Snow => Color.FromPtr(DllCall("Color\CreateSnowColor", "Ptr"))

    /** `SpringGreen = Color(0xFF00FF7F)` */
    static SpringGreen => Color.FromPtr(DllCall("Color\CreateSpringGreenColor", "Ptr"))

    /** `SteelBlue = Color(0xFF4682B4)` */
    static SteelBlue => Color.FromPtr(DllCall("Color\CreateSteelBlueColor", "Ptr"))

    /** `Tan = Color(0xFFD2B48C)` */
    static Tan => Color.FromPtr(DllCall("Color\CreateTanColor", "Ptr"))

    /** `Teal = Color(0xFF008080)` */
    static Teal => Color.FromPtr(DllCall("Color\CreateTealColor", "Ptr"))

    /** `Thistle = Color(0xFFD8BFD8)` */
    static Thistle => Color.FromPtr(DllCall("Color\CreateThistleColor", "Ptr"))

    /** `Tomato = Color(0xFFFF6347)` */
    static Tomato => Color.FromPtr(DllCall("Color\CreateTomatoColor", "Ptr"))

    /** `Transparent = Color(0xFF000000)` */
    static Transparent => Color.FromPtr(DllCall("Color\CreateTransparentColor", "Ptr"))

    /** `Turquoise = Color(0xFF40E0D0)` */
    static Turquoise => Color.FromPtr(DllCall("Color\CreateTurquoiseColor", "Ptr"))

    /** `Violet = Color(0xFFEE82EE)` */
    static Violet => Color.FromPtr(DllCall("Color\CreateVioletColor", "Ptr"))

    /** `Wheat = Color(0xFFF5DEB3)` */
    static Wheat => Color.FromPtr(DllCall("Color\CreateWheatColor", "Ptr"))

    /** `White = Color(0xFFFFFFFF)` */
    static White => Color.FromPtr(DllCall("Color\CreateWhiteColor", "Ptr"))

    /** `WhiteSmoke = Color(0xFFF5F5F5)` */
    static WhiteSmoke => Color.FromPtr(DllCall("Color\CreateWhiteSmokeColor", "Ptr"))

    /** `WhiteTransparent = Color(0x00FFFFFF)` */
    static WhiteTransparent => Color.FromPtr(DllCall("Color\CreateWhiteTransparentColor", "Ptr"))

    /** `Yellow = Color(0xFFFFFF00)` */
    static Yellow => Color.FromPtr(DllCall("Color\CreateYellowColor", "Ptr"))

    /** `YellowGreen = Color(0xFF9ACD32)` */
    static YellowGreen => Color.FromPtr(DllCall("Color\CreateYellowGreenColor", "Ptr"))

    /**
     * Creates a Color object from a pointer.
     * @param {Ptr} Ptr - The pointer to the color data.
     * @returns {Color}
     */
    static FromPtr(Ptr) => {base: Color.Prototype, Ptr: Ptr}

    /**
     * Creates a Color object from Adobe RGB values.
     * @param {number} r - The red value `(0 to 1)`.
     * @param {number} g - The green value `(0 to 1)`.
     * @param {number} b - The blue value `(0 to 1)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromAdobeRGB(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromAdobeRGB", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from HSL values.
     * @param {number} h - The hue value `(0 to 360)`.
     * @param {number} s - The saturation value `(0 to 100)`.
     * @param {number} l - The lightness value `(0 to 100)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromHSL(h, s, l, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHSL" , "Double", h, "Double", s, "Double", l, "Double", a, "Ptr"))

    /**
     * Creates a Color object from HSV values.
     * @param {number} h - The hue value `(0 to 360)`.
     * @param {number} s - The saturation value `(0 to 100)`.
     * @param {number} v - The value `(0 to 100)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromHSV(h, s, v, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHSV", "Double", h, "Double", s / 100, "Double", v / 100, "Double", a, "Ptr"))

    /**
     * Creates a Color object from HSV values.
     * @param {number} h - The hue value `(0 to 360)`.
     * @param {number} s - The saturation value `(0 to 100)`.
     * @param {number} i - The intensity `(0 to 100)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromHSI(h, s, i, a := 255) => Color(DllCall("Color\ColorFromHSI", "Double", h, "Double", s, "Double", i, "Int", a, "Ptr", "Cdecl"))

    /**
     * Creates a Color object from HWB values.
     * @param {number} h - The hue value `(0 to 360)`.
     * @param {number} w - The whiteness value `(0 to 100)`.
     * @param {number} b - The blackness value `(0 to 100)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromHWB(h, w, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHWB" , "Double", h, "Double", w, "Double", b, "Double", a, "Ptr"))

    /**
     * Creates a Color object from HSV values.
     * @param {number} h - The hue value `(0 to 360)`.
     * @param {number} s - The saturation value `(0 to 100)`.
     * @param {number} p - The perceived brightness value `(0 to 255)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromHSP(h, s, p, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHSP" , "Double", h, "Double", s, "Double", p, "Double", a, "Ptr"))

    /**
     * Creates a Color object from HCY values.
     * @param {number} h - The hue value `(0 to 360)`.
     * @param {number} c - The chroma value `(0 to 100)`.
     * @param {number} y - The luma value `(0 to 255)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromHCY(h, c, y, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHCY" , "Double", h, "Double", c, "Double", y, "Double", a, "Ptr"))

    /**
     * Creates a Color object from HCG values.
     * @param {number} h - The hue value `(0 to 360)`.
     * @param {number} c - The chroma value `(0 to 100)`.
     * @param {number} g - The gray value `(0 to 100)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromHCG(h, c, g, a := 255) => Color.FromPtr(DllCall("Color\ColorFromHCG" , "Double", h, "Double", c, "Double", g, "Double", a, "Ptr"))

    static FromTSL(t, s, l, a := 255) => Color.FromPtr(DllCall("Color\ColorFromTSL", "Double", t, "Double", s, "Double", l, "Int", a, "Ptr"))

    /**
     * Creates a Color object from XYZ D50 values.
     * @param {number} x - The X value `(0 to 1)`.
     * @param {number} y - The Y value `(0 to 1)`.
     * @param {number} z - The Z value `(0 to 1)`.
     * @param {number} [a=255] - The alpha value (0 to 255).
     * @returns {Color}
     */
    static FromXYZ_D50(x, y, z, a := 255) => Color.FromPtr(DllCall("Color\ColorFromXYZ_D50" , "Double", x, "Double", y, "Double", z, "Double", a, "Ptr"))

    /**
     * Creates a Color object from XYZ D65 values.
     * @param {number} x - The X value `(0 to 95.045592705167)`.
     * @param {number} y - The Y value `(0 to 100)`.
     * @param {number} z - The Z value `(0 to 108.9057750759878)`.
     * @param {number} [a=255] - The alpha value (0 to 255).
     * @returns {Color}
     */
    static FromXYZ_D65(x, y, z, a := 255) => Color.FromPtr(DllCall("Color\ColorFromXYZ_D65" , "Double", x, "Double", y, "Double", z, "Double", a, "Ptr"))

    /**
     * Creates a new Color from UCS values.
     * @param {number} u - The U component
     * @param {number} v - The V component
     * @param {number} w - The W component
     * @param {number} [a=255] - The alpha component
     * @returns {Color}
     */
    static FromUCS(u, c, s, a := 255) => Color.FromPtr(DllCall("Color\ColorFromUCS", "Double", u, "Double", c, "Double", s, "Int", a, "Ptr"))

    /**
     * Creates a new Color from UVW values.
     * @param {number} u - The U component `(-134 to 224)`
     * @param {number} v - The V component `(-140 to 122)`
     * @param {number} w - The W component `(0 to 100)`
     * @param {number} [a=255] - The alpha component
     * @returns {Color}
     */
    static FromUVW(u, v, w, a := 255) => Color.FromPtr(DllCall("Color\ColorFromUVW", "Double", u, "Double", v, "Double", w, "Int", a, "Ptr"))

    static FromXYY(x, y, y_, a := 255) => Color.FromPtr(DllCall("Color\ColorFromXYY", "Double", x, "Double", y, "Double", y_, "Int", a, "Ptr"))

    /**
     * Creates a Color object from CIE Lab values.
     * @param {number} l - The L value `(0 to 100)`.
     * @param {number} a - The a value `(-100 to 100)`.
     * @param {number} b - The b value `(-100 to 100)`.
     * @param {number} [t=255] - The alpha value `(0 to 255)`.
     * @returns {Color} A new Color object.
     */
    static FromLab(l, a, b, t := 255) => Color.FromPtr(DllCall("Color\ColorFromLab" , "Double", l, "Double", a, "Double", b, "Double", t, "Ptr"))

    /**
     * Creates a Color object from LCHuv values.
     * @param {number} l - The L value `(0 to 100)`.
     * @param {number} a - The a value `(0 to 100)`.
     * @param {number} b - The b value `(0 to 360)`.
     * @param {number} [alpha=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromLCHuv(l, c, h, a := 255) => Color.FromPtr(DllCall("Color\ColorFromLCHuv", "Double", l, "Double", c, "Double", h, "Int", a, "Ptr"))

    /**
     * Creates a Color object from CIE Luv values.
     * @param {number} L - The L value `(0 to 100)`.
     * @param {number} u - The u value `(-134-224)`.
     * @param {number} v - The v value `(-140-122)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromLuv(l, u, v, a := 255) => Color.FromPtr(DllCall("Color\ColorFromLuv", "Double", l, "Double", u, "Double", v, "Int", a, "Ptr"))

    /**
     * Creates a Color object from LCH values.
     * @param {number} l - The Lightness value `(0 to 100)`.
     * @param {number} c - The Chroma value `(0 to 100)`.
     * @param {number} h - The Hue value `(0 to 360)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromLCHab(l, c, h, a := 255) => Color.FromPtr(DllCall("Color\ColorFromLCHab", "Double", l, "Double", c, "Double", h, "Double", a, "Ptr"))

    /**
     * Creates a Color object from YPbPr values.
     * @param {number} y - The Y value `(0 to 1)`.
     * @param {number} cb - The Cb value `(-0.5 to 0.5)`.
     * @param {number} cr - The Cr value `(-0.5 to 0.5)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromYPbPr(y, cb, cr, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYPbPr", "Double", y, "Double", cb, "Double", cr, "Double", a, "Ptr"))

    static FromYCbCr(y, cb, cr, a := 255, yCbCrType := Color.YCbCrType.BT709) => Color.FromPtr(DllCall("Color\ColorFromYCbCr", "Double", y, "Double", cb, "Double", cr, "Int", a, "Int", yCbCrType, "Ptr"))

    static FromYCgCo(y, cg, co, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYCgCo", "Double", y, "Double", cg, "Double", co, "Int", a, "Ptr"))

    static FromYcCbcCrc(y, cb, cr, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYcCbcCrc", "Double", y, "Double", cb, "Double", cr, "Int", a, "Ptr"))

    static FromYDbDr(y, db, dr, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYDbDr", "Double", y, "Double", db, "Double", dr, "Int", a, "Ptr"))

    static FromYUV(y, u, v, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYUV", "Double", y, "Double", u, "Double", v, "Int", a, "Ptr"))

    static FromYES(y, e, s, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYES", "Double", y, "Double", e, "Double", s, "Int", a, "Ptr"))

    static FromJPEG(y, cb, cr, a := 255) => Color.FromPtr(DllCall("Color\ColorFromJPEG", "Double", y, "Double", cb, "Double", cr, "Int", a, "Ptr"))

    /**
     * Creates a Color object from YIQ values.
     * @param {number} y - The Y value `(0 to 1).
     * @param {number} i - The I value `(-0.5957 to 0.5957)`.
     * @param {number} q - The Q value `(-0.5226 to 0.5226)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromYIQ(y, i, q, a := 255) => Color.FromPtr(DllCall("Color\ColorFromYIQ" , "Double", y, "Double", i, "Double", q, "Double", a, "Ptr"))

    static FromCMY(c, m, y, a := 255) => Color.FromPtr(DllCall("Color\ColorFromCMY", "Double", c, "Double", m, "Double", y, "Int", a, "Ptr"))

    /**
     * Creates a Color object from CMYK values.
     * @param {number} c - The cyan value `(0 to 100)`.
     * @param {number} m - The magenta value `(0 to 100)`.
     * @param {number} y - The yellow value `(0 to 100)`.
     * @param {number} k - The key (black) value `(0 to 100)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
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
     * @param {number} r - The red value `(0 to 1)`.
     * @param {number} g - The green value `(0 to 1)`.
     * @param {number} b - The blue value `(0 to 1)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromLinearSRGB(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromLinearSRGB", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from ProPhoto RGB values.
     * @param {number} r - The red value `(0 to 1)`.
     * @param {number} g - The green value `(0 to 1)`.
     * @param {number} b - The blue value `(0 to 1)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromProPhotoRGB(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromProPhotoRGB", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from Rec.2020 values.
     * @param {number} r - The red value `(0 to 1)`.
     * @param {number} g - The green value `(0 to 1)`.
     * @param {number} b - The blue value `(0 to 1)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromRec2020(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromRec2020", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from Display-P3 values.
     * @param {number} r - The red value `(0 to 1)`.
     * @param {number} g - The green value `(0 to 1)`.
     * @param {number} b - The blue value `(0 to 1)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromDisplayP3(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromDisplayP3", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from OKLab values.
     * @param {number} l - The lightness value `(0 to 1)`.
     * @param {number} a - The green-red value `(-0.4 to 0.4)`.
     * @param {number} b - The blue-yellow value `(-0.4 to 0.4)`.
     * @param {number} [alpha=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromOKLab(l, a, b, alpha := 255) => Color.FromPtr(DllCall("Color\ColorFromOKLab", "Double", l, "Double", a, "Double", b, "Int", alpha, "Ptr"))

    /**
     * Creates a Color object from OKLCH values.
     * @param {number} l - The lightness value `(0 to 1)`.
     * @param {number} c - The chroma value `(0 to 0.4)`.
     * @param {number} h - The hue value `(0 to 360)`.
     * @param {number} [alpha=255] - The alpha value `(0 to 255)`.
     * @returns {Color}
     */
    static FromOKLCH(l, c, h, alpha := 255) => Color.FromPtr(DllCall("Color\ColorFromOKLCH", "Double", l, "Double", c, "Double", h, "Int", alpha, "Ptr"))

    /**
     * Creates a new Color from ACEScg values.
     * @param {number} r - The red component
     * @param {number} g - The green component
     * @param {number} b - The blue component
     * @param {number} [a=255] - The alpha component
     * @returns {Color}
     */
    static FromACEScg(r, g, b, a := 255) => Color.FromPtr(DllCall("Color\ColorFromACEScg", "Double", r, "Double", g, "Double", b, "Int", a, "Ptr"))

    /**
     * Creates a Color object from NCol values.
     * @param {string} h - The hue string `("R", "Y", "G", "C", "B", or "M")`.
     * @param {number} w - The whiteness value `(0 to 100)`.
     * @param {number} b - The blackness value `(0 to 100)`.
     * @param {number} [a=255] - The alpha value `(0 to 255)`.
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
    Grayscale(factor := 1) => (DllCall("Color\GrayscaleColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a sepia tone to the color.
     * @returns {Color}
     */
    Sepia(factor := 1) => (DllCall("Color\SepiaColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a cross process matrix to the color.
     * @returns {Color}
     */
    CrossProcess(factor := 1) => (DllCall("Color\CrossProcessColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a moonlight matrix to the color.
     * @returns {Color}
     */
    Moonlight(factor := 1) => (DllCall("Color\MoonlightColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a vintage film matrix to the color.
     * @returns {Color}
     */
    VintageFilm(factor := 1) => (DllCall("Color\VintageFilmColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a technicolor matrix to the color.
     * @returns {Color}
     */
    Technicolor(factor := 1) => (DllCall("Color\TechnicolorColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Applies a Polaroid matrix to the color.
     * @returns {Color}
     */
    Polaroid(factor := 1) => (DllCall("Color\PolaroidColor", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Shifts the hue of the color by the specified number of degrees.
     * @param {number} degrees - The number of degrees to shift the hue.
     * @returns {Color}
     */
    ShiftHue(degrees) => (DllCall("Color\ShiftHueColor", "Ptr", this.Ptr, "Double", degrees), this)

    /**
     * Shifts the saturation of the color by the specified amount.
     * @param {number} amount - The amount to shift the saturation.
     * @returns {Color}
     */
    ShiftSaturation(amount) => (DllCall("Color\ShiftSaturationColor", "Ptr", this.Ptr, "Double", amount), this)

    /**
     * Shifts the lightness of the color by the specified amount.
     * @param {number} amount - The amount to shift the lightness.
     * @returns {Color}
     */
    ShiftLightness(amount) => (DllCall("Color\ShiftLightnessColor", "Ptr", this.Ptr, "Double", amount), this)

    ShiftValue(amount) => (DllCall("Color\ShiftValueColor", "Ptr", this.Ptr, "Double", amount), this)

    ShiftIntensity(amount) => (DllCall("Color\ShiftIntensityColor", "Ptr", this.Ptr, "Double", amount), this)

    ShiftPerception(amount) => (DllCall("Color\ShiftPerceptionColor", "Ptr", this.Ptr, "Double", amount), this)

    ShiftChroma(amount) => (DllCall("Color\ShiftChromaColor", "Ptr", this.Ptr, "Double", amount), this)

    ShiftLuma(amount) => (DllCall("Color\ShiftLumaColor", "Ptr", this.Ptr, "Double", amount), this)

    ShiftGray(amount) => (DllCall("Color\ShiftGrayColor", "Ptr", this.Ptr, "Double", amount), this)

    /**
     * Shifts the whiteness of the color by the specified amount.
     * @param {number} amount - The amount to shift the whiteness.
     * @returns {Color}
     */
    ShiftWhiteLevel(amount) => (DllCall("Color\ShiftWhiteLevelColor", "Ptr", this.Ptr, "Double", amount), this)

    /**
     * Shifts the blac level of the color by the specified amount.
     * @param {number} amount - The amount to shift the blackness.
     * @returns {Color}
     */
    ShiftBlackLevel(amount) => (DllCall("Color\ShiftBlackLevelColor", "Ptr", this.Ptr, "Double", amount), this)

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
    Complement() => (DllCall("Color\ColorComplement", "Ptr", this.Ptr), this)

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
     * @returns {Object} An object containing R, G, B, and A values (0 to 255).
     */
    ToRGB() => (DllCall("Color\ColorToRGB", "Ptr", this.Ptr, "Ptr", r := Buffer(4, 0), "Ptr", g := Buffer(4, 0), "Ptr", b := Buffer(4, 0), "Ptr", a := Buffer(4, 0)), {R: NumGet(r, "Int"), G: NumGet(g, "Int"), B: NumGet(b, "Int"), A: NumGet(a, "Int")})

    /**
     * Converts the color to sRGB format.
     * @returns {Object} An object containing r, g, b (0 to 1), and A (0 to 255) values.
     */
    ToLinearSRGB() => (DllCall("Color\ColorToLinearSRGB", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {r: NumGet(r, "Double"), g: NumGet(g, "Double"), b: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to ProPhoto RGB format.
     * @returns {Object}
     */
    ToProPhotoRGB() => (DllCall("Color\ColorToProPhotoRGB", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {r: NumGet(r, "Double"), g: NumGet(g, "Double"), b: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to Adobe RGB format.
     * @returns {Object} An object containing r, g, b (0 to 1), and A (0 to 255) values.
     */
    ToAdobeRGB() => (DllCall("Color\ColorToAdobeRGB", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {r: NumGet(r, "Double"), g: NumGet(g, "Double"), b: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to hexadecimal format.
     * @returns {Object} An object containing A, R, G, and B values as hexadecimal strings.
     */
    ToHex() => (DllCall("Color\ColorToHex", "Ptr", this.Ptr, "Ptr", a := Buffer(3, 0), "Ptr", r := Buffer(3, 0), "Ptr", g := Buffer(3, 0), "Ptr", b := Buffer(3, 0)), {A: StrGet(a, "UTF-8"), R: StrGet(r, "UTF-8"), G: StrGet(g, "UTF-8"), B: StrGet(b, "UTF-8")})

    /**
     * Converts the color to HSL format.
     * @returns {Object} An object containing H (0 to 360), S (0 to 100), L (0 to 100), and A (0 to 255) values.
     */
    ToHSL() => (DllCall("Color\ColorToHSL"    , "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0)  , "Ptr", s := Buffer(8, 0), "Ptr", l := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), S: NumGet(s, "Double"), L: NumGet(l, "Double")      , A: NumGet(a, "Int")})

    /**
     * Converts the color to HSV format.
     * @returns {Object} An object containing H (0 to 360), S (0 to 100), V (0 to 100), and A (0 to 255) values.
     */
    ToHSV() => (DllCall("Color\ColorToHSV", "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0), "Ptr", s := Buffer(8, 0), "Ptr", v := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), S: NumGet(s, "Double"), V: NumGet(v, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to HSI format.
     * @returns {Object} An object containing H (0 to 360), S (0 to 100), I (0 to 100), and A (0 to 255) values.
     */
    ToHSI() => (DllCall("Color\ColorToHSI", "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0), "Ptr", s := Buffer(8, 0), "Ptr", i := Buffer(8, 0), "Ptr", a := Buffer(4, 0), "Cdecl"), {H: NumGet(h, "Double"), S: NumGet(s, "Double"), I: NumGet(i, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to HWB format.
     * @returns {Object} An object containing H (0 to 360), W (0 to 100), B (0 to 100), and A (0 to 255) values.
     */
    ToHWB() => (DllCall("Color\ColorToHWB"    , "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0)  , "Ptr", w := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), W: NumGet(w, "Double"), B: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to HSP format.
     * @returns {Object} An object containing H (0 to 360), S (0 to 100), P (0 to 255), and A (0 to 255) values.
     */
    ToHSP() => (DllCall("Color\ColorToHSP", "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0)  , "Ptr", s := Buffer(8, 0), "Ptr", p := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), S: NumGet(s, "Double"), P: NumGet(p, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to HCY format.
     * @returns {Object} An object containing H (0 to 360), C (0 to 100), Y (0 to 255), and A (0 to 255) values.
     */
    ToHCY() => (DllCall("Color\ColorToHCY", "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0)  , "Ptr", c := Buffer(8, 0), "Ptr", y := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), C: NumGet(c, "Double"), Y: NumGet(y, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to HCG format.
     * @returns {Object} An object containing H (0 to 360), C (0 to 100), G (0 to 100), and A (0 to 255) values.
     */
    ToHCG() => (DllCall("Color\ColorToHCG", "Ptr", this.Ptr, "Ptr", h := Buffer(8, 0)  , "Ptr", c := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {H: NumGet(h, "Double"), C: NumGet(c, "Double"), G: NumGet(g, "Double"), A: NumGet(a, "Int")})

    ToTSL() => (DllCall("Color\ColorToTSL", "Ptr", this.Ptr, "Ptr", t := Buffer(8, 0), "Ptr", s := Buffer(8, 0), "Ptr", l := Buffer(8, 0), "Ptr", a := Buffer(4, 0), "Cdecl"), {T: NumGet(t, "Double"), S: NumGet(s, "Double"), L: NumGet(l, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to XYZ D50 format.
     * @returns {Object} An object containing X, Y, Z, and A values.
     */
    ToXYZ_D50() => (DllCall("Color\ColorToXYZ_D50", "Ptr", this.Ptr, "Ptr", x := Buffer(8, 0)  , "Ptr", y := Buffer(8, 0), "Ptr", z := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {X: NumGet(x, "Double"), Y: NumGet(y, "Double"), Z: NumGet(z, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to XYZ D65 format.
     * @returns {Object} An object containing X, Y, Z, and A values.
     */
    ToXYZ_D65() => (DllCall("Color\ColorToXYZ_D65", "Ptr", this.Ptr, "Ptr", x := Buffer(8, 0)  , "Ptr", y := Buffer(8, 0), "Ptr", z := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {X: NumGet(x, "Double"), Y: NumGet(y, "Double"), Z: NumGet(z, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to UCS color space.
     * @returns {Object} Object containing U, V, W, A values
     */
    ToUCS() => (DllCall("Color\ColorToUCS", "Ptr", this.Ptr, "Ptr", u := Buffer(8, 0), "Ptr", c := Buffer(8, 0), "Ptr", s := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {U: NumGet(u, "Double"), C: NumGet(c, "Double"), S: NumGet(s, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to UVW color space.
     * @returns {Object} Object containing U, V, W, A values
     */
    ToUVW() => (DllCall("Color\ColorToUVW", "Ptr", this.Ptr, "Ptr", u := Buffer(8, 0), "Ptr", v := Buffer(8, 0), "Ptr", w := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {U: NumGet(u, "Double"), V: NumGet(v, "Double"), W: NumGet(w, "Double"), A: NumGet(a, "Int")})

    ToXYY() => (DllCall("Color\ColorToXYY", "Ptr", this.Ptr, "Ptr", x := Buffer(8, 0)  , "Ptr", y := Buffer(8, 0), "Ptr", y_ := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {X: NumGet(x, "Double"), Y: NumGet(y, "Double"), Y_: NumGet(y_, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to CIELab format.
     * @returns {Object} An object containing L, A, B, and T (alpha) values.
     */
    ToLab() => (DllCall("Color\ColorToLab", "Ptr", this.Ptr, "Ptr", l := Buffer(8, 0)  , "Ptr", a := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", t := Buffer(4, 0)), {L: NumGet(l, "Double"), A: NumGet(a, "Double"), B: NumGet(b, "Double"), T: NumGet(t, "Int")})

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
     * @returns {Object} An object containing L (0 to 100), C (0 to 100), H (0 to 360), and A (0 to 255) values.
     */
    ToLCHab() => (DllCall("Color\ColorToLCHab", "Ptr", this.Ptr, "Ptr", l := Buffer(8, 0), "Ptr", c := Buffer(8, 0), "Ptr", h := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {L: NumGet(l, "Double"), C: NumGet(c, "Double"), H: NumGet(h, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to YPbPr format.
     * @returns {Object} An object containing Y (0 to 1), Cb (0 to 1), Cr (0 to 1), and A (0 to 255) values.
     */
    ToYPbPr() => (DllCall("Color\ColorToYPbPr", "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0), "Ptr", cb := Buffer(8, 0), "Ptr", cr := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Y: NumGet(y, "Double"), Cb: NumGet(cb, "Double"), Cr: NumGet(cr, "Double"), A: NumGet(a, "Int")})

    ToYCbCr(yCbCrType := Color.YCbCrType.BT709) => (DllCall("Color\ColorToYCbCr", "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0), "Ptr", cb := Buffer(8, 0), "Ptr", cr := Buffer(8, 0), "Ptr", a := Buffer(4, 0), "Int", yCbCrType, "Cdecl"), {Y: NumGet(y, "Double"), Cb: NumGet(cb, "Double"), Cr: NumGet(cr, "Double"), A: NumGet(a, "Int")})

    ToYCgCo() => (DllCall("Color\ColorToYCgCo", "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0), "Ptr", cg := Buffer(8, 0), "Ptr", co := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Y: NumGet(y, "Double"), Cg: NumGet(cg, "Double"), Co: NumGet(co, "Double"), A: NumGet(a, "Int")})

    ToYcCbcCrc() => (DllCall("Color\ColorToYcCbcCrc", "Ptr", this.Ptr, "Ptr", yc := Buffer(8, 0), "Ptr", cbc := Buffer(8, 0), "Ptr", crc := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Yc: NumGet(yc, "Double"), Cbc: NumGet(cbc, "Double"), Crc: NumGet(crc, "Double"), A: NumGet(a, "Int")})

    ToYDbDr() => (DllCall("Color\ColorToYDbDr", "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0), "Ptr", db := Buffer(8, 0), "Ptr", dr := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Y: NumGet(y, "Double"), Db: NumGet(db, "Double"), Dr: NumGet(dr, "Double"), A: NumGet(a, "Int")})

    ToYUV() => (DllCall("Color\ColorToYUV", "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0), "Ptr", u := Buffer(8, 0), "Ptr", v := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Y: NumGet(y, "Double"), U: NumGet(u, "Double"), V: NumGet(v, "Double"), A: NumGet(a, "Int")})

    ToYES() => (DllCall("Color\ColorToYES", "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0), "Ptr", e := Buffer(8, 0), "Ptr", s := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Y: NumGet(y, "Double"), E: NumGet(e, "Double"), S: NumGet(s, "Double"), A: NumGet(a, "Int")})

    ToJPEG() => (DllCall("Color\ColorToJPEG", "Ptr", this.Ptr, "Ptr", y := Buffer(8, 0), "Ptr", cb := Buffer(8, 0), "Ptr", cr := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {Y: NumGet(y, "Double"), Cb: NumGet(cb, "Double"), Cr: NumGet(cr, "Double"), A: NumGet(a, "Int")})

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

    ToCMY() => (DllCall("Color\ColorToCMY", "Ptr", this.Ptr, "Ptr", c := Buffer(8, 0), "Ptr", m := Buffer(8, 0), "Ptr", y := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {C: NumGet(c, "Double"), M: NumGet(m, "Double"), Y: NumGet(y, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to CMYK format.
     * @returns {Object} An object containing C, M, Y, K, and A values.
     */
    ToCMYK() => (DllCall("Color\ColorToCMYK"   , "Ptr", this.Ptr, "Ptr", c := Buffer(8, 0)  , "Ptr", m := Buffer(8, 0), "Ptr", y := Buffer(8, 0), "Ptr", k := Buffer(8, 0) , "Ptr", a := Buffer(4, 0)), {C: NumGet(c, "Double"), M: NumGet(m, "Double"), Y: NumGet(y, "Double"), K: NumGet(k, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the Color to Duv representation.
     * @returns {Float}
     */
    ToDuv() => DllCall("Color\ColorToDuv", "Ptr", this.Ptr, "Double")

    /**
     * Converts the Color to Temperature (°K) representation.
     * @returns {Float}
     */
    ToTemp() => DllCall("Color\ColorToTemp", "Ptr", this.Ptr, "Double")

    /**
     * Converts the color to Rec.2020 format.
     * @returns {Object} An object containing r, g, b (0 to 1), and A (0 to 255) values.
     */
    ToRec2020() => (DllCall("Color\ColorToRec2020", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {r: NumGet(r, "Double"), g: NumGet(g, "Double"), b: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to Display-P3 format.
     * @returns {Object} An object containing r, g, b (0 to 1), and A (0 to 255) values.
     */
    ToDisplayP3() => (DllCall("Color\ColorToDisplayP3", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {r: NumGet(r, "Double"), g: NumGet(g, "Double"), b: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to OKLab format.
     * @returns {Object} An object containing L, a, b, and A values.
     */
    ToOKLab() => (DllCall("Color\ColorToOKLab", "Ptr", this.Ptr, "Ptr", l := Buffer(8, 0), "Ptr", a := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", t := Buffer(4, 0)), {L: NumGet(l, "Double"), a: NumGet(a, "Double"), b: NumGet(b, "Double"), T: NumGet(t, "Int")})

    /**
     * Converts the color to OKLCH format.
     * @returns {Object} An object containing L, C, H, and A values.
     */
    ToOKLCH() => (DllCall("Color\ColorToOKLCH", "Ptr", this.Ptr, "Ptr", l := Buffer(8, 0), "Ptr", c := Buffer(8, 0), "Ptr", h := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {L: NumGet(l, "Double"), C: NumGet(c, "Double"), H: NumGet(h, "Double"), A: NumGet(a, "Int")})

    /**
     * Converts the color to ACEScg color space.
     * @returns {Object} Object containing R, G, B, A values
     */
    ToACEScg() => (DllCall("Color\ColorToACEScg", "Ptr", this.Ptr, "Ptr", r := Buffer(8, 0), "Ptr", g := Buffer(8, 0), "Ptr", b := Buffer(8, 0), "Ptr", a := Buffer(4, 0)), {R: NumGet(r, "Double"), G: NumGet(g, "Double"), B: NumGet(b, "Double"), A: NumGet(a, "Int")})

    /**
     * Generates an analogous color scheme.
     * @returns {Color[]} An array of Color objects representing the color scheme.
     */
    Analogous(angle := 30, count := 3)
    {
        pColors := DllCall("Color\ColorAnalogous", "Ptr", this.Ptr, "Double", angle, "Int", count, "Ptr", Buffer(count * A_PtrSize, 0), "Ptr")

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
        pColors := DllCall("Color\ColorTriadic", "Ptr", this.Ptr, "Double", angle, "Ptr", Buffer(3 * A_PtrSize, 0), "Ptr")

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
        pColors := DllCall("Color\ColorTetradic", "Ptr", this.Ptr, "Double", angle, "Ptr", Buffer(4 * A_PtrSize, 0), "Ptr")

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
        pColors := DllCall("Color\ColorMonochromatic", "Ptr", this.Ptr, "Int", count, "Ptr", Buffer(count * A_PtrSize, 0), "Ptr")

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

    /**
     * Applies a ColorMatrix to this Color.
     * @param matrix The `ColorMatrix` to apply to the Color.
     * @returns {Color}
     */
    ApplyMatrix(matrix) => Color.FromPtr(DllCall("Color\ColorApplyMatrix", "Ptr", this.Ptr, "Ptr", matrix.Ptr, "Ptr"))

    /**
     * Defines default format strings for various color representations.
     */
    class DefaultFormatString
    {
        /**
         * Default format string for RGB color representation.
         * @type {string}
         */
        static RGB := "rgba({R}, {G}, {B}, {A})"

        /**
         * Default format string for Linear sRGB color representation.
         * @type {string}
         */
        static LinearSRGB := "srgb({R:2}, {G:2}, {B:2})"

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
         * Default format string for XYZ_D50 color representation.
         * @type {string}
         */
        static XYZ_D50  := "xyz_d50({X:2}, {Y:2}, {Z:2})"

        /**
         * Default format string for XYZ_D65 color representation.
         * @type {string}
         */
        static XYZ_D65  := "xyz_d65({X}, {Y}, {Z})"

        static UCS := "ucs({U}, {C}, {S})"

        static UVW := "uvw({U}, {V}, {W})"

        /**
         * Default format string for XYY color representation.
         * @type {string}
         */
        static XYY := "xyy({X:2}, {Y1:2}, {Y2})"

        /**
         * Default format string for Lab color representation.
         * @type {string}
         */
        static Lab  := "lab({L}, {a}, {b})"

        /**
         * Default format string for NCol color representation.
         * @type {string}
         */
        static NCol := "ncol({H}, {W}%, {B}%)"

        /**
         * Default format string for CMY color representation.
         * @type {string}
         */
        static CMY := "cmy({C}, {M}, {Y})"

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

        /**
         * Default format string for HSI color representation.
         * @type {string}
         */
        static HSI := "hsi({H}, {S}%, {I})"

        /**
         * Default format string for HSP color representation.
         * @type {string}
         */
        static HSP := "hsp({H}, {S}%, {P})"

        /**
         * Default format string for HCY color representation.
         * @type {string}
         */
        static HCY := "hcy({H}, {C}, {Y})"

        /**
         * Default format string for HCG color representation.
         * @type {string}
         */
        static HCG := "hcg({H}, {C}, {G})"

        /**
         * Default format string for TSL color representation.
         * @type {string}
         */
        static TSL := "tsl({T:2}, {S:2}, {L:2})"

        /**
         * Default format string for YPbPr color representation.
         * @type {string}
         */
        static YPbPr := "ypbpr({Y:2}, {Pb:2}, {Pr:2})"

        /**
         * Default format string for YCbCr color representation.
         * @type {string}
         */
        static YCbCr := "ycbcr({Y}, {Cb}, {Cr})"

        /**
         * Default format string for YCgCo color representation.
         * @type {string}
         */
        static YCgCo := "ycgco({Y:2}, {Cg:2}, {Co:2})"

        /**
         * Default format string for YcCbcCrc color representation.
         * @type {string}
         */
        static YcCbcCrc := "yccbccrc({Yc:2}, {Cbc:2}, {Crc:2})"

        /**
         * Default format string for YDbDr color representation.
         * @type {string}
         */
        static YDbDr := "ydbdr({Y:2}, {Db:2}, {Dr:2})"

        /**
         * Default format string for YUV color representation.
         * @type {string}
         */
        static YUV := "yuv({Y:2}, {U:2}, {V:2})"

        /**
         * Default format string for YES color representation.
         * @type {string}
         */
        static YES := "yes({Y:2}, {E:2}, {S:2})"

        static JPEG := "jpeg({Y}, {Cb}, {Cr})"

        /**
         * Default format string for YIQ color representation.
         * @type {string}
         */
        static YIQ  := "yiq({Y:2}, {I:2}, {Q:2})"

        /**
         * Default format string for LCH color representation.
         * @type {string}
         */
        static LCHab := "lchab({L}, {C}, {H})"

        /**
         * Default format string for LCHuv color representation.
         * @type {string}
         */
        static LCHuv := "lchuv({L}, {C}, {H})"

        /**
         * Default format string for Adobe RGB color representation.
         * @type {string}
         */
        static AdobeRGB := "adobe({R:2}, {G:2}, {B:2})"

        /**
         * Default format string for ProPhoto RGB color representation.
         * @type {string}
         */
        static ProPhotoRGB := "prophoto({R:2}, {G:2}, {B:2})"

        /**
         * Default format string for CIE Luv color representation.
         * @type {string}
         */
        static Luv := "luv({L}, {u}, {v})"

        /**
         * Default format string for Rec2020 color representation.
         * @type {string}
         */
        static Rec2020 := "rec2020({R:2}, {G:2}, {B:2})"

        /**
         * Default format string for DisplayP3 color representation.
         * @type {string}
         */
        static DisplayP3 := "displayp3({R:2}, {G:2}, {B:2})"

        /**
         * Default format string for OKLab color representation.
         * @type {string}
         */
        static OKLab := "oklab({L:2}, {a:2}, {b:2})"

        /**
         * Default format string for OKLCH color representation.
         * @type {string}
         */
        static OKLCH := "oklch({L:2}, {C:2}, {H})"

        static ACEScg := "acescg({R:2}, {G:2}, {B:2})"
    }

    static YCbCrType => { BT601: 0, BT709: 1, BT2020: 2 }
}

class Canvas
{
    Ptr := 0

    __New(width, height, col?)
    {
        this.Ptr := DllCall("Color\CreateCanvasFromWHC", "Int", width, "Int", height, "Ptr", (col ?? Color.Transparent).Ptr, "Ptr")
    }

    /**
     * Deletes the Canvas and frees associated resources.
     */
    __Delete() => DllCall("Color\DeleteCanvas", "Ptr", this.Ptr)

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
     * Provides enumeration functionality for the Canvas.
     * @param {number} num - The number of values to return for each iteration.
     * @returns {function} An enumerator function based on the specified number.
     * @throws {ValueError} If an invalid enumerator type is specified.
     */
    __Enum(num)
    {
        bufferSize := DllCall("Color\CanvasSize", "Ptr", this.Ptr, "Ptr")

        enumColor(&col)
        {
            static index := 0
            if (index >= bufferSize)
            {
                index := 0
                return false
            }
            col := Color.FromPtr(DllCall("Color\CanvasGetAt", "Ptr", this.Ptr, "Ptr", index, "Ptr"))
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
            DllCall("Color\CanvasGetXY", "Ptr", this.Ptr, "Ptr", index, "Int*", &x := 0, "Int*", &y := 0)
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
            DllCall("Color\CanvasGetXY", "Ptr", this.Ptr, "Ptr", index, "Int*", &x := 0, "Int*", &y := 0)
            col := Color.FromPtr(DllCall("Color\CanvasGetAt", "Ptr", this.Ptr, "Ptr", index, "Ptr"))
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
     * Gets the width of the Canvas.
     * @returns {number}
     */
    Width
    {
        get => DllCall("Color\GetCanvasWidth", "Ptr", this.Ptr, "Int")
    }

    /**
     * Gets the height of the Canvas.
     * @returns {number}
     */
    Height
    {
        get => DllCall("Color\GetCanvasHeight", "Ptr", this.Ptr, "Int")
    }

    /**
     * Gets the total number of pixels in the Canvas.
     * @returns {number}
     */
    Size
    {
        get => DllCall("Color\GetCanvasSize", "Ptr", this.Ptr, "Int")
    }

    Stride
    {
        get => DllCall("Color\GetCanvasStride", "Ptr", this.Ptr, "Int")
    }

    /**
     * Creates a Canvas from a BitmapBuffer object.
     * @static
     * @param {ImagePut.BitmapBuffer} bBuffer - The BitmapBuffer object to create the Canvas from.
     * @returns {Canvas}
     */
    static FromBitmapBuffer(bBuffer)
    {
        if Type(bBuffer) == "ImagePut.BitmapBuffer"
            return Canvas.FromPtr(DllCall("Color\CreateCanvasFromBuffer", "Ptr", bBuffer.Ptr, "Int", bBuffer.Width, "Int", bBuffer.Height, "Ptr"))
        else
            throw ValueError("Invalid BitmapBuffer object or incorrect type.")
    }

    /**
     * Creates a Canvas from a 2D array of Color objects.
     * @static
     * @param {Color[[],[],...]} colors - A 2D array of Color objects.
     * @returns {Canvas}
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

            this.Ptr := DllCall("Color\CreateCanvasFromArray", "Ptr", colorArray.Ptr, "Int", width, "Int", height, "Ptr")
        }
    }

    /**
     * Exports the Canvas as an HBITMAP.
     * @param {number} [width=0] - The desired width of the HBITMAP. If 0, uses the Canvas's width.
     * @param {number} [height=0] - The desired height of the HBITMAP. If 0, uses the Canvas's height.
     * @returns {Ptr} The HBITMAP handle of the exported image.
     */
    ToHBITMAP(width := 0, height := 0) => DllCall("Color\ExportCanvasAsHBITMAP", "Ptr", this.Ptr, "Int", width, "Int", height, "Ptr")

    /**
     * Creates a Canvas from an HBITMAP handle.
     * @param {Ptr} hBitmap - The HBITMAP handle.
     * @returns {Canvas} A new Canvas instance created from the HBITMAP.
     */
    static FromHBITMAP(hBitmap, width := 0, height := 0) => Canvas.FromPtr(DllCall("Color\CreateCanvasFromHBITMAP", "Ptr", hBitmap, "Int", width, "Int", height, "Ptr"))

    /**
     * Exports the Canvas as an HDC.
     * @param {number} [width=0] - The desired width of the HDC. If 0, uses the Canvas's width.
     * @param {number} [height=0] - The desired height of the HDC. If 0, uses the Canvas's height.
     * @returns {Ptr} The HDC handle of the exported image.
     */
    ToHDC(width := 0, height := 0) => DllCall("Color\ExportCanvasAsHDC", "Ptr", this.Ptr, "Int", width, "Int", height, "Ptr")

    /**
     * Creates a Canvas from an HDC handle.
     * @static
     * @param {Ptr} hdc - The HDC handle.
     * @param {number} x - The x-coordinate of the region to capture.
     * @param {number} y - The y-coordinate of the region to capture.
     * @param {number} width - The width of the region to capture.
     * @param {number} height - The height of the region to capture.
     * @returns {Canvas} A new Canvas instance created from the HDC region.
     */
    static FromHDC(hdc, x := 0, y := 0, width := 0, height := 0) => Canvas.FromPtr(DllCall("Color\CreateCanvasFromHDC", "Ptr", hdc, "Int", x, "Int", y, "Int", width, "Int", height, "Ptr"))

    /**
     * Exports the Canvas as an HICON.
     * @param {number} [width=0] - The desired width of the HICON. If 0, uses the Canvas's width.
     * @param {number} [height=0] - The desired height of the HICON. If 0, uses the Canvas's height.
     * @returns {Ptr} The HICON handle of the exported image.
     */
    ToHICON(width := 0, height := 0) => DllCall("Color\ExportCanvasAsHICON", "Ptr", this.Ptr, "Int", width, "Int", height, "Ptr")

    /**
     * Creates a Canvas from an HICON handle.
     * @static
     * @param {Ptr} hIcon - The HICON handle.
     * @returns {Canvas} A new Canvas instance created from the HICON.
     */
    static FromHICON(hIcon) => Canvas.FromPtr(DllCall("Color\CreateCanvasFromHICON", "Ptr", hIcon, "Ptr"))

    /**
     * Exports the Canvas as an HCURSOR.
     * @param {number} [width=0] - The desired width of the HCURSOR. If 0, uses the Canvas's width.
     * @param {number} [height=0] - The desired height of the HCURSOR. If 0, uses the Canvas's height.
     * @returns {Ptr} The HCURSOR handle of the exported image.
     */
    ToHCURSOR(width := 0, height := 0) => DllCall("Color\ExportCanvasAsHCURSOR", "Ptr", this.Ptr, "Int", width, "Int", height, "Ptr")

    /**
     * Creates a Canvas from an HCURSOR handle.
     * @static
     * @param {Ptr} hCursor - The HCURSOR handle.
     * @returns {Canvas} A new Canvas instance created from the HCURSOR.
     */
    static FromHCURSOR(hCursor) => Canvas.FromPtr(DllCall("Color\CreateCanvasFromHCURSOR", "Ptr", hCursor, "Ptr"))

    static FromHWND(hWnd, x := 0, y := 0, width := 0, height := 0) => Canvas.FromPtr(DllCall("Color\CreateCanvasFromHWND", "Ptr", hWnd, "Int", x, "Int", y, "Int", width, "Int", height, "Ptr"))

    static FromGradient(grad, width, height) => Canvas.FromPtr(DllCall("Color\CreateCanvasFromGradient", "Ptr", grad.Ptr, "Int", width, "Int", height, "Ptr"))

    /**
     * Resizes the Canvas to the specified dimensions.
     * @param {number} width - The new width.
     * @param {number} height - The new height.
     * @returns {Canvas}
     */
    Resize(width, height, resizeImage := true, fillColor := Color.Transparent) => (DllCall("Color\ResizeCanvas", "Ptr", this.Ptr, "Int", width, "Int", height, "Int", resizeImage, "Ptr", fillColor.Ptr), this)

    Scale(scale) => (DllCall("Color\ScaleCanvas", "Ptr", this.Ptr, "Double", scale), this)

    /**
     * Rotates the Canvas by the specified angle.
     * @param {number} angle - The rotation angle in degrees.
     * @returns {Canvas}
     */
    Rotate(angle) => (DllCall("Color.dll\RotateCanvas", "Ptr", this.Ptr, "Double", angle), this)

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
     * @returns {Canvas}
     */
    SetColor(x, y, color) => (DllCall("Color\SetColorInBuffer", "Ptr", this.Ptr, "Int", x, "Int", y, "Ptr", color.Ptr), this)

    /**
     * Shifts the red component of all colors in the buffer by the specified amount.
     * @param {number} amount - The amount to shift the red component.
     * @returns {Canvas}
     */
    ShiftRed(amount) => (DllCall("Color\ShiftRedCanvas", "Ptr", this.Ptr, "Int", amount), this)

    /**
     * Shifts the green component of all colors in the buffer by the specified amount.
     * @param {number} amount - The amount to shift the green component.
     * @returns {Canvas}
     */
    ShiftGreen(amount) => (DllCall("Color\ShiftGreenCanvas", "Ptr", this.Ptr, "Int", amount), this)

    /**
     * Shifts the blue component of all colors in the buffer by the specified amount.
     * @param {number} amount - The amount to shift the blue component.
     * @returns {Canvas}
     */
    ShiftBlue(amount) => (DllCall("Color\ShiftBlueCanvas", "Ptr", this.Ptr, "Int", amount), this)

    /**
     * Shifts the alpha component of all colors in the buffer by the specified amount.
     * @param {number} amount - The amount to shift the alpha component.
     * @returns {Canvas}
     */
    ShiftAlpha(amount) => (DllCall("Color\ShiftAlphaCanvas", "Ptr", this.Ptr, "Int", amount), this)

    /**
     * Sets the red component of all colors in the buffer to the specified value.
     * @param {number} value - The value to set for the red component.
     * @returns {Canvas}
     */
    SetRed(value) => (DllCall("Color\SetRedCanvas", "Ptr", this.Ptr, "Int", value), this)

    /**
     * Sets the green component of all colors in the buffer to the specified value.
     * @param {number} value - The value to set for the green component.
     * @returns {Canvas}
     */
    SetGreen(value) => (DllCall("Color\SetGreenCanvas", "Ptr", this.Ptr, "Int", value), this)

    /**
     * Sets the blue component of all colors in the buffer to the specified value.
     * @param {number} value - The value to set for the blue component.
     * @returns {Canvas}
     */
    SetBlue(value) => (DllCall("Color\SetBlueCanvas", "Ptr", this.Ptr, "Int", value), this)

    /**
     * Sets the alpha component of all colors in the buffer to the specified value.
     * @param {number} value - The value to set for the alpha component.
     * @returns {Canvas}
     */
    SetAlpha(value) => (DllCall("Color\SetAlphaCanvas", "Ptr", this.Ptr, "Int", value), this)

    ApplyMatrix(matrix) => (DllCall("Color\CanvasApplyMatrix", "Ptr", this.Ptr, "Ptr", matrix.Ptr), this)

    /**
     * Inverts all colors in the buffer.
     * @returns {Canvas}
     */
    Invert() => (DllCall("Color\InvertCanvas", "Ptr", this.Ptr), this)

    /**
     * Applies the complement effect to all colors in the buffer.
     * @returns {Canvas}
     */
    Complement() => (DllCall("Color\ComplementCanvas", "Ptr", this.Ptr), this)

    /**
     * Shifts the hue of all colors in the buffer by the specified degrees.
     * @param {number} degrees - The amount to shift the hue in degrees.
     * @returns {Canvas}
     */
    ShiftHue(degrees) => (DllCall("Color\ShiftHueCanvas", "Ptr", this.Ptr, "Double", degrees), this)

    /**
     * Converts all colors in the buffer to grayscale.
     * @returns {Canvas}
     */
    Grayscale() => (DllCall("Color\GrayscaleCanvas", "Ptr", this.Ptr), this)

    /**
     * Applies a sepia tone effect to all colors in the buffer.
     * @returns {Canvas}
     */
    Sepia(factor) => (DllCall("Color\SepiaCanvas", "Ptr", this.Ptr, "Double", factor), this)

    CrossProcess(factor) => (DllCall("Color\CrossProcessCanvas", "Ptr", this.Ptr, "Double", factor), this)

    Moonlight(factor) => (DllCall("Color\MoonlightCanvas", "Ptr", this.Ptr, "Double", factor), this)

    VintageFilm(factor) => (DllCall("Color\VintageFilmCanvas", "Ptr", this.Ptr, "Double", factor), this)

    Technicolor(factor) => (DllCall("Color\TechnicolorCanvas", "Ptr", this.Ptr, "Double", factor), this)

    Polaroid(factor) => (DllCall("Color\PolaroidCanvas", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Increases the saturation of all colors in the buffer.
     * @param {number} amount - The amount to increase saturation.
     * @returns {Canvas}
     */
    ShiftSaturation(amount) => (DllCall("Color\ShiftSaturationCanvas", "Ptr", this.Ptr, "Double", amount), this)

    /**
     * Lightens all colors in the buffer.
     * @param {number} amount - The amount to lighten the colors.
     * @returns {Canvas}
     */
    ShiftLightness(amount) => (DllCall("Color\ShiftLightnessCanvas", "Ptr", this.Ptr, "Double", amount), this)

    ShiftValue(amount) => (DllCall("Color\ShiftValueCanvas", "Ptr", this.Ptr, "Double", amount), this)

    ShiftIntensity(amount) => (DllCall("Color\ShiftIntensityCanvas", "Ptr", this.Ptr, "Double", amount), this)

    ShiftWhiteLevel(amount) => (DllCall("Color\ShiftWhiteLevelCanvas", "Ptr", this.Ptr, "Double", amount), this)

    ShiftBlackLevel(amount) => (DllCall("Color\ShiftBlackLevelCanvas", "Ptr", this.Ptr, "Double", amount), this)

    ShiftContrast(amount) => (DllCall("Color\ShiftContrastCanvas", "Ptr", this.Ptr, "Double", amount), this)

    /**
     * Applies a pixelation effect to the buffer.
     * @param {number} pixelSize - The size of each "pixel" in the pixelation effect.
     * @returns {Canvas}
     */
    Pixelate(pixelSize) => (DllCall("Color.dll\PixelateCanvas", "Ptr", this.Ptr, "Int", pixelSize), this)

    /**
     * Applies a blur effect to the buffer.
     * @param {number} radius - The radius of the blur effect.
     * @returns {Canvas}
     */
    Blur(radius) => (DllCall("Color.dll\BlurCanvas", "Ptr", this.Ptr, "Int", radius), this)

    GaussianBlur(sigma) => (DllCall("Color.dll\GaussianBlurCanvas", "Ptr", this.Ptr, "Float", sigma), this)

    /**
     * Applies a sharpening effect to the buffer.
     * @param {number} amount - The intensity of the sharpening effect.
     * @returns {Canvas}
     */
    Sharpen(amount) => (DllCall("Color.dll\SharpenCanvas", "Ptr", this.Ptr, "Float", amount), this)

    /**
     * Adjusts the contrast of all colors in the buffer.
     * @param {number} factor - The contrast adjustment factor.
     * @returns {Canvas}
     */
    AdjustContrast(factor) => (DllCall("Color\AdjustContrastCanvas", "Ptr", this.Ptr, "Double", factor), this)

    /**
     * Adjusts the color balance of the buffer.
     * @param {number} redFactor - The adjustment factor for the red channel.
     * @param {number} greenFactor - The adjustment factor for the green channel.
     * @param {number} blueFactor - The adjustment factor for the blue channel.
     * @returns {Canvas}
     */
    ColorBalance(redFactor, greenFactor, blueFactor) => (DllCall("Color\AdjustColorBalanceCanvas", "Ptr", this.Ptr, "Double", redFactor, "Double", greenFactor, "Double", blueFactor), this)

    /**
     * Overlays another Canvas onto this one.
     * @param {Canvas} overlayBuffer - The Canvas to overlay.
     * @param {number} x - The x-coordinate to place the overlay.
     * @param {number} y - The y-coordinate to place the overlay.
     * @param {number} opacity - The opacity of the overlay (0 to 255).
     * @returns {Canvas}
     */
    OverlayCanvas(overlayBuffer, x, y, opacity := 1.0) => (DllCall("Color\OverlayImageCanvas", "Ptr", this.Ptr, "Ptr", overlayBuffer.Ptr, "Int", x, "Int", y, "Double", opacity), this)

    /**
     * Applies an emboss effect to the buffer.
     * @param {number} [depth=1.0] - The depth of the emboss effect.
     * @param {number} [angle=45.0] - The angle of the emboss effect in degrees.
     * @returns {Canvas}
     */
    Emboss(depth := 1.0, angle := 45.0) => (DllCall("Color\ApplyEmbossCanvas", "Ptr", this.Ptr, "Double", depth, "Double", angle), this)

    /**
     * Applies an edge detection effect to the buffer.
     * @returns {Canvas}
     */
    EdgeDetect() => (DllCall("Color\ApplyEdgeDetectCanvas", "Ptr", this.Ptr), this)

    /**
     * Flips the buffer horizontally or vertically.
     * @param {boolean} [horizontal=true] - If true, flips horizontally; if false, flips vertically.
     * @returns {Canvas}
     */
    Flip(horizontal := true) => (DllCall("Color.dll\FlipCanvas", "Ptr", this.Ptr, "Int", horizontal), this)

    /**
     * Crops the buffer to the specified dimensions.
     * @param {number} x - The x-coordinate of the top-left corner of the crop area.
     * @param {number} y - The y-coordinate of the top-left corner of the crop area.
     * @param {number} width - The width of the crop area.
     * @param {number} height - The height of the crop area.
     * @returns {Canvas}
     */
    Crop(x, y, width, height) => (DllCall("Color.dll\CropCanvas", "Ptr", this.Ptr, "Int", x, "Int", y, "Int", width, "Int", height), this)

    /**
     * Calculates the average color of the entire buffer.
     * @returns {Color}
     */
    AverageColor() => Color.FromPtr((DllCall("Color\AverageCanvas", "Ptr", this.Ptr, "Ptr")))

    /**
     * Applies a vignette effect to the buffer.
     * @param {number} [strength=0.3] - The strength of the vignette effect (0.0 to 1.0).
     * @param {number} [radius=1.0] - The radius of the vignette effect.
     * @returns {Canvas} The current Canvas instance (for method chaining).
     */
    Vignette(strength := 0.3, radius := 1.0) => (DllCall("Color.dll\ApplyVignetteCanvas", "Ptr", this.Ptr, "Double", strength, "Double", radius), this)

    /**
     * Applies a two-color noise effect to the buffer.
     * @param {number} [density=0.05] - The density of the noise (0.0 to 1.0).
     * @param {Color} [saltColor=Color.White] - The first color.
     * @param {Color} [pepperColor=Color.Black] - The second color.
     * @returns {Canvas}
     */
    TwoColorNoise(density := 0.05, saltColor := Color.White, pepperColor := Color.Black) => (DllCall("Color.dll\ApplyTwoColorNoiseCanvas", "Ptr", this.Ptr, "Double", density, "Ptr", saltColor.Ptr, "Ptr", pepperColor.Ptr), this)

    /**
     * Applies Gaussian noise to the buffer.
     * @param {number} [mean=0.0] - The mean of the Gaussian distribution.
     * @param {number} [stdDev=10.0] - The standard deviation of the Gaussian distribution.
     * @returns {Canvas}
     */
    GaussianNoise(mean := 0.0, stdDev := 10.0) => (DllCall("Color.dll\ApplyGaussianNoiseCanvas", "Ptr", this.Ptr, "Double", mean, "Double", stdDev), this)

    /**
     * Applies Perlin noise to the color buffer.
     * @param {number} frequency - Controls the scale of the noise pattern. Higher values create smaller, more frequent patterns.
     * @param {number} amplitude - Determines the strength of the noise effect. Higher values create more pronounced changes.
     * @param {number} octaves - The number of noise layers to combine. More octaves add finer details but increase computation time.
     * @param {number} persistence - Controls how quickly the amplitude decreases for each octave. `0-1`, where lower values create smoother noise.
     * @param {number} lacunarity - Determines how quickly the frequency increases for each octave. Values greater than 1 increase detail in higher octaves.
     * @returns {this} The Canvas object, allowing for method chaining.
     */
    PerlinNoise(frequency := 4.0, amplitude := 1.0, octaves := 4, persistence := 0.5, lacunarity := 2.0) => (DllCall("Color.dll\ApplyPerlinNoiseCanvas", "Ptr", this.Ptr, "Double", frequency, "Double", amplitude, "Int", octaves, "Double", persistence, "Double", lacunarity), this)

    SimplexNoise(frequency := 4.0, amplitude := 1.0, octaves := 4, persistence := 0.5, lacunarity := 2.0) => (DllCall("Color.dll\ApplySimplexNoiseCanvas", "Ptr", this.Ptr, "Double", frequency, "Double", amplitude, "Int", octaves, "Double", persistence, "Double", lacunarity), this)

    FractalBrownianMotion(frequency := 4.0, amplitude := 1.0, octaves := 4, persistence := 0.5, lacunarity := 2.0) => (DllCall("Color.dll\ApplyFractalBrownianMotionCanvas", "Ptr", this.Ptr, "Double", frequency, "Double", amplitude, "Int", octaves, "Double", persistence, "Double", lacunarity), this)

    /**
     * Applies a Voronoi diagram effect to the color buffer.
     * @param {number} [numPoints=20] - The number of seed points for the Voronoi diagram. More points create a more complex cellular pattern.
     * @param {number} [falloff=1.0] - Controls the sharpness of cell edges. Higher values create sharper edges, while lower values create smoother transitions.
     * @param {number} [strength=1] - How strongly the effect is applied. Higher values create more pronounced changes.
     * @returns {this} The Canvas object, allowing for method chaining.
     */
    VoronoiDiagram(points := 20, falloff := 1, strength := 1) => (DllCall("Color.dll\ApplyVoronoiDiagramCanvas", "Ptr", this.Ptr, "Int", points, "Double", falloff, "Double", strength), this)

    /**
     * Applies a static Plasma effect to the color buffer, creating vibrant, swirling patterns.
     * @param {number} [frequency=5.0] - Controls the density of the plasma pattern. Higher values create more intricate patterns.
     * @param {number} [phase=0.0] - Adjusts the overall pattern shift. Different values create unique variations of the pattern.
     * @returns {this} The Canvas object, allowing for method chaining.
     */
    Plasma(frequency := 5, phase := 0) => (DllCall("Color.dll\PlasmaEffectCanvas", "Ptr", this.Ptr, "Double", frequency, "Double", phase), this)

    DiamondSquare(roughness := 5, waterLevel := 1, levelsPerStop := 5) => (DllCall("Color.dll\DiamondSquareEffectCanvas", "Ptr", this.Ptr, "Double", roughness, "Double", waterLevel, "Double", levelsPerStop), this)

    Posterize(levels := 4) => (DllCall("Color.dll\PosterizeCanvas", "Ptr", this.Ptr, "Int", levels), this)

    /**
     * Creates a deep copy of the Canvas.
     * @returns {Canvas} A new Canvas instance that is a copy of the current one.
     */
    Copy() => Canvas.FromPtr(DllCall("Color\CopyCanvas", "Ptr", this.Ptr, "Ptr"))

    /**
     * Creates a copy of a specific region of the Canvas.
     * @param {number} x - The x-coordinate of the top-left corner of the region.
     * @param {number} y - The y-coordinate of the top-left corner of the region.
     * @param {number} w - The width of the region.
     * @param {number} h - The height of the region.
     * @returns {Canvas} A new Canvas instance containing the specified region.
     */
    CopyRegion(x, y, w, h) => Canvas.FromPtr(DllCall("Color\CopyCanvasRegion", "Ptr", this.Ptr, "Int", x, "Int", y, "Int", w, "Int", h, "Ptr"))

    /**
     * Applies a mapping function to a specific region of the Canvas.
     * @param {number} x - The x-coordinate of the top-left corner of the region.
     * @param {number} y - The y-coordinate of the top-left corner of the region.
     * @param {number} width - The width of the region.
     * @param {number} height - The height of the region.
     * @param {function} mapFunction - A function that takes (x, y, Color) as arguments and returns a new Color.
     * @returns {Canvas}
     */
    MapRegion(x, y, width, height, mapFunction)
    {
        callbackPtr := CallbackCreate((x, y, c) => mapFunction(x, y, Color(c)).ToInt(), "F", 3)
        DllCall("Color\MapColorsInBuffer", "Ptr", this.Ptr, "Int", x, "Int", y, "Int", width, "Int", height, "Ptr", callbackPtr)
        CallbackFree(callbackPtr)
        return this
    }

    /**
     * Applies a mapping function to the entire Canvas.
     * @param {function} mapFunction - A function that takes (x, y, Color) as arguments and returns a new Color.
     * @returns {Canvas}
     */
    Map(mapFunction) => this.MapRegion(0, 0, this.Width, this.Height, mapFunction)

    ForEach(func) => (callbackPtr := CallbackCreate(func), DllCall("Color\CanvasForEach", "Ptr", this.Ptr, "Ptr", callbackPtr), CallbackFree(callbackPtr), this)

    Find(color) => DllCall("Color\CanvasFind", "Ptr", this.Ptr, "Ptr", color.Ptr, "Int")

    FindLast(color) => DllCall("Color\CanvasFindLast", "Ptr", this.Ptr, "Ptr", color.Ptr, "Int")

    FindAll(color)
    {
        countPtr := Buffer(4, 0)
        resultPtr := DllCall("Color\CanvasFindAll", "Ptr", this.Ptr, "Ptr", color.Ptr, "Ptr", countPtr, "Ptr")
        count := NumGet(countPtr, 0, "Int")
        result := []

        Loop count
            result.Push(NumGet(resultPtr + (A_Index - 1) * 4, "Int"))

        DllCall("Color\FreeIntArray", "Ptr", resultPtr)
        return result
    }

    Swap(index1, index2) => (DllCall("Color\CanvasSwap", "Ptr", this.Ptr, "Int", index1, "Int", index2), this)

    Filter(predicate) => (callbackPtr := CallbackCreate(predicate), resultPtr := DllCall("Color\CanvasFilter", "Ptr", this.Ptr, "Ptr", callbackPtr, "Ptr"), CallbackFree(callbackPtr), Canvas.FromPtr(resultPtr))

    Count(color) => DllCall("Color\CanvasCount", "Ptr", this.Ptr, "Ptr", color.Ptr, "Int")

    CountUnique() => DllCall("Color\CanvasCountUniqueColors", "Ptr", this.Ptr, "Int")

    Shuffle() => (DllCall("Color\CanvasShuffle", "Ptr", this.Ptr), this)

    Clear() => (DllCall("Color\CanvasClear", "Ptr", this.Ptr), this)

    Sort(compare) => (callbackPtr := CallbackCreate(compare), DllCall("Color\CanvasSort", "Ptr", this.Ptr, "Ptr", callbackPtr), CallbackFree(callbackPtr), this)

    AppendRight(other) => (DllCall("Color\CanvasAppendRight", "Ptr", this.Ptr, "Ptr", other.Ptr), this)

    AppendBottom(other) => (DllCall("Color\CanvasAppendBottom", "Ptr", this.Ptr, "Ptr", other.Ptr), this)

    /**
     * Converts a linear index to x and y coordinates.
     * @param {number} index - The linear index to convert.
     * @param {number} &x - Output parameter for the x-coordinate.
     * @param {number} &y - Output parameter for the y-coordinate.
     * @returns {Canvas}
     */
    XYFromIndex(index, &x, &y) => (DllCall("Color\CanvasGetXY", "Ptr", this.Ptr, "Int", index, "Int*", &x, "Int*", &y), this)

    /**
     * Converts x and y coordinates to a linear index.
     * @param {number} x - The x-coordinate.
     * @param {number} y - The y-coordinate.
     * @param {number} `&index` - Output parameter for the linear index.
     * @returns {Canvas}
     */
    IndexFromXY(x, y, &index) => (DllCall("Color\CanvasGetIndex", "Ptr", this.Ptr, "Int", x, "Int", y, "Int*", &index), this)

    Draw(hwnd, x, y) => (DllCall("Color\DrawCanvas", "Ptr", this.Ptr, "Ptr", hwnd, "Int", x, "Int", y), this)

    /**
     * Creates a Canvas instance from a pointer.
     * @param {Ptr} Ptr - The pointer to the Canvas data.
     * @returns {Canvas} A new Canvas instance.
     */
    static FromPtr(Ptr) => {base: Canvas.Prototype, Ptr: Ptr}
}

class Gradient
{
    static Type := { Linear: 0, Radial: 1, Conical: 2 }

    /**
     * Gets or sets the total number of steps in the gradient.
     * @example
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.TotalSteps := 30
     */
    TotalSteps
    {
        get => DllCall("Color\GradientGetTotalSteps", "Ptr", this.Ptr, "Int")
        set => DllCall("Color\GradientSetTotalSteps", "Ptr", this.Ptr, "Int", value)
    }

    /**
     * Gets or sets the type of Gradient: `Linear`, `Radial`, or `Conical`.
     * @example
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.Type := Gradient.Type.Radial
     */
    Type
    {
        get => DllCall("Color\GradientGetType", "Ptr", this.Ptr, "Int")
        set => DllCall("Color\GradientSetType", "Ptr", this.Ptr, "Int", value)
    }

    /**
     * Gets or sets the angle of the gradient
     * @example
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.Angle += 45 ; Increase the angle by 45°
     */
    Angle
    {
        get => DllCall("Color\GradientGetAngle", "Ptr", this.Ptr, "Float")
        set => DllCall("Color\GradientSetAngle", "Ptr", this.Ptr, "Float", value)
    }

    /**
     * Gets or sets the number of Vertices in the gradient.
     * Only affects Radial gradients.
     * @example
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.Vertices := 5
     */
    Vertices
    {
        get => DllCall("Color\GradientGetVertices", "Ptr", this.Ptr, "Int")
        set => DllCall("Color\GradientSetVertices", "Ptr", this.Ptr, "Int", value)
    }

    /**
     * Gets or sets the Edge Sharpness, which controls the transitions between the "edges" of the gradient.
     *
     * This will only have an effect when `this.Vertices` is >= 3.
     *
     * Values greater than `0` lead to "sharper" gradients, while
     * values less than `0` lead to "rounder" gradients.
     * @example
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.EdgeSharpness := 2.368
     */
    EdgeSharpness
    {
        get => DllCall("Color\GradientGetEdgeSharpness", "Ptr", this.Ptr, "Float")
        set => DllCall("Color\GradientSetEdgeSharpness", "Ptr", this.Ptr, "Float", value)
    }

    /**
     * Gets or sets the Wavelength of the gradient.
     *
     * Must be greater than or equal to `0`.
     * @example
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.Wavelength := 2
     */
    Wavelength
    {
        get => DllCall("Color\GradientGetWavelength", "Ptr", this.Ptr, "Float")
        set => DllCall("Color\GradientSetWavelength", "Ptr", this.Ptr, "Float", value)
    }

    /**
     * Gets or sets the Amplitude of the gradient.
     *
     * Must be greater than or equal to `0`.
     * @example
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.Amplitude := .125
     */
    Amplitude
    {
        get => DllCall("Color\GradientGetAmplitude", "Ptr", this.Ptr, "Float")
        set => DllCall("Color\GradientSetAmplitude", "Ptr", this.Ptr, "Float", value)
    }

    /**
     * Gets or sets the Repetitions of the gradient.
     * @example
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.Repetitions := 2
     */
    Repetitions
    {
        get => DllCall("Color\GradientGetRepetitions", "Ptr", this.Ptr, "Float")
        set => DllCall("Color\GradientSetRepetitions", "Ptr", this.Ptr, "Float", value)
    }

    /**
     * Gets or sets the Focus of the gradient.
     * Only affects Radial and Conical gradients.
     * @example
     * ; Setting
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * exGradient.Focus := { X: 200, Y: 150 } ; X and Y can not be set individually
     *
     * ; Getting
     * exGradient := Gradient(20, Color.Red, Color.Blue)
     * strX := "Focus X: " exGradient.Focus.X
     * strY := "Focus Y: " exGradient.Focus.Y
     */
    Focus
    {
        get
        {
            x := Buffer(4, 0), y := Buffer(4, 0)
            DllCall("Color\GradientGetFocus", "Ptr", this.Ptr, "Ptr", x, "Ptr", y)
            return {X: NumGet(x, "Float"), Y: NumGet(y, "Float")}
        }

        set => DllCall("Color\GradientSetFocus", "Ptr", this.Ptr, "Float", value.X, "Float", value.Y)
    }

    ColorStops
    {
        get => Gradient.ColorStopArray(this.Ptr)
    }

    /**
     * @constructor
     * @param {number} totalSteps - The total number of steps in the gradient.
     * @param {...Color} colors - The colors to initialize the gradient with.
     */
    __New(totalStepsOrGradient := 0, colors*)
    {
        if totalStepsOrGradient is Gradient  ; Copy constructor
        {
            this.Ptr := DllCall("Color\CreateGradientFromGradient", "Ptr", totalStepsOrGradient.Ptr, "Ptr")
        }
        else if colors.Length  ; Constructor with steps and Colors or ColorStops
        {
            if (colors[1] is Color) and (totalStepsOrGradient > 1)
            {
                cols := Buffer(colors.Length * 4)
                For i, col in colors
                    NumPut("UInt", col.ToInt(), cols, (i-1) * 4)

                this.Ptr := DllCall("Color\CreateGradientFromColors", "Int", totalStepsOrGradient, "Ptr", cols, "Int", colors.Length, "Ptr")
            }
            else if colors[1] is ColorStop
            {
                stops := Buffer(A_PtrSize * colors.Length)
                for i, stop in colors
                    NumPut("Ptr", stop.Ptr, stops, (i - 1) * A_PtrSize)

                this.Ptr := DllCall("Color\CreateGradientFromColorStops", "Int", totalStepsOrGradient, "Ptr", stops, "Int", colors.Length, "Ptr")
            }
        }
        else  if totalStepsOrGradient is Number ; Constructor with just steps (rainbow gradient)
        {
            if totalStepsOrGradient > 0
                this.Ptr := DllCall("Color\CreateGradientFromSteps", "Int", totalStepsOrGradient, "Ptr")
            else
                this.Ptr := DllCall("Color\CreateGradient", "Ptr")
        }
    }


    /**
     * Deletes the Gradient object and frees associated resources.
     * @returns {void}
     */
    __Delete() => DllCall("Color\DeleteGradient", "Ptr", this.Ptr)

    /**
     * Gets an interpolated Color object at the specified step.
     * @param {number} index - The index of the Color to retrieve (1-based).
     * @returns {Color} The Color object at the specified step.
     */
    __Item[index]
    {
        get
        {
            --index
            totalSteps := DllCall("Color\GradientGetTotalSteps", "Ptr", this.Ptr, "Int")

            if (index < 0) or (index >= totalSteps)
                throw Error("Index is out of bounds.")

            return this.GetColorAtStep(index)
        }
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

            col := this.GetColorAtStep(index)
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
            col := this.GetColorAtStep(index)
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
     * Adds a new color stop at the specified position.
     * @param {Color} col - The color to add at the stop.
     * @param {number} position - The position of the color stop (0 to 1).
     */
    AddColorStop(colStop) => (DllCall("Color\GradientAddColorStop", "Ptr", this.Ptr, "UInt", colStop.Color.ToInt(), "Float", colStop.Position), this)

    /**
     * @method RemoveColorStop
     * @param {number} position - The index of the ColorStop to remove (must exist).
     */
    RemoveColorStopAt(index) => (DllCall("Color\GradientRemoveColorStopAt", "Ptr", this.Ptr, "Float", index - 1), this)

    CreateColorStopFromPosition(position) => ColorStop(position, this.GetColorAtPosition(position))

    /**
     * Gets an interpolated Color object at a position along the gradient.
     * @param {number} position - The position to get the color at (0 to 1).
     * @returns {Color} The Color object at the specified position.
     */
    GetColorAtPosition(position) => Color(DllCall("Color\GradientGetColorAt", "Ptr", this.Ptr, "Float", position, "UInt"))

    /**
     * Gets an interpolated Color object at the specified step.
     * @param {number} index - The index of the Color to retrieve (1-based).
     * @returns {Color} The Color object at the specified step.
     */
    GetColorAtStep(step) => Color.FromPtr(DllCall("Color\GradientGetColorAtStep", "Ptr", this.Ptr, "Int", step, "Ptr"))

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
    static Deserialize(data) => (dataBuf := Buffer(StrPut(data, "UTF-8")), StrPut(data, dataBuf, "UTF-8"), ptr := DllCall("Color\GradientDeserialize", "Ptr", dataBuf, "Ptr"), Gradient.FromPtr(ptr))

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

    /**
     * Returns the Complements of all the Gradients colors. Self-Modifying.
     * @returns {Gradient}
     */
    Complement() => (DllCall("Color\GradientComplement", "Ptr", this.Ptr, "Ptr"), this)

    Draw(hwnd, x, y, width, height) => (DllCall("Color\DrawGradient", "Ptr", this.Ptr, "Ptr", hwnd, "Int", x, "Int", y, "Int", width, "Int", height), this)

    /**
     * Creates a Gradient instance from a pointer to existing gradient data.
     * @static
     * @param {Ptr} Ptr - A pointer to the existing gradient data.
     * @returns {Gradient} A new Gradient instance with the base set to Gradient.Prototype and the given Ptr.
     */
    static FromPtr(Ptr) => {base: Gradient.Prototype, Ptr: Ptr}

    class ColorStopArray
    {
        Ptr := 0
        Length := 0

        __New(Ptr)
        {
            this.Ptr := Ptr
            this.Length := DllCall("Color\GradientGetColorStopCount", "Ptr", this.Ptr, "Int")
        }

        /**
         * Gets or Sets a ColorStop object at the specified index.
         * @param {number} index - The index of the ColorStop to retrieve (1-based).
         * @returns {Color} The ColorStop object at the specified index.
         */
        __Item[index]
        {
            get => ColorStop.FromPtr(DllCall("Color\GradientGetColorStopAt", "Ptr", this.Ptr, "Int", index - 1, "Ptr"))
            set
            {
                if (value == "") or (value == False)
                    DllCall("Color\GradientRemoveColorStopAt", "Ptr", this.Ptr, "Int", index - 1)
                else
                    DllCall("Color\GradientSetColorStopAt", "Ptr", this.Ptr, "Int", index - 1, "Ptr", value.Ptr)
            }
        }

        /**
         * Provides enumeration functionality for the Gradient's ColorStops.
         * @param {number} num - The number of values to return for each iteration (1 or 2).
         * @returns {function} An enumerator function based on the specified number.
         * @throws {ValueError} If an invalid enumerator type is specified.
         */
        __Enum(num)
        {
            totalStops := DllCall("Color\GradientGetColorStopCount", "Ptr", this.Ptr, "Int")

            enumColorStop(&col)
            {
                static index := 0
                if (index >= totalStops)
                {
                    index := 0
                    return false
                }

                col := ColorStop.FromPtr(DllCall("Color\GradientGetColorStopAt", "Ptr", this.Ptr, "Int", index, "Ptr"))
                index++
                return true
            }

            enumIndexColorStop(&index, &col)
            {
                static currentIndex := 0
                if (currentIndex >= totalStops)
                {
                    currentIndex := 0
                    return false
                }

                index := currentIndex
                col := ColorStop.FromPtr(DllCall("Color\GradientGetColorStopAt", "Ptr", this.Ptr, "Int", index, "Ptr"))
                currentIndex++
                return true
            }

            switch num
            {
                case 1: return enumColorStop
                case 2: return enumIndexColorStop
                default: throw ValueError("Invalid enumerator type")
            }
        }
    }
}

class ColorPicker
{
    static ViewModes => { None: 0, Grid: 1, Dot: 2, Crosshair: 3 }

    __New(colorSpace := "Hex", format := "#{R}{G}{B}") => (this.Ptr := DllCall("Color\ColorPickerCreate", "AStr", colorSpace, "AStr", format, "Ptr"))
    __Delete() => DllCall("Color\ColorPickerDestroy", "Ptr", this.Ptr)

    Value
    {
        get => DllCall("Color\ColorPickerGetFormattedValue", "Ptr", this.Ptr, "AStr")
    }

    Start() => DllCall("Color\ColorPickerStart", "Ptr", this.Ptr)
    Stop() => (DllCall("Color\ColorPickerStop", "Ptr", this.Ptr), Sleep(50))
    Toggle() => (DllCall("Color\ColorPickerToggle", "Ptr", this.Ptr), (!this.IsActive() ? Sleep(50) : 0))
    IsActive() => DllCall("Color\ColorPickerIsActive", "Ptr", this.Ptr, "Int")
    GetCurrentColor() => Color.FromPtr(DllCall("Color\ColorPickerGetCurrentColor", "Ptr", this.Ptr, "Ptr"))

    SetPosition(x, y) => DllCall("Color\ColorPickerSetPosition", "Ptr", this.Ptr, "Int", x, "Int", y)
    GetPosition(&x, &y) => DllCall("Color\ColorPickerGetPosition", "Ptr", this.Ptr, "Int*", &x, "Int*", &y)
    Move(deltaX, deltaY) => DllCall("Color\ColorPickerMove", "Ptr", this.Ptr, "Int", deltaX, "Int", deltaY)
    SetPreviewSize(width, height) => DllCall("Color\ColorPickerSetPreviewSize", "Ptr", this.Ptr, "Int", width, "Int", height)
    SetCaptureSize(size) => DllCall("Color\ColorPickerSetCaptureSize", "Ptr", this.Ptr, "Int", size)
    SetZoomLevel(zoom) => DllCall("Color\ColorPickerSetZoomLevel", "Ptr", this.Ptr, "Float", zoom)
    ZoomIn(amount := 0.1) => DllCall("Color\ColorPickerZoomIn", "Ptr", this.Ptr, "Float", amount)
    ZoomOut(amount := 0.1) => DllCall("Color\ColorPickerZoomOut", "Ptr", this.Ptr, "Float", amount)
    IncreaseCaptureSize(amount := 1) => DllCall("Color\ColorPickerIncreaseCaptureSize", "Ptr", this.Ptr, "Int", amount)
    DecreaseCaptureSize(amount := 1) => DllCall("Color\ColorPickerDecreaseCaptureSize", "Ptr", this.Ptr, "Int", amount)
    SetFontSize(size) => DllCall("Color\ColorPickerSetFontSize", "Ptr", this.Ptr, "Int", size)
    SetFontName(name) => DllCall("Color\ColorPickerSetFontName", "Ptr", this.Ptr, "WStr", name)
    SetTextColor(color) => DllCall("Color\ColorPickerSetTextColor", "Ptr", this.Ptr, "Ptr", color.Ptr)
    SetTextBackgroundColor(color) => DllCall("Color\ColorPickerSetTextBackgroundColor", "Ptr", this.Ptr, "Ptr", color.Ptr)
    SetBorderColor(color) => DllCall("Color\ColorPickerSetBorderColor", "Ptr", this.Ptr, "Ptr", color.Ptr)
    SetViewModeColor(color) => DllCall("Color\ColorPickerSetViewModeColor", "Ptr", this.Ptr, "Ptr", color.Ptr)
    SetHighlightColor(color) => DllCall("Color\ColorPickerSetHighlightColor", "Ptr", this.Ptr, "Ptr", color.Ptr)
    SetViewMode(mode) => DllCall("Color\ColorPickerSetViewMode", "Ptr", this.Ptr, "Int", mode)
    ToggleViewMode(direction := 1) => DllCall("Color\ColorPickerToggleViewMode", "Ptr", this.Ptr, "Int", direction)
    ToggleHighlight() => DllCall("Color\ColorPickerToggleHighlight", "Ptr", this.Ptr)

    OnStart(callback)
    {
        if callback is Func
            this.startCallback := CallbackCreate(callback)

        DllCall("Color\ColorPickerSetOnStart", "Ptr", this.Ptr, "Ptr", this.startCallback)
    }

    OnUpdate(callback)
    {
        if callback is Func
        {
            wrappedCallback(colorInt)
            {
                col := Color.FromPtr(colorInt)
                callback(col)
            }
            this.updateCallback := CallbackCreate(wrappedCallback)
        }
        DllCall("Color\ColorPickerSetOnUpdate", "Ptr", this.Ptr, "Ptr", this.updateCallback)
    }

    OnExit(callback)
    {
        if callback is Func
        {
            wrappedCallback(colorInt)
            {
                col := Color.FromPtr(colorInt)
                callback(col)
            }
            this.exitCallback := CallbackCreate(wrappedCallback)
        }
        DllCall("Color\ColorPickerSetOnExit", "Ptr", this.Ptr, "Ptr", this.exitCallback)
    }

    static RunDefault(copy := true)
    {
        currentColor := 0

        picker := ColorPicker("Hex", "#{R}{G}{B}")
        picker.SetViewMode(ColorPicker.ViewModes.Grid)
        picker.SetFontName("Consolas")
        picker.SetFontSize(14)
        picker.SetTextColor(Color.White)
        picker.SetTextBackgroundColor(Color.Black)
        picker.SetBorderColor(Color.Black)
        picker.SetViewModeColor(Color.Black)
        picker.SetHighlightColor(Color.White)
        picker.Start()

        while picker.IsActive()
        {
            currentColor := picker.GetCurrentColor()
            colorStr := picker.Value
            picker.SetTextBackgroundColor(currentColor)

            if GetKeyState("h", "P")
            {
                picker.ToggleHighlight()
                if !KeyWait("h", "T0.25")
                    continue
            }

            if GetKeyState("m")
            {
                picker.ToggleViewMode()
                if !KeyWait("m", "T0.25")
                    continue
            }

            if GetKeyState("Up") || GetKeyState("Numpad8")
            {
                if GetKeyState("Shift", "P")
                    picker.Move(0, -5)
                else
                    picker.Move(0, -1)
                if !KeyWait("Up", "T0.10") || !KeyWait("Numpad8", "T0.10")
                    continue
            }

            if GetKeyState("Down") || GetKeyState("Numpad2")
            {
                if GetKeyState("Shift", "P")
                    picker.Move(0, 5)
                else
                    picker.Move(0, 1)
                if !KeyWait("Down", "T0.10") || !KeyWait("Numpad2", "T0.10")
                    continue
            }

            if GetKeyState("Left") || GetKeyState("Numpad4")
            {
                if GetKeyState("Shift", "P")
                    picker.Move(-5, 0)
                else
                    picker.Move(-1, 0)
                if !KeyWait("Left", "T0.10") || !KeyWait("Numpad4", "T0.10")
                    continue
            }

            if GetKeyState("Right") || GetKeyState("Numpad6")
            {
                if GetKeyState("Shift", "P")
                    picker.Move(5, 0)
                else
                    picker.Move(1, 0)
                if !KeyWait("Right", "T0.10") || !KeyWait("Numpad6", "T0.10")
                    continue
            }

            if GetKeyState("[") || GetKeyState("NumpadDiv")
            {
                picker.ZoomOut()
                if !KeyWait("[", "T0.25") || !KeyWait("NumpadDiv", "T0.25")
                    continue
            }

            if GetKeyState("]") || GetKeyState("NumpadMult")
            {
                picker.ZoomIn()
                if !KeyWait("]", "T0.25") || !KeyWait("NumpadMult", "T0.25")
                    continue
            }

            if GetKeyState("-") || GetKeyState("NumpadSub")
            {
                picker.DecreaseCaptureSize()
                if !KeyWait("-", "T0.25") || !KeyWait("NumpadSub", "T0.25")
                    continue
            }

            if GetKeyState("=") || GetKeyState("NumpadAdd")
            {
                picker.IncreaseCaptureSize()
                if !KeyWait("=", "T0.25") || !KeyWait("NumpadAdd", "T0.25")
                    continue
            }

            if GetKeyState("Space") || GetKeyState("Enter") || GetKeyState("NumpadEnter") || GetKeyState("LButton")
            {
                picker.Stop()
                break
            }

            if GetKeyState("Escape") || GetKeyState("q")
            {
                picker.Stop()
                return
            }

            Sleep(16)
        }

        if copy
            A_Clipboard := colorStr

        return currentColor
    }
}

class ColorMatrix
{
    Ptr := 0

    __New(matrix?)
    {
        this.Ptr := DllCall("Color\CreateColorMatrix", "Ptr")

        if !IsSet(matrix)
        {
            ; Set identity matrix
            Loop 5
            {
                row := A_Index
                Loop 5
                    this[row, A_Index] := (row == A_Index) ? 1.0 : 0.0
            }
        }
        else
        {
            ; Set values from provided matrix array
            Loop 5
            {
                row := A_Index
                Loop 5
                    this[row, A_Index] := matrix[row][A_Index]
            }
        }
    }

    __Delete() => DllCall("Color\DeleteColorMatrix", "Ptr", this.Ptr)

    __Item[row, col]
    {
        get => DllCall("Color\GetMatrixValue", "Ptr", this.Ptr, "Int", row-1, "Int", col-1, "Double")
        set => DllCall("Color\SetMatrixValue", "Ptr", this.Ptr, "Int", row-1, "Int", col-1, "Double", value)
    }

    Add(other) => ColorMatrix.FromPtr(DllCall("Color\AddMatrices", "Ptr", this.Ptr, "Ptr", other.Ptr, "Ptr"))

    Subtract(other) => ColorMatrix.FromPtr(DllCall("Color\SubtractMatrices", "Ptr", this.Ptr, "Ptr", other.Ptr, "Ptr"))

    Multiply(other) => IsNumber(other) ? ColorMatrix.FromPtr(DllCall("Color\MultiplyMatrixByScalar", "Ptr", this.Ptr, "Double", other, "Ptr")) : ColorMatrix.FromPtr(DllCall("Color\MultiplyMatrices", "Ptr", this.Ptr, "Ptr", other.Ptr, "Ptr"))

    Transpose() => ColorMatrix.FromPtr(DllCall("Color\TransposeMatrix", "Ptr", this.Ptr, "Ptr"))

    static FromPtr(ptr) => { base: ColorMatrix.Prototype, Ptr: ptr }

    ; Inversion Matrix
    static Invert => ColorMatrix([
        [-1.000,  0.000,  0.000,  0.000,  1.000],
        [ 0.000, -1.000,  0.000,  0.000,  1.000],
        [ 0.000,  0.000, -1.000,  0.000,  1.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])

    ; Applies an effect reminiscent of Polaroid photographs
    static Polaroid => ColorMatrix([
        [ 1.438, -0.062, -0.062,  0.000,  0.000],
        [-0.122,  1.378, -0.122,  0.000,  0.000],
        [-0.016, -0.016,  1.483,  0.000,  0.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])

    ; Applies a technicolor effect
    static Technicolor => ColorMatrix([
        [ 1.600, -0.400, -0.200,  0.000,  0.000],
        [-0.200,  1.400, -0.200,  0.000,  0.000],
        [-0.200, -0.400,  1.600,  0.000,  0.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])

    ; Applies an effect similar to vintage film
    static VintageFilm => ColorMatrix([
        [ 0.900,  0.100,  0.100,  0.000,  0.000],
        [ 0.100,  0.900,  0.100,  0.000,  0.000],
        [ 0.100,  0.200,  0.700,  0.000,  0.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])

    ; Applies a kodachrome-like effect
    static Kodachrome => ColorMatrix([
        [ 1.200, -0.100,  0.000,  0.000,  0.100],
        [-0.050,  1.100,  0.000,  0.000,  0.050],
        [ 0.000, -0.100,  1.000,  0.000,  0.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])

    ; Applies an effect that imitates cross-processing
    static CrossProcess => ColorMatrix([
        [ 1.000,  0.200,  0.100,  0.000,  0.000],
        [ 0.000,  1.100, -0.100,  0.000,  0.000],
        [ 0.100,  0.100,  1.000,  0.000,  0.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])

    ; Applies a blue-channel grayscale effect
    static BlueChannelGrayscale => ColorMatrix([
        [ 0.000,  0.000,  1.000,  0.000,  0.000],
        [ 0.000,  0.000,  1.000,  0.000,  0.000],
        [ 0.000,  0.000,  1.000,  0.000,  0.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])

    ; Applies a green-channel grayscale effect
    static GreenChannelGrayscale => ColorMatrix([
        [ 0.000,  1.000,  0.000,  0.000,  0.000],
        [ 0.000,  1.000,  0.000,  0.000,  0.000],
        [ 0.000,  1.000,  0.000,  0.000,  0.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])

    ; Applies a red-channel grayscale effect
    static RedChannelGrayscale => ColorMatrix([
        [ 1.000,  0.000,  0.000,  0.000,  0.000],
        [ 1.000,  0.000,  0.000,  0.000,  0.000],
        [ 1.000,  0.000,  0.000,  0.000,  0.000],
        [ 0.000,  0.000,  0.000,  1.000,  0.000],
        [ 0.000,  0.000,  0.000,  0.000,  1.000]
    ])
}

class ColorStop
{
    Ptr := 0

    __New(position, col)
    {
        this.Ptr := DllCall("Color\ColorStopCreate", "Float", position, "Ptr", col.ToInt(), "Ptr")
    }

    __Delete() => DllCall("Color\ColorStopDelete", "Ptr", this.Ptr)

    Position
    {
        get => DllCall("Color\ColorStopGetPosition", "Ptr", this.Ptr, "Float")
        set => DllCall("Color\ColorStopSetPosition", "Ptr", this.Ptr, "Float", value)
    }

    Color
    {
        get => Color.FromPtr(DllCall("Color\ColorStopGetColor", "Ptr", this.Ptr, "Ptr"))
        set => DllCall("Color\ColorStopSetColor", "Ptr", this.Ptr, "Ptr", value.ToInt())
    }

    static FromPtr(ptr) => { base: ColorStop.Prototype, Ptr: ptr }
}

/**
 * A function to display a `Color`, `Gradient`, or `Canvas`, along with some information about it.
 * @param {Color|Gradient|Canvas} obj The Color, Gradient, or Canvas to display.
 * @param {String} title The title of the window.
 * @returns {Integer} wParam
 */
Showcase(obj, title := "Showcase")
{
    if obj is Color
        wParam := DllCall("Color\ShowColorShowcase", "Ptr", obj.Ptr, "AStr", title, "Int")
    else if obj is Gradient
        wParam := DllCall("Color\ShowGradientShowcase", "Ptr", obj.Ptr, "AStr", title, "Int")
    else if obj is Canvas
        wParam := DllCall("Color\ShowCanvasShowcase", "Ptr", obj.Ptr, "AStr", title, "Int")
    else
        throw Error("Invalid Type Received.")

    return wParam
}