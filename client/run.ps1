$debug = $true

# Define directories and file names
$sourceDir = ".\src"  # Directory containing source files
$outputDir = "."  # Output directory for the compiled executable
$outputFile = "void_star.exe"  # Output executable name
$sdlLibs = @("SDL2.dll", "SDL2_image.dll", "SDL2_mixer.dll", "SDL2_ttf.dll")  # SDL libraries to copy to output


# Paths for SDL2 headers and libraries (adjust these paths for your setup)
$sdlIncludePath = ".\libs\sdl2\include"
$sdlLibPath = ".\libs\sdl2\lib"

$includePaths = @(".\libs\sdl2\include", ".\libs\cJSON\include",".\libs\uthash\include", ".\include")



# Update compile flags to include paths



# Ensure output directory exists
if (!(Test-Path -Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir
}

# Find all source files
$sourceFiles = Get-ChildItem -Path $sourceDir -Filter "*.c" -Recurse | ForEach-Object { $_.FullName }

if (-not $sourceFiles) {
    Write-Host "No source files found in $sourceDir" -ForegroundColor Red
    exit 1
}

# Set compiler and flags
$compiler = "gcc"
$compileFlags = "-I$($includePaths -join ' -I') -L$sdlLibPath -L$sourceDir -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -std=c99 -Wall -Wextra -I$sourceDir"

#gcc test.c -o test.exe -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -mwindows

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

