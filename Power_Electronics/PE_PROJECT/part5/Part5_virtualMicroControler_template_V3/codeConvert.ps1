param(
    [string]$Input = ""
)

# Find input file if not provided
$inputFile = $Input
if ($Input -eq "") {
    $cFiles = Get-ChildItem -Path "." -Filter "*.c"
    if ($cFiles.Count -gt 0) {
        $inputFile = $cFiles[0].Name
    }
}

Write-Host "Selected input file: $inputFile"

# Validate input file
if ($inputFile -eq "") {
    Write-Host "Input file name is not provided. Code conversion aborted!!"
    exit 1
}

if (-not (Test-Path -Path ".\$inputFile")) {
    Write-Host "Input file $inputFile not found. Code conversion aborted!!"
    exit 1
}

# Check if vMCU_template folder exists
if (-not (Test-Path -Path ".\vMCU_template")) {
    Write-Host "vMCU_template folder not found. Code conversion aborted!!"
    exit 1
}

# Check if all required template files exist
$requiredFiles = @(
    ".\vMCU_template\codeWrapper.cpp",
    ".\vMCU_template\codeWrapper.h",
    ".\vMCU_template\virtualCode.cpp",
    ".\vMCU_template\virtualCode.h",
    ".\vMCU_template\virtualControler.cpp",
    ".\vMCU_template\virtualControler.h"
)

$allFilesExist = $true
foreach ($file in $requiredFiles) {
    if (-not (Test-Path -Path $file)) {
        $allFilesExist = $false
        break
    }
}

if (-not $allFilesExist) {
    Write-Host "vMCU_template folder is not complete. Code conversion aborted!!"
    exit 1
}

# Remove existing vMCU folder if it exists
if (Test-Path -Path ".\vMCU") {
    Write-Host "Output folder vMCU exists, it will be regenerated."
    Remove-Item -Path ".\vMCU" -Recurse -Force
}

# Copy template folder to vMCU
Copy-Item -Path ".\vMCU_template" -Destination ".\vMCU" -Recurse

# Read input file content
$contentInput = Get-Content -Path $inputFile -Raw

# Read virtualCode.h content
$contentVirtualCode = Get-Content -Path ".\vMCU\virtualCode.h" -Raw

# Replace newlines with newlines + tab
$contentInput = $contentInput -replace "`n", "`n`t"

# Replace placeholder with user code
$contentVirtualCode = $contentVirtualCode -replace "//\*\*\*USER DEFINED CODE\*\*\*//", $contentInput

# Write modified content back to virtualCode.h
Set-Content -Path ".\vMCU\virtualCode.h" -Value $contentVirtualCode

Write-Host "`nCode conversion complete.`nPlease run the following command in Matlab to compile the code:`n"
Write-Host "`t`tmex vMCU_sfunction.cpp vMCU/virtualControler.cpp vMCU/codeWrapper.cpp vMCU/virtualCode.cpp`n"

Read-Host "Press Enter to continue"