$srcDir = "src"
$includeDir = "include"
$outputFile = "Color.dll"

$reset = "`e[0m"
$cyan = "`e[36m"
$yellow = "`e[33m"
$red = "`e[31m"
$green = "`e[32m"

if (Test-Path $outputFile)
{
    $response = Read-Host "${yellow}$outputFile${reset} already exists. Do you want to overwrite it? (Y/N)"
    if ($response -ne "Y" -and $response -ne "y")
    {
        Write-Host "${red}Build cancelled.${reset}"
        exit
    }
}

$sourceFiles = @(
    "$srcDir/Color.cpp",
    "$srcDir/Canvas.cpp",
    "$srcDir/Gradient.cpp",
    "$srcDir/ColorPicker.cpp",
    "$srcDir/Showcase.cpp",
    "$srcDir/exports/CanvasExports.cpp",
    "$srcDir/exports/ColorExports.cpp",
    "$srcDir/exports/GradientExports.cpp"
    "$srcDir/exports/ColorPickerExports.cpp",
    "$srcDir/exports/ShowcaseExports.cpp"
) -join " "

$compilerFlags = "-DBUILDING_DLL -fPIC -std=c++17 -Wall -Wextra"
$linkerFlags = '-static -static-libgcc -static-libstdc++ "-Wl,--enable-stdcall-fixup" "-Wl,-Bstatic"'
$libraries = "-lgdi32 -lgdiplus -fopenmp"
$includes = "-I$includeDir"

$buildCmd = "g++ -shared -o `"$outputFile`" $sourceFiles $compilerFlags $linkerFlags $libraries $includes"

Write-Host "Building ${cyan}$outputFile${reset}..."
Invoke-Expression $buildCmd

if ($LASTEXITCODE -eq 0)
{
    Write-Host "${green}Build completed successfully.${reset}`n"
}
else
{
    Write-Host "${red}Build failed with exit code $LASTEXITCODE${reset}`n"
}