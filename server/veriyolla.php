<?php
require 'vendor/autoload.php';  // PhpSpreadsheet kütüphanesini dahil edin

use PhpOffice\PhpSpreadsheet\Spreadsheet;
use PhpOffice\PhpSpreadsheet\Writer\Xlsx;
use PhpOffice\PhpSpreadsheet\IOFactory;

$fileName = 'data.xlsx';  // Excel dosyasının adı

// Eğer form gönderilmişse verileri işleyin
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $personID = $_POST['personID'] ?? "N/A";
    $areaID = $_POST['areaID'] ?? "N/A";
    $bpm = $_POST['bpm'] ?? "N/A";
    $latitude = $_POST['latitude'] ?? "N/A";
    $longitude = $_POST['longitude'] ?? "N/A";
    $altitude = $_POST['altitude'] ?? "N/A";

    // Eğer Excel dosyası varsa yükle, yoksa yeni bir tane oluştur
    if (file_exists($fileName)) {
        $spreadsheet = IOFactory::load($fileName);
        $sheet = $spreadsheet->getActiveSheet();
    } else {
        $spreadsheet = new Spreadsheet();
        $sheet = $spreadsheet->getActiveSheet();
        
        // Başlıkları ekle
        $sheet->setCellValue('A1', 'personID');
        $sheet->setCellValue('B1', 'areaID');
        $sheet->setCellValue('C1', 'bpm');
        $sheet->setCellValue('D1', 'latitude');
        $sheet->setCellValue('E1', 'longitude');
        $sheet->setCellValue('F1', 'altitude');
    }

    // Yeni veri satırını bul
    $lastRow = $sheet->getHighestRow() + 1;

    // Verileri Excel'e ekle
    $sheet->setCellValue('A' . $lastRow, $personID);
    $sheet->setCellValue('B' . $lastRow, $areaID);
    $sheet->setCellValue('C' . $lastRow, $bpm);
    $sheet->setCellValue('D' . $lastRow, $latitude);
    $sheet->setCellValue('E' . $lastRow, $longitude);
    $sheet->setCellValue('F' . $lastRow, $altitude);

    // Excel dosyasını kaydet
    $writer = new Xlsx($spreadsheet);
    $writer->save($fileName);

    echo "<p style='color: green;'>Veriler başarıyla kaydedildi!</p>";
}
?>

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <title>Veri Girişi</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            color: #333;
            padding: 20px;
        }
        h1 {
            text-align: center;
            color: #4CAF50;
        }
        form {
            max-width: 600px;
            margin: 0 auto;
            background-color: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
        }
        input[type="text"], input[type="number"] {
            width: 100%;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
        input[type="submit"] {
            background-color: #4CAF50;
            color: white;
            padding: 10px 15px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }
        input[type="submit"]:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>

<h1>Veri Girişi Formu</h1>

<form action="" method="post">
    <label for="personID">Person ID:</label>
    <input type="text" id="personID" name="personID" required>

    <label for="areaID">Area ID:</label>
    <input type="text" id="areaID" name="areaID" required>

    <label for="bpm">BPM (Kalp Atış Hızı):</label>
    <input type="number" id="bpm" name="bpm" required>

    <label for="latitude">Latitude (Enlem):</label>
    <input type="text" id="latitude" name="latitude" required>

    <label for="longitude">Longitude (Boylam):</label>
    <input type="text" id="longitude" name="longitude" required>

    <label for="altitude">Altitude (Rakım):</label>
    <input type="text" id="altitude" name="altitude" required>

    <input type="submit" value="Veriyi Kaydet">
</form>

</body>
</html>
