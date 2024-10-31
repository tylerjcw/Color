$srcDir = "src"
$outputFile = "Color.dll"

# Check if Color.dll already exists
if (Test-Path $outputFile)
{
    $response = Read-Host "$outputFile already exists. Do you want to overwrite it? (Y/N)"
    if ($response -ne "Y" -and $response -ne "y")
    {
        Write-Host "Build cancelled."
        exit
    }
}

# Create build command with required flags
$buildCmd = "g++ -shared -o $outputFile $srcDir/Color.h -DBUILDING_DLL -fPIC -std=c++17 -Wall -Wextra -shared -fopenmp -I$srcDir"

# Execute the build command
Write-Host "Building $outputFile..."
Invoke-Expression $buildCmd

# Check if build was successful
if ($LASTEXITCODE -eq 0)
{
    Write-Host "Build completed successfully. Output: $outputFile"
}
else
{
    Write-Host "Build failed with exit code $LASTEXITCODE"
}
