param(
	[string]$ProcessName = "UnrealEditor"
)

$editorProcesses = Get-Process -Name $ProcessName -ErrorAction SilentlyContinue |
	Where-Object { -not [string]::IsNullOrWhiteSpace($_.MainWindowTitle) -or $_.MainWindowHandle -ne 0 } |
	Sort-Object StartTime -Descending

if (-not $editorProcesses)
{
	Write-Error "No running Unreal Editor window found for process '$ProcessName'."
	exit 1
}

$targetProcess = $editorProcesses | Select-Object -First 1

$shell = New-Object -ComObject WScript.Shell
$activated = $shell.AppActivate($targetProcess.Id)
if (-not $activated -and -not [string]::IsNullOrWhiteSpace($targetProcess.MainWindowTitle))
{
	$activated = $shell.AppActivate($targetProcess.MainWindowTitle)
}

if (-not $activated)
{
	Write-Error "Failed to activate Unreal Editor window (PID $($targetProcess.Id))."
	exit 1
}

Start-Sleep -Milliseconds 300
$shell.SendKeys('^%{F11}')

Write-Output "Triggered Unreal Live Coding for PID $($targetProcess.Id)."
