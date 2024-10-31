$srcDir = "src"
$outputFile = "Color.dll"

if (Test-Path $outputFile)
{
    $response = Read-Host "$outputFile already exists. Do you want to overwrite it? (Y/N)"
    if ($response -ne "Y" -and $response -ne "y")
    {
        Write-Host "Build cancelled."
        exit
    }
}

$sourceFiles = "$srcDir/Color.cpp $srcDir/ColorBuffer.cpp $srcDir/Gradient.cpp"
$compilerFlags = "-DBUILDING_DLL -fPIC -std=c++17 -Wall -Wextra"
$linkerFlags = '-static -static-libgcc -static-libstdc++ "-Wl,--enable-stdcall-fixup" "-Wl,-Bstatic"'
$libraries = "-lgdi32 -fopenmp"
$includes = "-I$srcDir"

$buildCmd = "g++ -shared -o `"$outputFile`" $sourceFiles $compilerFlags $linkerFlags $libraries $includes"

Write-Host "Building $outputFile..."
Invoke-Expression $buildCmd

if ($LASTEXITCODE -eq 0)
{
    Write-Host "Build completed successfully."
}
else
{
    Write-Host "Build failed with exit code $LASTEXITCODE"
}