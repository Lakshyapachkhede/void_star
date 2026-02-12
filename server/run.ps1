$debug = $true

# Define directories and file names
$sourceDir = ".\src"  # Directory containing source files
$externalSourceDir = "..\shared\src"
$outputDir = "."  # Output directory for the compiled executable
$outputFile = "server_void_star.exe"  # Output executable name
$sdlLibs = @("SDL2.dll", "SDL2_net.dll")  # SDL libraries to copy to output


# Paths for SDL2 headers and libraries (adjust these paths for your setup)
$sdlIncludePath = "..\external\sdl2\include"
$sdlLibPath = "..\external\sdl2\lib"

$includePaths = @("..\external\sdl2\include", "..\external\cJSON\include","..\external\uthash\include", ".\include", "..\shared\include")



# Update compile flags to include paths



# Ensure output directory exists
if (!(Test-Path -Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir
}

# Find all source files
$sourceFiles = Get-ChildItem -Path $sourceDir, $externalSourceDir -Filter "*.c" -Recurse | Select-Object -ExpandProperty FullName

if (-not $sourceFiles) {
    Write-Host "No source files found in $sourceDir" -ForegroundColor Red
    exit 1
}

# Set compiler and flags
$compiler = "gcc"
$compileFlags = "-I$($includePaths -join ' -I') -L$sdlLibPath -L$sourceDir -lmingw32 -lSDL2main -lSDL2 -lSDL2_net  -std=c99 -Wall -Wextra -I$sourceDir"

if ($debug) {
    $compileFlags += " -g -O0 -DDEBUG -fno-omit-frame-pointer"
}
else {
    $compileFlags += " -O2 -DNDEBUG -mwindows"
}


$compileCommand = "$compiler $($sourceFiles -join ' ') $compileFlags -o $outputDir\$outputFile"


Write-Host "Compiling source files..." -ForegroundColor Yellow
Write-Host "Running: $compileCommand" -ForegroundColor Cyan

$startTime = Get-Date

Invoke-Expression -Command $compileCommand

$endTime = Get-Date

$elapsed = $endTime - $startTime
$minutes = [int]$elapsed.TotalMinutes
$seconds = $elapsed.Seconds

if ($minutes -gt 0) {
    $elapsedStr = "$minutes minute(s) $seconds second(s)"
} else {
    $elapsedStr = "$seconds second(s)"
}

if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed! Time taken: $elapsedStr" -ForegroundColor Red
    exit 1
} else {
    Write-Host "Compilation successful. Output: $outputDir\$outputFile. Time taken: $elapsedStr" -ForegroundColor Green
}


# Run the executable
Write-Host "Running the program..." -ForegroundColor Yellow
$executionCommand = ".\$outputFile"
Invoke-Expression -Command $executionCommand

