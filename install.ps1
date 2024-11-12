$username = $env:USERNAME
$libPath = "C:\Users\$username\OneDrive\Documents\AutoHotkey\Lib"
$files = @("Color.ahk", "Color.dll")

$reset = "`e[0m"
$cyan = "`e[36m"
$yellow = "`e[33m"
$red = "`e[31m"
$green = "`e[32m"

$existingFiles = $files | Where-Object { Test-Path (Join-Path $libPath $_) }

if ($existingFiles)
{
    Write-Host "The following files already exist in ${yellow}$libPath${reset}:"
    Write-Host "${cyan}$($existingFiles -join "${reset}, ${cyan}")${reset}"
    $response = Read-Host "Do you want to overwrite them? (Y/N)"
    if ($response -ne "Y" -and $response -ne "y")
    {
        Write-Host "${red}Installation cancelled.${reset}"
        exit
    }
}

foreach ($file in $files)
{
    $destPath = Join-Path $libPath $file
    Copy-Item $file $destPath -Force
    Write-Host "Installed ${cyan}$file${reset} to ${yellow}$libPath${reset}"
}

Write-Host "${green}Installation complete!${reset}`n"